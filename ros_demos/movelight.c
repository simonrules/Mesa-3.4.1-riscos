/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
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
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 *  movelight.c
 *  This program demonstrates when to issue lighting and
 *  transformation commands to render a model with a light
 *  which is moved by a modeling transformation (rotate or
 *  translate).  The light position is reset after the modeling
 *  transformation is called.  The eye position does not change.
 *
 *  A sphere is drawn using a grey material characteristic.
 *  A single light source illuminates the object.
 *
 *  Interaction:  pressing the left mouse button alters
 *  the modeling transformation (x rotation) by 30 degrees.
 *  The scene is then redrawn with the light in a new position.
 */

/* -------------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "GL/rosmesa.h"
#include "GL/glu.h"
#include "glaux.h"
#include <math.h>
#include <string.h>
#include <sys/swis.h>
#include <kernel.h>

#define WX 256
#define WY 256
#define WIDTH 128
#define HEIGHT 128
#define HEADER 60
#define BPP 32
#define LOG2BPP 5
#define UNITS 4

static	_kernel_swi_regs	regs;
static	_kernel_oserror		error;

ROSMesaContext ctx;

/* Image buffer */
void	*buffer;

/* Task details */
char		task_name[16] = "MoveLight";
unsigned int	task_handle;

/* Window details */
int	window_block[23] = {0, 256, 256, WX+(WIDTH*UNITS), WY+(HEIGHT*UNITS), 0, 0, -1, 0x87141012, 0xff070207, 0x000c0103, 0, -HEIGHT*UNITS, WIDTH*UNITS, 0, 0x2700003d, 0x00002000, 1, 0, 000, 0, 0, 0};
int	icon_block[9] = {0, 0, -HEIGHT*UNITS, WIDTH*UNITS, 0, 0x2102, 0, 0, 0};
char	sprite_name[12] = "display";

int	wx = WX, wy = WY;

/* Menu details */
int	menu_block[15];

int	message[64];
int	workspace[64];

void clear_screen(void *buffer)
{
	int	i;
	GLuint *ptr4;

	ptr4 = (GLuint *) buffer;

	for (i=0;i < (WIDTH*HEIGHT*BPP/32); i++)
	{
		ptr4[i] = 0;
	}
}

void	swapbuffers( void )
{
	/* Do nothing */
}

void	redisplay( void )
{
	regs.r[0] = window_block[0];
	regs.r[1] = 0;
	regs.r[2] = -HEIGHT*UNITS;
	regs.r[3] = WIDTH*UNITS;
	regs.r[4] = 0;
	_kernel_swi(Wimp_ForceRedraw, &regs, &regs);
}

void	glutWireCube(GLfloat size)
{
	auxWireCube(size);
}

void	glutSolidTorus(GLfloat a1, GLfloat a2, GLfloat a3, GLfloat a4)
{
	auxSolidTorus(a1, a2);
}

void	glutPostRedisplay( void )
{
	redisplay();
}

#define GLUT_LEFT_BUTTON 4
#define GLUT_DOWN 1

/* -------------------------------------------------------------------------------- */

static int spin = 0;

/*  Initialize material property, light source, lighting model,
 *  and depth buffer.
 */
void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
}

/*  Here is where the light position is reset after the modeling
 *  transformation (glRotated) is called.  This places the
 *  light at a new position in world coordinates.  The cube
 *  represents the position of the light.
 */
void display(void)
{
   GLfloat position[] = { 0.0, 0.0, 1.5, 1.0 };

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix ();
   gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glPushMatrix ();
   glRotated ((GLdouble) spin, 1.0, 0.0, 0.0);
   glLightfv (GL_LIGHT0, GL_POSITION, position);

   glTranslated (0.0, 0.0, 1.5);
   glDisable (GL_LIGHTING);
   glColor3f (0.0, 1.0, 1.0);
   glutWireCube (0.1);
   glEnable (GL_LIGHTING);
   glPopMatrix ();

   glutSolidTorus (0.275, 0.85, 8, 15);
   glPopMatrix ();
   glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN) {
            spin = (spin + 30) % 360;
            glutPostRedisplay();
         }
         break;
      default:
         break;
   }
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

