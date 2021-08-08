# Makefile for Mesa under Acorn RISC OS - GLU library

# Author:		David Boddie
# Significant help:	Steven Purchase, Paul Boddie

############################################################################

include ../Make-cfg

INCDIR = ../include

CC = gcc -c -munixlib -mapcs -O3 -DUSE_ARM_ASM -I$(INCDIR) -I../src-glu

OBJECTS = glu.o mipmap.o nurbs.o nurbscrv.o nurbssrf.o nurbsutl.o \
          polytest.o project.o quadric.o tess.o tesselat.o
#          tess_clip.o
#          tess_fist.o tess_hash.o tess_heap.o tess_winding.o

############################################################################
# Rules

default : ../libGLU.lib

../libGLU.lib : $(OBJECTS)
	$(MAKELIB) $@ $(OBJECTS)

.SUFFIXES : .o .c

%.o : %.c
	$(CC) $<

clean :
	wipe *.o
