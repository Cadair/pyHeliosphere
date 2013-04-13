#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Volume.h"

///////////////////////////////////////////////////////////////////////////////
// Load a vox file
// Input: (char*) vox file path
// Output: Volume& current Volume
// Return: 		

bool loadVoxFile(char* path, Volume*& current)
{
	const char* VOXMAGICSTR  = "Vox1999a\n";
	const char* DENSITYMAGICSTR = "Density";
	const char* VELOCITYMAGICSTR = "Velocity";	
	const char* SAKPEAKMAGICSTR = "//SACPEAK";

	const int MAXLINELENGTH = 256;

	char line[MAXLINELENGTH];

	FILE* fp;
	fp = fopen(path,"r");
	if (fp == NULL)
		return false;

	unsigned int Nx, Ny, Nz;

	int VolumeType = -1; 
	const int DENSITYTYPE = 0;
	const int VELOCITYTYPE = 1;
	const int SAKPEAKTYPE = 2;

	//read file header

	// check if the file is a vox file
	if (NULL==fgets(line,MAXLINELENGTH,fp))
	{
		fclose(fp);
		return false;
	}
	if (strcmp(line, VOXMAGICSTR)!=0)
	{
		fclose(fp);
		return false;
	}
	

	// check the source of the volume
	if (NULL == fgets(line, MAXLINELENGTH, fp))
	{
		fclose(fp);
		return false;
	}

	int lineNum = 0;

	// pattern of SAKPEAK should appear before line 4 if there is one
	while(lineNum<4)
	{
		bool found = false;
		if (strncmp(line,SAKPEAKMAGICSTR,strlen(SAKPEAKMAGICSTR))==0)
		{
			VolumeType = SAKPEAKTYPE;
			found = true;
		}
		if(NULL == fgets(line,MAXLINELENGTH,fp))
		{
			fclose(fp);
			return false;
		}
		lineNum++;
		if (found) break;
	}

	if (VolumeType<0)
	{
		// pattern of Velocity /Density should appear before line 15
		while (lineNum<15)
		{
			bool found = false;
			if (strncmp(line,"Field",strlen("Field"))==0)
			{
				if (strstr(line, DENSITYMAGICSTR)!=NULL)
					VolumeType = DENSITYTYPE;
				else if (strstr(line, VELOCITYMAGICSTR)!=NULL)
					VolumeType = VELOCITYTYPE;
				found = true;				
			}
			if(NULL == fgets(line,MAXLINELENGTH,fp))
			{
				fclose(fp);
				return false;
			}

			lineNum++;
			if (found) break;
		}
	}

	if (VolumeType <0)
	{
		fprintf(stderr,"Unknown Volume source: %s\n", path);
		fclose(fp);
		return false;
	}
	
	// move to the information lines
	// the first information line should be before line 20
	while (lineNum < 20)
	{
		if (strncmp(line, "//", 2)==0) break;
		if(NULL == fgets(line,MAXLINELENGTH,fp))
		{
			fclose(fp);
			return false;
		}
		lineNum++;
	}
	if (strncmp(line, "//", 2)!=0)
	{
		fclose(fp);
		return false;
	}


	switch (VolumeType)
	{
		case DENSITYTYPE:
			current = new DensityVolume();
			break;
		case VELOCITYTYPE:
			current = new VelocityVolume();
			break;
		case SAKPEAKTYPE:
			current = new SakPeakVolume();
			break;
	}

	
	strcpy(current->VolumeFilename, path); 
	
	while (strncmp(line,"##", 2)!=0)
	{
		char* info = line+2;
		current->setInfo (info);
		
		if(NULL == fgets(line,MAXLINELENGTH,fp))
		{
			fclose(fp);
			return false;
		}
		lineNum++;

	}	

	//create VOLData from current volume file
	current->VolData = VLIVolume::CreateFromFile(current->VolumeFilename,0,VLIVolume::kCopyOnWrite );
	current->VolData ->GetSize( current->VolNx, current->VolNy, current->VolNz );

	fprintf( stderr, "'%s' Volume size = %d x %d x %d\n", current->VolumeFilename,
				current->VolNx, current->VolNy, current->VolNz );

	Nx = current->VolNx;
	Ny = current->VolNy;
	Nz = current->VolNz;
	

	//set volume file format
	unsigned long format = current->VolData->GetFormat();
	switch( format )
	{
		case kVLIVoxelFormatUINT8:
		  LutSize = 256;
		  break;

		case kVLIVoxelFormatUINT12L:
		case kVLIVoxelFormatUINT12U:
		        LutSize = 4096;
			break;

		default:
		        LutSize = 0;
			break;
	}
	fclose(fp);

	return true;
}
