# Makefile for Mesa under Acorn RISC OS - Main Makefile

# Author:		David Boddie
# Significant help:	Steven Purchase, Paul Boddie

# Created:	Sat 13th June 1998
# Last updated:	Fri 25th May 2001 (for 3.4.2)

############################################################################

# Mesa 3-D graphics library
# Version:  3.0
# Copyright (C) 1995-1998  Brian Paul
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the Free
# Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

# Attempt to recursively make the various components of the distribution

default :
	@echo Type one of the following:
	@echo
	@echo make install
	@echo make all
	@echo make clean

install :
	@echo Rearranging files.
	cdir h
	ifthere acconfig/h then rename acconfig/h h.acconfig
	ifthere include.GL.gl/h then rename include.GL.gl/h include.GL.h.gl
	ifthere include.GL.glext/h then rename include.GL.glext/h include.GL.h.glext
	ifthere include.GL.glu/h then rename include.GL.glu/h include.GL.h.glu
	ifthere include.GL.glu_mangle/h then rename include.GL.glu_mangle/h include.GL.h.glu_mangle
	ifthere include.GL.gl_mangle/h then rename include.GL.gl_mangle/h include.GL.h.gl_mangle
	ifthere include.GL.osmesa/h then rename include.GL.osmesa/h include.GL.h.osmesa
	ifthere include.GLView/h then rename include.GLView/h include.h.GLView

#	wipe include.GL.* ~C ~V ~R

	cdir src.c
	cdir src.h
	ifthere src.aatriangle/c then rename src.aatriangle/c src.c.aatriangle
	ifthere src.aatriangle/h then rename src.aatriangle/h src.h.aatriangle
	ifthere src.aatritemp/h then rename src.aatritemp/h src.h.aatritemp
	ifthere src.accum/c then rename src.accum/c src.c.accum
	ifthere src.accum/h then rename src.accum/h src.h.accum
	ifthere src.all/h then rename src.all/h src.h.all
	ifthere src.alpha/c then rename src.alpha/c src.c.alpha
	ifthere src.alpha/h then rename src.alpha/h src.h.alpha
	ifthere src.alphabuf/c then rename src.alphabuf/c src.c.alphabuf
	ifthere src.alphabuf/h then rename src.alphabuf/h src.h.alphabuf
