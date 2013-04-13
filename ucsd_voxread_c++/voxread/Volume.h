#ifndef VOLUME_H
#define VOLUME_H

#include "core.h"

class Volume
{
public:
	char VolumeFilename[MAXPATHLENGTH]; /* vox file */
	char LutFilename[MAXPATHLENGTH]; // reserved for future usage
	unsigned int VolNx, VolNy, VolNz; /* volume dimensions */
	VLIVolume *VolData; /* the volume in VP1000 land */
	double range1, range2; /* reserved  */
	double min; /* min value */
	double max; /* max value */
	double top; /* top value */
	double center; // reserved for future usage
	virtual void setInfo(char* line); 
};


class DVVolume: public Volume
{
public:
	char TimeString[TIMESTRINGLEN]; /* time string associated with the volume */
	VLIVector3D ep; /* earth position */
	VLIVector3D orbitPs[3]; /* 3 points used to calculate the earth orbit*/
	virtual void setInfo(char* line);
};

class DensityVolume: public DVVolume
{
public:
	DensityVolume();
	double baseDensityVal; /* base density value */
	virtual void setInfo(char* line);
};

class VelocityVolume: public DVVolume
{
};

class SakPeakVolume: public Volume
{
public:
	double Carrington;
	char textureFile[MAXPATHLENGTH];
	virtual void setInfo(char* line);
};

#endif