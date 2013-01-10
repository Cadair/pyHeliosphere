# -*- coding: utf-8 -*-
"""
Created on Thu Jan 10 14:31:19 2013

@author: stuart
"""
import voxread
from mayavi import mlab

den_file = "/home/stuart/GitHub/pyHeliosphere/Test Data/2008_06_06_03-STELab_IPS_CR2070_n.vox"
vel_file = "/home/stuart/GitHub/pyHeliosphere/Test Data/2008_06_06_03-STELab_IPS_CR2070_v.vox"

density_vox = voxread.voxread(den_file)
density = density_vox.volumes[0].volume

velocity_vox = voxread.voxread(vel_file)
velocity = velocity_vox.volumes[0].volume
#Extract the Earth Coordinates for plotting
hdr = density_vox.volumes[0].header[8]
ep_x, ep_y, ep_z = map(float,hdr[2:-2].split()[2:])

#==============================================================================
# Begin Plotting
#==============================================================================
mlab.figure()

cont = mlab.contour3d(density)
cont.actor.property.opacity = 0.2
cont.contour.number_of_contours = 7
cont.contour.minimum_contour = 80

ep = mlab.points3d(ep_x,ep_y,ep_z)
ep.glyph.glyph.scale_factor = 3

#contv = mlab.contour3d(velocity)
#contv.actor.property.opacity = 0.2
#contv.contour.number_of_contours = 7
#contv.contour.minimum_contour = 80