#	ifthere src.api/h then rename src.api/h src.h.api
#	ifthere src.api1/c then rename src.api1/c src.c.api1
#	ifthere src.api2/c then rename src.api2/c src.c.api2
#	ifthere src.apiext/c then rename src.apiext/c src.c.apiext
#	ifthere src.asm_386/h then rename src.asm_386/h src.h.asm_386
#	ifthere src.asm_mmx/c then rename src.asm_mmx/c src.c.asm_mmx
#	ifthere src.asm_mmx/h then rename src.asm_mmx/h src.h.asm_mmx
	ifthere src.attrib/c then rename src.attrib/c src.c.attrib
	ifthere src.attrib/h then rename src.attrib/h src.h.attrib
	ifthere src.bbox/c then rename src.bbox/c src.c.bbox
	ifthere src.bbox/h then rename src.bbox/h src.h.bbox
	ifthere src.bitmap/c then rename src.bitmap/c src.c.bitmap
	ifthere src.bitmap/h then rename src.bitmap/h src.h.bitmap
	ifthere src.blend/c then rename src.blend/c src.c.blend
	ifthere src.blend/h then rename src.blend/h src.h.blend
	ifthere src.buffers/c then rename src.buffers/c src.c.buffers
	ifthere src.buffers/h then rename src.buffers/h src.h.buffers
	ifthere src.clip/c then rename src.clip/c src.c.clip
	ifthere src.clip/h then rename src.clip/h src.h.clip
	ifthere src.clip_funcs/h then rename src.clip_funcs/h src.h.clip_funcs
	ifthere src.clip_tmp/h then rename src.clip_tmp/h src.h.clip_tmp
	ifthere src.colortab/c then rename src.colortab/c src.c.colortab
	ifthere src.colortab/h then rename src.colortab/h src.h.colortab
	ifthere src.config/c then rename src.config/c src.c.config
	ifthere src.config/h then rename src.config/h src.h.config
	ifthere src.context/c then rename src.context/c src.c.context
	ifthere src.context/h then rename src.context/h src.h.context
	ifthere src.convolve/c then rename src.convolve/c src.c.convolve
	ifthere src.copypix/c then rename src.copypix/c src.c.copypix
	ifthere src.copypix/h then rename src.copypix/h src.h.copypix
	ifthere src.copy_tmp/h then rename src.copy_tmp/h src.h.copy_tmp
	ifthere src.cull_tmp/h then rename src.cull_tmp/h src.h.cull_tmp
	ifthere src.cva/c then rename src.cva/c src.c.cva
	ifthere src.cva/h then rename src.cva/h src.h.cva
	ifthere src.dd/h then rename src.dd/h src.h.dd
	ifthere src.debug_xform/c then rename src.debug_xform/c src.c.debug_xform
	ifthere src.debug_xform/h then rename src.debug_xform/h src.h.debug_xform
	ifthere src.depth/c then rename src.depth/c src.c.depth
	ifthere src.depth/h then rename src.depth/h src.h.depth
	ifthere src.dispatch/c then rename src.dispatch/c src.c.dispatch
	ifthere src.dlist/c then rename src.dlist/c src.c.dlist
	ifthere src.dlist/h then rename src.dlist/h src.h.dlist
	ifthere src.dotprod_tmp/h then rename src.dotprod_tmp/h src.h.dotprod_tmp
	ifthere src.drawpix/c then rename src.drawpix/c src.c.drawpix
	ifthere src.drawpix/h then rename src.drawpix/h src.h.drawpix
	ifthere src.enable/c then rename src.enable/c src.c.enable
	ifthere src.enable/h then rename src.enable/h src.h.enable
	ifthere src.enums/c then rename src.enums/c src.c.enums
	ifthere src.enums/h then rename src.enums/h src.h.enums
	ifthere src.eval/c then rename src.eval/c src.c.eval
	ifthere src.eval/h then rename src.eval/h src.h.eval
	ifthere src.extensions/c then rename src.extensions/c src.c.extensions
	ifthere src.extensions/h then rename src.extensions/h src.h.extensions
	ifthere src.feedback/c then rename src.feedback/c src.c.feedback
	ifthere src.feedback/h then rename src.feedback/h src.h.feedback
	ifthere src.fixed/h then rename src.fixed/h src.h.fixed
	ifthere src.fog/c then rename src.fog/c src.c.fog
	ifthere src.fog/h then rename src.fog/h src.h.fog
	ifthere src.fog_tmp/h then rename src.fog_tmp/h src.h.fog_tmp
	ifthere src.general_clip/h then rename src.general_clip/h src.h.general_clip
	ifthere src.get/c then rename src.get/c src.c.get
	ifthere src.get/h then rename src.get/h src.h.get
	ifthere src.glapi/c then rename src.glapi/c src.c.glapi
	ifthere src.glapi/h then rename src.glapi/h src.h.glapi
	ifthere src.glapinoop/c then rename src.glapinoop/c src.c.glapinoop
	ifthere src.glapinoop/h then rename src.glapinoop/h src.h.glapinoop
	ifthere src.glapioffsets/h then rename src.glapioffsets/h src.h.glapioffsets
	ifthere src.glapitable/h then rename src.glapitable/h src.h.glapitable
	ifthere src.glapitemp/h then rename src.glapitemp/h src.h.glapitemp
	ifthere src.glheader/h then rename src.glheader/h src.h.glheader