/* -------------------------------------------------------------------------------- */

void setup( void )
{

	/* Write the window title */
	strcpy((char *)&window_block[19], "MoveLight");

	strcpy((char *)menu_block, "MoveLight");
	menu_block[3] = 0x00070207;
	menu_block[4] = (17+1)*16;
	menu_block[5] = 44;
	menu_block[6] = 0;

	menu_block[7] = 128;
	menu_block[8] = -1;
	menu_block[9] = 0x07000101;
	menu_block[10] = (unsigned int)(&menu_block[13]);	/* text buffer */
	menu_block[11] = (unsigned int)(&menu_block[14]);	/* validation string */
	menu_block[12] = 4;		/* buffer length */
	strcpy((char *)(unsigned int)&(menu_block[13]), "Quit");
	menu_block[14] = 0;

   /* Create a single buffered RGBA-mode context */
   ctx = ROSMesaCreateContext( ROSMESA_RGBA, BPP, GL_FALSE );
   if (ctx==NULL) exit(0);

   /* Allocate the image buffer */
	buffer = malloc( (size_t) WIDTH * HEIGHT * 4 + HEADER );

   /* Initialise a sprite area */

	*(unsigned int *)buffer		= WIDTH * HEIGHT * 4 + HEADER;
	*(unsigned int *)(buffer + 8)	= 0;
	*(unsigned int *)(buffer + 8)	= 16;
	*(unsigned int *)(buffer + 8)	= 16;

	regs.r[0] = 256+9;
	regs.r[1] = (unsigned int)buffer;
   _kernel_swi(OS_SpriteOp, &regs, &regs);

	regs.r[0] = 256+15;
	regs.r[1] = (unsigned int)buffer;
	regs.r[2] = (unsigned int)&sprite_name;
	regs.r[3] = 0;
	regs.r[4] = WIDTH;
	regs.r[5] = HEIGHT;
	regs.r[6] = (6 << 27) | ((180/UNITS) << 14) | ((180/UNITS) << 1) | 1;
   _kernel_swi(OS_SpriteOp, &regs, &regs);

  clear_screen(buffer+HEADER);

  /* Bind the buffer to the context and make it current */
  if (ROSMesaMakeCurrent( ctx, buffer+HEADER, NULL, GL_UNSIGNED_BYTE, WIDTH, HEIGHT )==GL_FALSE)
  {
  	exit(0);
  }

/* Initialise task */
	regs.r[0] = (unsigned int)310;
	regs.r[1] = (unsigned int)0x4b534154;
	regs.r[2] = (unsigned int)&task_name;
	regs.r[3] = 0;
   _kernel_swi(Wimp_Initialise, &regs, &regs);
	task_handle = regs.r[1];

/* Create window */
	regs.r[1] = (unsigned int)(&window_block) + 4;
   _kernel_swi(Wimp_CreateWindow, &regs, &regs);
   	window_block[0] = regs.r[0];

/* Create icon */
        icon_block[0] = window_block[0];
        icon_block[6] = (unsigned int)(buffer) + 20;
	icon_block[7] = (unsigned int)buffer;
	icon_block[8] = 7;
        regs.r[0] = 0;
        regs.r[1] = (unsigned int)&icon_block;
   _kernel_swi(Wimp_CreateIcon, &regs, &regs);

/* Open window */
        regs.r[1] = (unsigned int)&window_block;
   _kernel_swi(Wimp_OpenWindow, &regs, &regs);

}

