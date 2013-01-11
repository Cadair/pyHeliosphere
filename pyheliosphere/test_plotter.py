# -*- coding: utf-8 -*-
"""
Created on Thu Jan 10 14:31:19 2013

@author: stuart
"""
import voxread
import copy
from mayavi import mlab
from tvtk.api import tvtk
import numpy as np
#import cProfile

#den_file = "/home/stuart/GitHub/pyHeliosphere/Test Data/2008_06_06_03-STELab_IPS_CR2070_n.vox"
#vel_file = "/home/stuart/GitHub/pyHeliosphere/Test Data/2008_06_06_03-STELab_IPS_CR2070_v.vox"

den_file = "/home/stuart/Downloads/nv3h2099_9054_00001_n.vox"
vel_file = "/home/stuart/Downloads/nv3h2099_9054_00001_v.vox"

density_vox = voxread.voxread(den_file,memmap=False)
density_vox.info()
density = density_vox.volumes[0]
#density.calibrate()

#velocity_vox = voxread.voxread(vel_file)
#velocity_vox.info()
#velocity = velocity_vox.volumes[0]
##velocity.calibrate()
#velocity = velocity.volume
#Extract the Earth Coordinates for plotting
hdr = density_vox.volumes[0].header[8]
ep_x, ep_y, ep_z = map(float,hdr[2:-2].split()[2:])

#==============================================================================
# Begin Plotting
#==============================================================================
mlab.figure()

cont = mlab.contour3d(density.volume)
cont.actor.property.opacity = 0.2
cont.contour.number_of_contours = 1
cont.contour.auto_contours = False
cont.contour.contours = [20.0]

#
ep = mlab.points3d(ep_x,ep_y,ep_z)
ep.glyph.glyph.scale_factor = 3
#
cen = float(density_vox.list_extract(density_vox.volumes[0].header,'Center(CNTR)'))
sun = mlab.points3d(cen,cen,cen)
sun.glyph.glyph.scale_factor = 10

thresh = tvtk.Threshold()
image_data = cont.parent.parent.outputs[0]
thresh.input = image_data
thresh.threshold_between(15,18)

conFilter = tvtk.ConnectivityFilter()
conFilter.input = thresh.output
conFilter.extraction_mode = 'specified_regions'
conFilter.add_specified_region(0)
conFilter.update()

mmapper = tvtk.DataSetMapper()
mmapper.input = conFilter.output

#
#contv = mlab.contour3d(velocity)
#contv.actor.property.opacity = 0.2
#contv.contour.number_of_contours = 2
#contv.contour.minimum_contour = 80

#==============================================================================
# Select All Data Points inside selected area
#==============================================================================
#print cont.parent.parent.outputs[0].scalar_type

#image_data = cont.parent.parent.outputs[0]
#inside = tvtk.SelectEnclosedPoints()
#inside.check_surface = True
#inside.input = cont.parent.parent.outputs[0]
#inside.surface = cont.contour.outputs[0]
#inside.update()

#mlab.pipline.surface(inside.output)
#print "boo"
#mask = inside.output.point_data.scalars.to_array().reshape(density.size,order='F')
#
#filt = copy.copy(density.volume)
#filt[(mask == 0).nonzero()] = 0.0
#
#mlab.contour3d(filt)
#tostencil = tvtk.ImageToImageStencil()
#tostencil.input = inside.output
#tostencil.threshold_by_upper(0.5)
#
#stencil = tvtk.ImageStencil()
#stencil.set_input(2, tostencil.output)
##stencil.reverse_stencil = True
#stencil.background_value = 0.0
#stencil.input = cont.parent.parent.outputs[0]
#stencil.update()
#sel = tvtk.Selection()
#selnode = tvtk.SelectionNode()
#selnode.content_type = 6
#selnode.field_type = 1
#selnode.selection_list = [0.5,2.0]
#sel.add_node(selnode)
#
#extract = tvtk.ExtractSelectedThresholds()
#extract.set_input(0,inside.output)
#extract.set_input(1,sel)


#
#new_out = copy.copy(cont.parent.parent.outputs[0])
#print new_out.scalar_type
#
#mask = tvtk.ImageMask()
#mask.set_mask_input(inside.output)
#mask.set_image_input(cont.parent.parent.outputs[0])
#mask.set_output(new_out)
#mask.set_masked_output_value(0,1.0,0)
#mask.update()
#
#print mask.output.scalar_type, mask.output.scalar_range
#x = mask.output
#x.scalar_type = 'double'
#print x.scalar_range