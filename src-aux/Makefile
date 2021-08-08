# Makefile for Mesa under Acorn RISC OS - aux library

# Author:		David Boddie
# Significant help:	Steven Purchase, Paul Boddie

############################################################################

include ../Make-cfg

INCDIR = ../include

CC = gcc $(CFLAGS) -I$(INCDIR) -I../src

OBJECTS = shapes.o teapot.o vect3d.o xxform.o

############################################################################
# Rules

default : ../libAux.lib

../libAux.lib : $(OBJECTS)
	$(MAKELIB) $@ $(OBJECTS)

.SUFFIXES : .o .c

%.o : %.c
	$(CC) $<

clean :
	wipe o.* ~C ~V
