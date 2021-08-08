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
 *  cube.c
 *  Draws a 3-D cube, viewed with perspective, stretched
 *  along the y-axis.
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <GL/rosmesa.h>
#include "glaux.h"
#include <sys/swis.h>
#include <kernel.h>

#define WIDTH 320
#define HEIGHT 256

/*  Clear the screen.  Set the current color to white.
 *  Draw the wire frame cube.
 */
void display (void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);
    glLoadIdentity ();	/*  clear the matrix	*/
    glTranslatef (0.0, 0.0, -5.0);	/*  viewing transformation	*/
    glScalef (1.0, 2.0, 1.0);	/*  modeling transformation	*/
    auxWireCube(1.0);	/*  draw the cube	*/
    glFlush();
}

void myinit (void) {
    glShadeModel (GL_FLAT);
}

/*  Called when the window is first opened and whenever
 *  the window is reconfigured (moved or resized).
 */
void myReshape(int w, int h)
{
    glMatrixMode (GL_PROJECTION);	/*  prepare for and then  */
    glLoadIdentity ();	/*  define the projection  */
    glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);	/*  transformation  */
    glMatrixMode (GL_MODELVIEW);	/*  back to modelview matrix	*/
    glViewport (0, 0, w, h);	/*  define the viewport	*/
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
   int	mode_block[6] = {1, 320, 256, 5, -1, -1};

   /* Create an RGBA-mode context */
   ctx = ROSMesaCreateContext( ROSMESA_RGBA, 32, GL_FALSE );
   if (ctx==NULL) return 0;

   /* Allocate the image buffer */
   regs.r[0] = (unsigned int)&temp;
   regs.r[1] = (unsigned int)&temp[2];

   temp[0] = 148;
   temp[1] = -1;

   _kernel_swi(OS_ReadVduVariables, &regs, &regs);
   buffer = (int *) temp[2];

/* Change mode */
   regs.r[0] = 0;
   regs.r[1] = (unsigned int)&mode_block;

   _kernel_swi(OS_ScreenMode, &regs, &regs);

   /* Bind the buffer to the context and make it current */
   ROSMesaMakeCurrent( ctx, buffer, NULL, GL_UNSIGNED_BYTE, WIDTH, HEIGHT );

/*
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB);
    auxInitPosition (0, 0, 500, 500);
    if (!auxInitWindow (argv[0]))
       auxQuit();
*/
    myinit ();
/*
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
*/
	myReshape( WIDTH, HEIGHT );
	display();

   /* destroy the context */
   ROSMesaDestroyContext( ctx );

    return 0;
}
