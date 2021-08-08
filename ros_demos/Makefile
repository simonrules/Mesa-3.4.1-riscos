# Makefile for Mesa under Acorn RISC OS - RISC OS demos

# Author:		David Boddie
# Significant help:	Steven Purchase, Paul Boddie

############################################################################

# Flags for the compiler - choose your configuration

# gcc and UnixLib
CFLAGS = -c -munixlib -O3
# gcc and Shared C Library
# CFLAGS = -c -mstubs
# Norcroft and UnixLib
# CFLAGS = -W -c -Otime -JUnix:

############################################################################

# Your preferred linker

# makeALF - to make a library
# MAKELIB = makeALF -o
# ld - to statically link the object files
MAKELIB = ld -rescan gcc:o.libgcc unix:o.unixlib ../libGL.lib \
          ../libGLU.lib ../libAux.lib -o
# MAKELIB = gcc -L^.lib -lGL -lGLU -lAux -o
# MAKELIB = ld -rescan gcc:o.libgcc unix:o.unixlib ^.lib.libMesaGL \
#           ^.lib.libMesaGLU ^.lib.libMesaAux -o
# MAKELIB = ld -rescan gcc:o.libgcc c:o.stubs ^.lib.libMesaGL \
#           ^.lib.libMesaGLU ^.lib.libMesaAux -o

############################################################################

# Your choice of C compiler

INCDIR = ../include

CC = gcc $(CFLAGS) -I$(INCDIR) -I../src

OBJECTS = anti32.o cone2.o cube3.o cube32.o \
	  morph3d.o morph32.o movelight.o \
	  rosdemo8.o rosdemo16.o rosdemo32.o \
	  teapot8.o teapot16.o teapot32.o

DEMOS = $(OBJECTS:=.o)

############################################################################
# Rules

default :

.SUFFIXES : .o .c

% : %.o
	$(MAKELIB) demos.$@ $<

%.o : %.o
	$(CC) $<
