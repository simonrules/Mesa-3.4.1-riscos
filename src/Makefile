# Makefile for Mesa under Acorn RISC OS - GL library

# Author:		David Boddie
# Significant help:	Steven Purchase, Paul Boddie
# Last updated:		Fri 04th May 2001 (for 3.4.1)

############################################################################

include ../Make-cfg

INCDIR = ../include

CC = gcc -c -munixlib -mapcs -O3 -DUSE_ARM_ASM -I$(INCDIR) -I../src

CORE_OBJECTS = aatriangle.o accum.o alpha.o alphabuf.o attrib.o bbox.o \
               bitmap.o blend.o buffers.o clip.o colortab.o config.o \
               context.o convolve.o copypix.o cva.o debug_xform.o depth.o \
               dispatch.o dlist.o drawpix.o enable.o enums.o eval.o \
               extensions.o feedback.o fog.o get.o glapi.o glapinoop.o \
               glthread.o hash.o highpc.o hint.o image.o imaging.o \
               light.o lines.o logic.o lowpc.o masking.o matrix.o \
               mem.o mmath.o pb.o pipeline.o pixel.o pixeltex.o \
               points.o polygon.o quads.o rastpos.o readpix.o rect.o \
               scissor.o shade.o span.o stages.o state.o stencil.o \
               teximage.o texobj.o texstate.o texture.o texutil.o \
               translate.o triangle.o varray.o vb.o vbcull.o vbfill.o \
               vbindirect.o vbrender.o vbxform.o vector.o vertices.o winpos.o \
               xform.o zoom.o

DRIVER_OBJECTS = rosmesa.o ros_asm.o

OBJECTS = $(CORE_OBJECTS) $(DRIVER_OBJECTS)

############################################################################
# Rules

default : ../lib/libGL

../lib/libGL : $(OBJECTS)
	$(MAKELIB) $@ $(OBJECTS)

.SUFFIXES : .o .c

rosmesa.o : RISCOS/rosmesa.c
	$(CC) RISCOS/rosmesa.c -O3

ros_asm.o : RISCOS/ros_asm.s
	$(AS) RISCOS/ros_asm.s -o ros_asm.o

x86.o : X86/x86.c
	$(CC) X86/x86.c

# o.xform : RISCOS.o.xform_asm RISCOS.o.xform_c
# 	ld -aof -o $@ o.xform_asm o.xform_c

# RISCOS.o.xform_asm : RISCOS.s.xform_asm
# 	$(AS) RISCOS/xform_asm.s -o o.xform_asm

# RISCOS.o.xform_c : RISCOS.c.xform_c
# 	$(CC) RISCOS/xform_c.c -o o.xform_c

%.o : %.c
	$(CC) $<

clean :
	wipe *.o ~C ~V
