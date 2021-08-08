/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * US Government Users Restricted Rights
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */
/*
 *  tea.c
 *  This program demonstrates two-sided lighting and compares
 *  it with one-sided lighting.  Three teapots are drawn, with
 *  a clipping plane to expose the interior of the objects.
 */

#include <stdio.h>
#include <stdlib.h>
#include "GL/rosmesa.h"
#include <GL/glu.h>
#include <stdlib.h>
#include "glaux.h"
#include <swis.h>
#include <kernel.h>

#define WIDTH 640
#define HEIGHT 480

/*  Initialize light source.
 */
void myinit(void)
{
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
/*	light_position is NOT default value	*/
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glFrontFace (GL_CW);
    glDisable(GL_DITHER);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
}

void display(void)
{
    GLfloat mat_diffuse1[] = { 0.8, 0.2, 0.2, 1.0 };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, mat_diffuse1);
    glPushMatrix ();
    glTranslatef (0.0, 0.0, 0.0);
    glRotatef (30.0, 1.0, 0.0, 0.0); /* Rotate by 30 degrees about the x-axis */
    auxSolidTeapot(2.0);	/*  one-sided lighting	*/
    glPopMatrix ();

    glFlush();
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
	glOrtho (-4.0, 4.0, -4.0*(GLfloat)h/(GLfloat)w,
	    4.0*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
    else
	glOrtho (-4.0*(GLfloat)w/(GLfloat)h,
	    4.0*(GLfloat)w/(GLfloat)h, -4.0, 4.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

/*  Main Loop
 *  Open window with initial window size, title bar,
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
   ROSMesaContext ctx;
   void *buffer;
   int temp[3];
   _kernel_swi_regs regs;
   int	mode_block[6] = {1, WIDTH, HEIGHT, 4, -1, -1};

   /* Create an RGBA-mode context */
   ctx = ROSMesaCreateContext( ROSMESA_RGB, 16, GL_FALSE );
   if (ctx==NULL) return 0;

/* Change mode */
   regs.r[0] = 0;
   regs.r[1] = (unsigned int)&mode_block;

   _kernel_swi(OS_ScreenMode, &regs, &regs);

/* Remove cursors */
   _kernel_swi(OS_RemoveCursors, &regs, &regs);

   /* Allocate the image buffer */
   regs.r[0] = (unsigned int)&temp;
   regs.r[1] = (unsigned int)&temp[2];

   temp[0] = 148;
   temp[1] = -1;

   _kernel_swi(OS_ReadVduVariables, &regs, &regs);
   buffer = (int *) temp[2];

   /* Bind the buffer to the context and make it current */
   ROSMesaMakeCurrent( ctx, buffer, NULL, GL_UNSIGNED_BYTE, WIDTH, HEIGHT );

   myinit();
   myReshape(WIDTH, HEIGHT);
   display();

   /* destroy the context */
   ROSMesaDestroyContext( ctx );

   return 0;
}