#	ifthere src.glmisc/c then rename src.glmisc/c src.c.glmisc
#	ifthere src.glmisc/h then rename src.glmisc/h src.h.glmisc
	ifthere src.glthread/c then rename src.glthread/c src.c.glthread
	ifthere src.glthread/h then rename src.glthread/h src.h.glthread
	ifthere src.hash/c then rename src.hash/c src.c.hash
	ifthere src.hash/h then rename src.hash/h src.h.hash
	ifthere src.highpc/c then rename src.highpc/c src.c.highpc
	ifthere src.hint/c then rename src.hint/c src.c.hint
	ifthere src.hint/h then rename src.hint/h src.h.hint
	ifthere src.image/c then rename src.image/c src.c.image
	ifthere src.image/h then rename src.image/h src.h.image
	ifthere src.imaging/c then rename src.imaging/c src.c.imaging
	ifthere src.imaging/h then rename src.imaging/h src.h.imaging
	ifthere src.indirect_tmp/h then rename src.indirect_tmp/h src.h.indirect_tmp
	ifthere src.interp_tmp/h then rename src.interp_tmp/h src.h.interp_tmp
	ifthere src.light/c then rename src.light/c src.c.light
	ifthere src.light/h then rename src.light/h src.h.light
	ifthere src.lines/c then rename src.lines/c src.c.lines
	ifthere src.lines/h then rename src.lines/h src.h.lines
	ifthere src.linetemp/h then rename src.linetemp/h src.h.linetemp
	ifthere src.lnaatemp/h then rename src.lnaatemp/h src.h.lnaatemp
	ifthere src.logic/c then rename src.logic/c src.c.logic
	ifthere src.logic/h then rename src.logic/h src.h.logic
	ifthere src.lowpc/c then rename src.lowpc/c src.c.lowpc
	ifthere src.macros/h then rename src.macros/h src.h.macros
	ifthere src.masking/c then rename src.masking/c src.c.masking
	ifthere src.masking/h then rename src.masking/h src.h.masking
	ifthere src.matrix/c then rename src.matrix/c src.c.matrix
	ifthere src.matrix/h then rename src.matrix/h src.h.matrix
	ifthere src.mem/c then rename src.mem/c src.c.mem
	ifthere src.mem/h then rename src.mem/h src.h.mem
#	ifthere src.misc/c then rename src.misc/c src.c.misc
#	ifthere src.misc/h then rename src.misc/h src.h.misc
	ifthere src.mmath/c then rename src.mmath/c src.c.mmath
	ifthere src.mmath/h then rename src.mmath/h src.h.mmath
#	ifthere src.mthreads/c then rename src.mthreads/c src.c.mthreads
#	ifthere src.mthreads/h then rename src.mthreads/h src.h.mthreads
	ifthere src.norm_tmp/h then rename src.norm_tmp/h src.h.norm_tmp
	ifthere src.pb/c then rename src.pb/c src.c.pb
	ifthere src.pb/h then rename src.pb/h src.h.pb
	ifthere src.pipeline/c then rename src.pipeline/c src.c.pipeline
	ifthere src.pipeline/h then rename src.pipeline/h src.h.pipeline
	ifthere src.pixel/c then rename src.pixel/c src.c.pixel
	ifthere src.pixel/h then rename src.pixel/h src.h.pixel
	ifthere src.pixeltex/c then rename src.pixeltex/c src.c.pixeltex
	ifthere src.pixeltex/h then rename src.pixeltex/h src.h.pixeltex
