# -*- coding: utf-8 -*-
"""
Created on Tue Mar 15 18:31:44 2011

@author: Stuart Mumford

Inital attempt at a Mario Mayavi UI
"""

#Standard Imports

from voxread import voxread
import numpy as np

#Traits and MayaVi imports
from enthought.traits.api import HasTraits, Str, File, ListInstance, Instance, List, Property
from enthought.traits.ui.api import View, Item, VSplit, Handler
from enthought.mayavi.tools.mlab_scene_model import MlabSceneModel
from enthought.mayavi import mlab
from enthought.tvtk.pyface.scene_editor import SceneEditor 
from enthought.mayavi.core.ui.engine_view import EngineView
from enthought.traits.ui.menu import ApplyButton


class VoxEditor(HasTraits):
    """This class holds a vox file and related attribs"""
    voxfile = File(exists=True, filter=["*.vox"])
    vox_info = Str
    vox_volumes = ListInstance
    vox_header = List

    traits_view = View(Item(name="voxfile"),
                       Item(name="vox_volumes",style="readonly"),
                        buttons = [ApplyButton])
    
    def _voxfile_changed(self):
        """On change of voxfile do things!"""
        self.vox = voxread(self.voxfile)
        self.vox_info = self.vox.info()
        self.vox_volumes = self.vox.volumes
        self.vox_header = self.vox.header

class DataVis(HasTraits):
    vox = Instance("VoxEditor",())
    scene = Instance(MlabSceneModel, ())
    engine_view = Instance(EngineView)
    current_selection = Property
    
    traits_view = View(VSplit(Item("vox",show_label=False),
                              Item('scene', height=400, show_label=False,
                                   editor=SceneEditor())))
    
    def __init__(self, **traits):
        HasTraits.__init__(self, **traits)
        self.engine_view = EngineView(engine=self.scene.engine)
        self.vox.on_trait_change(self.update_plot, 'voxfile')
        
    def update_plot(self):
        mlab.contour3d(self.vox.vox_volumes[0].volume,figure=self.scene.mayavi_scene)

x = DataVis()
x.configure_traits()