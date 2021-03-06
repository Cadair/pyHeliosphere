# -*- coding: utf-8 -*-
"""
Created on Thu Feb 17 17:17:42 2011

@author: Stuart Mumford

This is the vox read module, it will read in Standard VOX files and specific 
UCSD heliospheric VOX files. The file manual for VOX files can be found at:
http://www.terarecon.com/downloads/support/vp500_vol_file_format.pdf
"""
#import Modules, this uses the io stream module from python 2.6 (readinto)
import numpy as np
#from memory_profiler import profile
class Error(Exception):
    """Base error Class"""
    pass

class VOXError(Error):
    """Error with vox file format or syntax"""
    def __init__(self, msg):
        self.msg = msg
    def __str__(self):
        return "%s"%(self.msg) 

class ReadError(VOXError):
    """Error reading in a vox file"""
    pass

class Volume():
    """This is a simple class that holds a voxel and its header """
#    @profile
    def __init__(self, File, struct, voxel, volume_dimensions, VDR, memmap=True):
        """This Class is initilised by passing a file object, vox struct dict,
        the voxel number to read and the voxel header"""
        self.size = volume_dimensions
        self.header = VDR
        #Extract Volume File Locations from struct        
        volume_start = struct["VDR%i"%voxel]
        volume_end = struct["Volume%i"%voxel]
        
        #Seek to volume start
        File.seek(volume_start)
        
        #Read in Volume
        if memmap:
            self.volume = np.memmap(File, dtype=np.uint8, mode='r',
                                offset=volume_start)
        else:
            self.volume = np.fromfile(File, dtype=np.uint8,
                                      count=volume_end - volume_start)
        
        
        self.volume = self.volume.reshape(volume_dimensions)
        
        if File.tell() != volume_end:
            raise ReadError("Error Reading in Volume %i, Read Wrong Number of Bytes"%voxel)

    def __str__(self):
        return "A %s Volume"%(self.size.__str__())
    
    def string_extract(self, line, stripper):
        """Extracts the varibles from a line, with name stripper"""
        return line.strip(stripper)
        
    def list_extract(self, headerlist, var_name):
        """This extracts a varible from a header list (either HDR or VDR)
        Returns the stripped list or -1 if not found """
        #print headerlist
        for line in headerlist:
            if line.find(var_name) != -1:
                return self.string_extract(line, var_name)
        return -1
        
#    @profile
    def calibrate(self):
        """ Converts the byte arrays into physical units"""
        nmin = float(self.list_extract(self.header, "min"))
        nmax = float(self.list_extract(self.header, "max"))
        
        self.volume = np.array(self.volume, dtype=np.float64,order='F')
        self.volume /= np.max(self.volume)
        self.volume += nmin
        self.volume *= nmax
    

class voxread():
    """This is a standard vox read class, it will read in the Header of the file
    and the headers of any found volume headers, as well as each of the volume
    blocks. This class is not gauranteed to import any VOX file with more than
    one volume inside it, as the author only has single volume files to test it.
    Also no attempt has been made to process multiple field data.

    Calling:
        vox = voxread(filename)
        
        vox then holds:
            vox.header -> The File header
            vox.volumes -> A list of Volume instances holding the header and 
                            Voxel data for each volume in the file
            vox.info -> Print info about the volumes in a file
            vox.struct -> A Dictionary holding byte locations of each file section.
            
            
        Vol = vox.volume[0]
            Vol then holds:
                Vol.volume
                Vol.header
                Vol.size
    
    """
    #Define Global Varibles
    #Standard Header strings
    voxHDR = 'Vox1999a\n'
    
    def __init__(self, filename, memmap=True):
        """File open and header read function"""
        self.filename = filename
        self.memmap = memmap
        #Open file
        self.vox = open(filename, 'rb')
        #Check that it is a vox file, based on the first line of file
        #Create header list
        self.header = []
        #Read in first line
        line1 = self.vox.readline()
        if line1 == self.voxHDR:
            self.header.append(line1)
        else:
            raise VOXError("This file does not appear to be a standard VOX file"
            + " it does not have '%s' as its first line"%self.voxHDR)
        
        #Read rest of first header section
        line = False
        while line != '##\x0c\n':
            line = self.vox.readline()
            #Read next line
            self.header.append(line.strip('\n').strip('/'))
        
        #There may be a line in the header specificing the number of volumes
        #the file holds, if it there find it and extract it.
        self.volume_count = self.list_extract(self.header, "VolumeCount")
        if self.volume_count == -1:
            self.volume_count = 1
        else:
            self.volume_count = int(self.volume_count[1])
        #Create a dictionary of the file structure
        self.struct = {"HDR":self.vox.tell()} #Header Length
        
        self.vol_read()
    
    def info(self):
        print self.filename
        try:
            from prettytable import PrettyTable
        except:
            print "___INSTALL PrettyTable FOR BETTER PRINT OUTS!___\n"
            print "Voxel,", "Voxel Dimensions,", "Header,","File Location"
            for i,V in enumerate(self.volumes):
                print i,",",V.size,",%s Lines,"%len(V.header),self.struct["VDR%i"%i]
            return
        
        inf = PrettyTable()
        inf.field_names = ["Volume", "Volume Dimensions", "Header","File Location"]
        for i,V in enumerate(self.volumes):
            inf.add_row([i,V.size,"%s Lines"%len(V.header),self.struct["VDR%i"%i]])
        return inf.printt()
        
    def list_extract(self, headerlist, var_name):
        """This extracts a varible from a header list (either HDR or VDR)
        Returns the stripped list or -1 if not found """
        #print headerlist
        for line in headerlist:
            if line.find(var_name) != -1:
                return self.string_extract(line, var_name)
        return -1
        
    def string_extract(self, line, stripper):
        """Extracts the varibles from a line, with name stripper"""
        return line.strip(stripper)
    
    def vol_read(self):
        """Reads all VDR's and length of voxel data"""
        self.volumes = [] # A listy list of volumes and their headers
        
        for i in xrange(0,self.volume_count):
            #Read in Header
            vdr = self.VDR_read()
            
            #Save File Position of End of VDR
            self.struct["VDR%i"%i] = self.vox.tell()
            
            #Read in VoxelSize params
            VolumeDimensions = map(int,self.list_extract(vdr,"VolumeSize").split())
            VoxelSize = int(self.list_extract(vdr,"VoxelSize"))
            VolumeLength = 1
            
            #multiply all items in a list together
            for each in VolumeDimensions:
                VolumeLength *= each
                
            #Calculate Volume Length based on the formula in the specifications
            VolumeLength = int(np.floor((VolumeLength*VoxelSize + 7)/8.0))
            self.struct["Volume%i"%i] = VolumeLength + self.vox.tell()
            
            self.volumes.append(Volume(self.vox, self.struct, i,
                                       VolumeDimensions, vdr, 
                                       memmap=self.memmap))
            
    def VDR_read(self):
        """Reads the Volume Header, needs file in the right place"""
        VDRlines = []
        line = False
        while line != '##\x0c\n':
            line = self.vox.readline()
            #Read next line
            VDRlines.append(line.strip('\n').strip('/'))

        return VDRlines