#	ifthere src.pointers/c then rename src.pointers/c src.c.pointers
#	ifthere src.pointers/h then rename src.pointers/h src.h.pointers
	ifthere src.points/c then rename src.points/c src.c.points
	ifthere src.points/h then rename src.points/h src.h.points
	ifthere src.polygon/c then rename src.polygon/c src.c.polygon
	ifthere src.polygon/h then rename src.polygon/h src.h.polygon
	ifthere src.quads/c then rename src.quads/c src.c.quads
	ifthere src.quads/h then rename src.quads/h src.h.quads
	ifthere src.rastpos/c then rename src.rastpos/c src.c.rastpos
	ifthere src.rastpos/h then rename src.rastpos/h src.h.rastpos
	ifthere src.readpix/c then rename src.readpix/c src.c.readpix
	ifthere src.readpix/h then rename src.readpix/h src.h.readpix
	ifthere src.rect/c then rename src.rect/c src.c.rect
	ifthere src.rect/h then rename src.rect/h src.h.rect
	ifthere src.render_tmp/h then rename src.render_tmp/h src.h.render_tmp
	ifthere src.scissor/c then rename src.scissor/c src.c.scissor
	ifthere src.scissor/h then rename src.scissor/h src.h.scissor
	ifthere src.shade/c then rename src.shade/c src.c.shade
	ifthere src.shade/h then rename src.shade/h src.h.shade
	ifthere src.shade_tmp/h then rename src.shade_tmp/h src.h.shade_tmp
	ifthere src.simple_list/h then rename src.simple_list/h src.h.simple_list
	ifthere src.span/c then rename src.span/c src.c.span
	ifthere src.span/h then rename src.span/h src.h.span
	ifthere src.stages/c then rename src.stages/c src.c.stages
	ifthere src.stages/h then rename src.stages/h src.h.stages
	ifthere src.state/c then rename src.state/c src.c.state
	ifthere src.state/h then rename src.state/h src.h.state
	ifthere src.stencil/c then rename src.stencil/c src.c.stencil
	ifthere src.stencil/h then rename src.stencil/h src.h.stencil
	ifthere src.texgen_tmp/h then rename src.texgen_tmp/h src.h.texgen_tmp
	ifthere src.teximage/c then rename src.teximage/c src.c.teximage
	ifthere src.teximage/h then rename src.teximage/h src.h.teximage
	ifthere src.texformat/c then rename src.texformat/c src.c.texformat
	ifthere src.texformat/h then rename src.texformat/h src.h.texformat
	ifthere src.texobj/c then rename src.texobj/c src.c.texobj
	ifthere src.texobj/h then rename src.texobj/h src.h.texobj
	ifthere src.texstate/c then rename src.texstate/c src.c.texstate
	ifthere src.texstate/h then rename src.texstate/h src.h.texstate
	ifthere src.texture/c then rename src.texture/c src.c.texture
	ifthere src.texture/h then rename src.texture/h src.h.texture
	ifthere src.texutil/c then rename src.texutil/c src.c.texutil
	ifthere src.texutil/h then rename src.texutil/h src.h.texutil
	ifthere src.texutil_tmp/h then rename src.texutil_tmp/h src.h.texutil_tmp
	ifthere src.translate/c then rename src.translate/c src.c.translate
	ifthere src.translate/h then rename src.translate/h src.h.translate
	ifthere src.trans_tmp/h then rename src.trans_tmp/h src.h.trans_tmp
	ifthere src.triangle/c then rename src.triangle/c src.c.triangle
	ifthere src.triangle/h then rename src.triangle/h src.h.triangle
	ifthere src.tritemp/h then rename src.tritemp/h src.h.tritemp
	ifthere src.types/h then rename src.types/h src.h.types
	ifthere src.varray/c then rename src.varray/c src.c.varray
	ifthere src.varray/h then rename src.varray/h src.h.varray
	ifthere src.vb/c then rename src.vb/c src.c.vb
	ifthere src.vb/h then rename src.vb/h src.h.vb
	ifthere src.vbcull/c then rename src.vbcull/c src.c.vbcull
	ifthere src.vbcull/h then rename src.vbcull/h src.h.vbcull
	ifthere src.vbfill/c then rename src.vbfill/c src.c.vbfill
	ifthere src.vbfill/h then rename src.vbfill/h src.h.vbfill
	ifthere src.vbindirect/c then rename src.vbindirect/c src.c.vbindirect
	ifthere src.vbindirect/h then rename src.vbindirect/h src.h.vbindirect
	ifthere src.vbrender/c then rename src.vbrender/c src.c.vbrender
	ifthere src.vbrender/h then rename src.vbrender/h src.h.vbrender
	ifthere src.vbxform/c then rename src.vbxform/c src.c.vbxform
	ifthere src.vbxform/h then rename src.vbxform/h src.h.vbxform
	ifthere src.vector/c then rename src.vector/c src.c.vector
	ifthere src.vector/h then rename src.vector/h src.h.vector
	ifthere src.vertices/c then rename src.vertices/c src.c.vertices
	ifthere src.vertices/h then rename src.vertices/h src.h.vertices
	ifthere src.winpos/c then rename src.winpos/c src.c.winpos
	ifthere src.winpos/h then rename src.winpos/h src.h.winpos
	ifthere src.xform/c then rename src.xform/c src.c.xform
	ifthere src.xform/h then rename src.xform/h src.h.xform
	ifthere src.xform_tmp/h then rename src.xform_tmp/h src.h.xform_tmp
	ifthere src.zoom/c then rename src.zoom/c src.c.zoom
	ifthere src.zoom/h then rename src.zoom/h src.h.zoom

	cdir src.X86.c
	cdir src.X86.h
	ifthere src.X86.3dnow/c then rename src.X86.3dnow/c src.X86.c.3dnow
	ifthere src.X86.3dnow/h then rename src.X86.3dnow/h src.X86.h.3dnow
	ifthere src.X86.assyntax/h then rename src.X86.assyntax/h src.X86.h.assyntax
	ifthere src.X86.common_x86/c then rename src.X86.common_x86/c src.X86.c.common_x86
	ifthere src.X86.common_x86asm/h then rename src.X86.common_x86asm/h src.X86.h.common_x86asm
	ifthere src.X86.mmx/h then rename src.X86.mmx/h src.X86.h.mmx
	ifthere src.X86.x86/c then rename src.X86.x86/c src.X86.c.x86
	ifthere src.X86.x86/h then rename src.X86.x86/h src.X86.h.x86
