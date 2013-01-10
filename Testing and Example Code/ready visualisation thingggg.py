from voxread import voxread
from enthought.mayavi import mlab
import enthought.mayavi.modules.api as mapi

tim = voxread('../2008_06_06_03-STELab_IPS_CR2070_n.vox')

minitim = mlab.pipeline.scalar_field(tim.volumes[0].volume)

TimmyTim = mlab.pipeline.contour(minitim)
TimTheDog = mlab.pipeline.surface(TimmyTim, opacity=0.5)
mlab.pipeline.add_module_manager(TimmyTim)
mlab.show()
