# -*- coding: utf-8 -*-
"""
Created on Tue Mar 15 18:44:36 2011

@author: Stuart Mumford

This is my first experiment with Traits and Traits UI

This should, Have an interface, open a vox file and return the info of the file
in a pop-up Dialoug... hopefully
"""

from voxread import voxread

from enthought.traits.api import HasTraits, Str, File, ListInstance, List
from enthought.traits.ui.api import View, Item

class VoxEditor(HasTraits):
    """This class holds a vox file and related attribs"""
    voxfile = File(exists=True, filter=["*.vox"])
    vox_info = Str
    vox_volumes = ListInstance
    vox_header = List

    traits_view = View(Item(name="voxfile"),
                       Item(name="vox_volumes",style="readonly")
                       )
    
    def _voxfile_changed(self):
        self.vox = voxread(self.voxfile)
        self.vox_info = self.vox.info()
        self.vox_volumes = self.vox.volumes
        self.vox_header = self.vox.header

f = VoxEditor()
f.configure_traits()