#	wipe src.X86.*/* ~C ~V ~R
#	wipe src.DOS ~C ~V ~R
#	wipe src.FX ~C ~V ~R
#	wipe src.MGL ~C ~V ~R
#	wipe src.SVGA ~C ~V ~R
#	wipe src.Windows ~C ~V ~R
#	wipe src.X ~C ~V ~R
#	wipe src.*/* ~C ~V ~R

	cdir src-glu.c
	cdir src-glu.h
	ifthere src-glu.all/h then rename src-glu.all/h src-glu.h.all
	ifthere src-glu.glu/c then rename src-glu.glu/c src-glu.c.glu
	ifthere src-glu.gluP/h then rename src-glu.gluP/h src-glu.h.gluP
	ifthere src-glu.mipmap/c then rename src-glu.mipmap/c src-glu.c.mipmap
	ifthere src-glu.nurbs/c then rename src-glu.nurbs/c src-glu.c.nurbs
	ifthere src-glu.nurbs/h then rename src-glu.nurbs/h src-glu.h.nurbs
	ifthere src-glu.nurbscrv/c then rename src-glu.nurbscrv/c src-glu.c.nurbscrv
	ifthere src-glu.nurbssrf/c then rename src-glu.nurbssrf/c src-glu.c.nurbssrf
	ifthere src-glu.nurbsutl/c then rename src-glu.nurbsutl/c src-glu.c.nurbsutl
	ifthere src-glu.polytest/c then rename src-glu.polytest/c src-glu.c.polytest
	ifthere src-glu.project/c then rename src-glu.project/c src-glu.c.project
	ifthere src-glu.quadric/c then rename src-glu.quadric/c src-glu.c.quadric
	ifthere src-glu.tess/c then rename src-glu.tess/c src-glu.c.tess
	ifthere src-glu.tess/h then rename src-glu.tess/h src-glu.h.tess
	ifthere src-glu.tesselat/c then rename src-glu.tesselat/c src-glu.c.tesselat
#	ifthere src-glu.tess_clip/c then rename src-glu.tess_clip/c src-glu.c.tess_clip
#	ifthere src-glu.tess_clip/h then rename src-glu.tess_clip/h src-glu.h.tess_clip
#	ifthere src-glu.tess_fist/c then rename src-glu.tess_fist/c src-glu.c.tess_fist
#	ifthere src-glu.tess_fist/h then rename src-glu.tess_fist/h src-glu.h.tess_fist
#	ifthere src-glu.tess_hash/c then rename src-glu.tess_hash/c src-glu.c.tess_hash
#	ifthere src-glu.tess_hash/h then rename src-glu.tess_hash/h src-glu.h.tess_hash
#	ifthere src-glu.tess_heap/c then rename src-glu.tess_heap/c src-glu.c.tess_heap
#	ifthere src-glu.tess_heap/h then rename src-glu.tess_heap/h src-glu.h.tess_heap
#	ifthere src-glu.tess_macros/h then rename src-glu.tess_macros/h src-glu.h.tess_macros
#	ifthere src-glu.tess_typedefs/h then rename src-glu.tess_typedefs/h src-glu.h.tess_typedefs
#	ifthere src-glu.tess_winding/c then rename src-glu.tess_winding/c src-glu.c.tess_winding
#	ifthere src-glu.tess_winding/h then rename src-glu.tess_winding/h src-glu.h.tess_winding

#	wipe src-glu.*/* ~C ~V ~R

all :
	@echo Entering src directory.
	dir src
	make -r
	dir ^
	@echo Entering src-glu directory.
	dir src-glu
	make -r
	dir ^
	@echo All done.

clean :
	wipe src.o.* ~C ~V
	wipe src-glu.o.* ~C ~V