void poll( void (*idle_function)(void), void (*display_function)(void), void (*keyboard_function)(unsigned char, int, int), void (*mouse_function)(int, int, int, int) )
{
	int	quit = 0;

	do
	{
		unsigned int	code;

		/* Poll */
			regs.r[0] = (unsigned int)0;
			regs.r[1] = (unsigned int)&message;
			regs.r[2] = 10;
		   _kernel_swi(Wimp_PollIdle, &regs, &regs);

		code = (unsigned int)regs.r[0];

		switch (code)
		{
			case 0:		/* Null */
					if (display_function != NULL) display_function();
					if (idle_function != NULL) idle_function();
					break;
			case 2:		/* Open window */
					_kernel_swi(Wimp_OpenWindow, &regs, &regs);
					/* Maintain the position information */
					wx = message[1];
					wy = message[2];
					break;
                	case 3:		/* Close window */
                			_kernel_swi(Wimp_CloseWindow, &regs, &regs);
                			quit = 1;
                			break;
			case 4:		/* Pointer leaving window */
					regs.r[0] = -1;
					_kernel_swi(Wimp_SetCaretPosition, &regs, &regs);
					break;
			case 5:		/* Pointer entering window */
					regs.r[0] = window_block[0];
					regs.r[1] = -1;
					_kernel_swi(Wimp_SetCaretPosition, &regs, &regs);
					break;
			case 6:		/* Button */
                        		switch (message[2])
					{
						case 2:
	                				/* Open menu */

        	                                	/* Get pointer position */
                	                        	regs.r[1] = (unsigned int)&workspace;
							_kernel_swi(Wimp_GetPointerInfo, &regs, &regs);
                                	        	regs.r[1] = (unsigned int)&menu_block;
                                        		regs.r[2] = workspace[0] - 64;
                                        		regs.r[3] = workspace[1];
	                                        	_kernel_swi(Wimp_CreateMenu, &regs, &regs);
						default:
							/* Use mouse event handler if there is one */
							if (mouse_function != NULL)
							{
	        	                                	/* Get pointer position */
        	        	                        	regs.r[1] = (unsigned int)&workspace;
								_kernel_swi(Wimp_GetPointerInfo, &regs, &regs);

								/* Pass position from window lower-left corner */
								mouse_function(message[2] & GLUT_LEFT_BUTTON, GLUT_DOWN, workspace[0] - wx, workspace[1] - wy);
							}
					}
                                        break;
			case 8:		/* Key pressed */
					if (keyboard_function != NULL) keyboard_function(message[6], 0, 0);
					break;
			case 9:		/* Menu selection */

					/* Quit? */
					if (message[0] == 0) quit = 1;

                                       	/* Get pointer position */
                                       	regs.r[1] = (unsigned int)&message;
					_kernel_swi(Wimp_GetPointerInfo, &regs, &regs);
					if (message[2] == 1)
					{
                                        	regs.r[1] = (unsigned int)&menu_block;
                                        	regs.r[2] = message[0] - 64;
                                        	regs.r[3] = message[1];
                                        	_kernel_swi(Wimp_CreateMenu, &regs, &regs);
					}
					break;
                	case 17:
                	case 18:
                			if (message[4] == 0)
                			{
                				quit = 1;
                                        }
                                        break;
                        default:
		}
	} while (quit == 0);
}

void end( void )
{
/* Close window */
        regs.r[1] = (unsigned int)&window_block;
   _kernel_swi(Wimp_CloseWindow, &regs, &regs);

/* Delete window */
        regs.r[1] = (unsigned int)&window_block;
   _kernel_swi(Wimp_DeleteWindow, &regs, &regs);

/* Closedown */
	regs.r[0] = task_handle;
	regs.r[1] = (unsigned int)0x4b534154;
   _kernel_swi(Wimp_CloseDown, &regs, &regs);

   free(buffer);

   /* destroy the context */
   ROSMesaDestroyContext( ctx );
}

/* -------------------------------------------------------------------------------- */

int main(int argc, char** argv)
{
/*
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
*/
/* -------------------------------------------------------------------------------- */
	setup();
/* -------------------------------------------------------------------------------- */
   init ();
/*
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
*/
/* -------------------------------------------------------------------------------- */
   reshape(WIDTH, HEIGHT);

   poll(redisplay, display, keyboard, mouse);
   end();
   exit(0);
}
