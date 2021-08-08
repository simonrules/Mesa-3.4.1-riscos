/* vgears. */

/*
 * Spinning gears demo for Linux SVGA/Mesa interface in 32K color mode.
 *
 * Compile with:  gcc vgears.c -I../include -L../lib -lMesaGL -lX11 -lXext
 *   -lvga -lm -o vgears
 *
 * Brian Paul, January 1996
 */

#include <stdio.h>
#include <stdlib.h>
#include "GL/rosmesa.h"
#include <math.h>
#include <string.h>
#include <swis.h>
#include <kernel.h>

#define WIDTH 128
#define HEIGHT 128
#define HEADER 60
#define BPP 8
#define LOG2BPP 3
#define UNITS 4

_kernel_swi_regs	regs;
_kernel_oserror		error;

ROSMesaContext ctx;

   void *buffer;
   int	window_block[23] = {0, 256, 256, 256+(WIDTH*UNITS), 256+(HEIGHT*UNITS), 0, 0, -1, 0x87040012, 0xff070207, 0x000c0103, 0, -HEIGHT*UNITS, WIDTH*UNITS, 0, 0x2700003d, 0x00002000, 1, 0, 000, 0, 0, 0};
	char		task_name[16] = "Gears";
	unsigned int	task_handle;
   int	icon_block[9] = {0, 0, -HEIGHT*UNITS, WIDTH*UNITS, 0, 0x2102, 0, 0, 0};
   int	message[64];
   char	sprite_name[12] = "display";

/*
 * Draw a gear wheel.  You'll probably want to call this function when
 * building a display list since we do a lot of trig here.
 *
 * Input:  inner_radius - radius of hole at center
 *         outer_radius - radius at center of teeth
 *         width - width of gear
 *         teeth - number of teeth
 *         tooth_depth - depth of tooth
 */
static void gear( GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
		  GLint teeth, GLfloat tooth_depth )
{
   GLint i;
   GLfloat r0, r1, r2;
   GLfloat angle, da;
   GLfloat u, v, len;

   r0 = inner_radius;
   r1 = outer_radius - tooth_depth/2.0;
   r2 = outer_radius + tooth_depth/2.0;

   da = 2.0*M_PI / teeth / 4.0;

   glShadeModel( GL_FLAT );

   glNormal3f( 0.0, 0.0, 1.0 );

   /* draw front face */
   glBegin( GL_QUAD_STRIP );
   for (i=0;i<=teeth;i++) {
      angle = i * 2.0*M_PI / teeth;
      glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
      glVertex3f( r1*cos(angle), r1*sin(angle), width*0.5 );
      glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
      glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 );
   }
   glEnd();

   /* draw front sides of teeth */
   glBegin( GL_QUADS );
   da = 2.0*M_PI / teeth / 4.0;
   for (i=0;i<teeth;i++) {
      angle = i * 2.0*M_PI / teeth;

      glVertex3f( r1*cos(angle),      r1*sin(angle),      width*0.5 );
      glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),   width*0.5 );
      glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), width*0.5 );
      glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 );
   }
   glEnd();


   glNormal3f( 0.0, 0.0, -1.0 );

   /* draw back face */
   glBegin( GL_QUAD_STRIP );
   for (i=0;i<=teeth;i++) {
      angle = i * 2.0*M_PI / teeth;
      glVertex3f( r1*cos(angle), r1*sin(angle), -width*0.5 );
      glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
      glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
      glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
   }
   glEnd();

   /* draw back sides of teeth */
   glBegin( GL_QUADS );
   da = 2.0*M_PI / teeth / 4.0;
   for (i=0;i<teeth;i++) {
      angle = i * 2.0*M_PI / teeth;

      glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
      glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 );
      glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),   -width*0.5 );
      glVertex3f( r1*cos(angle),      r1*sin(angle),      -width*0.5 );
   }
   glEnd();


   /* draw outward faces of teeth */
   glBegin( GL_QUAD_STRIP );
   for (i=0;i<teeth;i++) {
      angle = i * 2.0*M_PI / teeth;

      glVertex3f( r1*cos(angle),      r1*sin(angle),       width*0.5 );
      glVertex3f( r1*cos(angle),      r1*sin(angle),      -width*0.5 );
      u = r2*cos(angle+da) - r1*cos(angle);
      v = r2*sin(angle+da) - r1*sin(angle);
      len = sqrt( u*u + v*v );
      u /= len;
      v /= len;
      glNormal3f( v, -u, 0.0 );
      glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),    width*0.5 );
      glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),   -width*0.5 );
      glNormal3f( cos(angle), sin(angle), 0.0 );
      glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da),  width*0.5 );
      glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 );
      u = r1*cos(angle+3*da) - r2*cos(angle+2*da);
      v = r1*sin(angle+3*da) - r2*sin(angle+2*da);
      glNormal3f( v, -u, 0.0 );
      glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da),  width*0.5 );
      glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
      glNormal3f( cos(angle), sin(angle), 0.0 );
   }

   glVertex3f( r1*cos(0), r1*sin(0), width*0.5 );
   glVertex3f( r1*cos(0), r1*sin(0), -width*0.5 );

   glEnd();


   glShadeModel( GL_SMOOTH );

   /* draw inside radius cylinder */
   glBegin( GL_QUAD_STRIP );
   for (i=0;i<=teeth;i++) {
      angle = i * 2.0*M_PI / teeth;
      glNormal3f( -cos(angle), -sin(angle), 0.0 );
      glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
      glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
   }
   glEnd();

}


static GLfloat view_rotx=20.0, view_roty=30.0, view_rotz=0.0;
static GLint gear1, gear2, gear3;
static GLfloat angle = 0.0;

static GLuint limit;
static GLuint count = 1;

static void draw( void )
{
   angle += 2.0;

   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glPushMatrix();
   glRotatef( view_rotx, 1.0, 0.0, 0.0 );
   glRotatef( view_roty, 0.0, 1.0, 0.0 );
   glRotatef( view_rotz, 0.0, 0.0, 1.0 );

   glPushMatrix();
   glTranslatef( -3.0, -2.0, 0.0 );
   glRotatef( angle, 0.0, 0.0, 1.0 );
   glCallList(gear1);
   glPopMatrix();

   glPushMatrix();
   glTranslatef( 3.1, -2.0, 0.0 );
   glRotatef( -2.0*angle-9.0, 0.0, 0.0, 1.0 );
   glCallList(gear2);
   glPopMatrix();

   glPushMatrix();
   glTranslatef( -3.1, 4.2, 0.0 );
   glRotatef( -2.0*angle-25.0, 0.0, 0.0, 1.0 );
   glCallList(gear3);
   glPopMatrix();

   glPopMatrix();

}


static void init( void )
{
   static GLfloat pos[4] = {5.0, 5.0, 10.0, 1.0 };
   static GLfloat red[4] = {0.8, 0.1, 0.0, 1.0 };
   static GLfloat green[4] = {0.0, 0.8, 0.2, 1.0 };
   static GLfloat blue[4] = {0.2, 0.2, 1.0, 1.0 };

   GLfloat w = (float) WIDTH / (float) HEIGHT;
   GLfloat h = 1.0;

   glLightfv( GL_LIGHT0, GL_POSITION, pos );
   glEnable( GL_CULL_FACE );
   glEnable( GL_LIGHTING );
   glEnable( GL_LIGHT0 );
   glEnable( GL_DEPTH_TEST );

   /* make the gears */
   gear1 = glGenLists(1);
   glNewList(gear1, GL_COMPILE);
   glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red );
   gear( 1.0, 4.0, 1.0, 20, 0.7 );
   glEndList();

   gear2 = glGenLists(1);
   glNewList(gear2, GL_COMPILE);
   glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green );
   gear( 0.5, 2.0, 2.0, 10, 0.7 );
   glEndList();

   gear3 = glGenLists(1);
   glNewList(gear3, GL_COMPILE);
   glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue );
   gear( 1.3, 2.0, 0.5, 10, 0.7 );
   glEndList();

   glEnable( GL_NORMALIZE );


   glViewport( 0, 0, WIDTH, HEIGHT );
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (WIDTH>HEIGHT) {
      GLfloat w = (GLfloat) WIDTH / (GLfloat) HEIGHT;
      glFrustum( -w, w, -1.0, 1.0, 5.0, 60.0 );
   }
   else {
      GLfloat h = (GLfloat) HEIGHT / (GLfloat) WIDTH;
      glFrustum( -1.0, 1.0, -h, h, 5.0, 60.0 );
   }

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef( 0.0, 0.0, -40.0 );

/* Added by David Boddie (Thu 18th March 1999) */
  glDisable(GL_DITHER);

}

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

void setup( void )
{

	/* Write the window title */
	strcpy((char *)&window_block[19], "Gears");

   /* Create a single buffered RGB8-mode context */
   ctx = ROSMesaCreateContext( ROSMESA_RGB, 8, GL_FALSE );
   if (ctx==NULL) exit(0);

   /* Allocate the image buffer */
	buffer = malloc( (size_t) WIDTH * HEIGHT + HEADER );

   /* Initialise a sprite area */

	*(unsigned int *)buffer		= WIDTH * HEIGHT + HEADER;
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
	regs.r[6] = (4 << 27) | ((180/UNITS) << 14) | ((180/UNITS) << 1) | 1;
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


int main( int argc, char *argv[] )
{
   int	quit = 0;

   setup();
   init();

do
{
	unsigned int	code;

	/* Poll */
		regs.r[0] = (unsigned int)0;
		regs.r[1] = (unsigned int)&message;
		regs.r[2] = 10;
	   _kernel_swi(Wimp_PollIdle, &regs, &regs);

	code = (unsigned int)regs.r[0];

/*
	fprintf(output, "code = %i\n", code);
*/

		switch (code)
		{
			case 0:		/* Null */
					draw();
					view_roty += 2.0;
					if (view_roty >= 360.0) view_roty = view_roty - 360.0;
					regs.r[0] = window_block[0];
					regs.r[1] = 0;
					regs.r[2] = -HEIGHT*UNITS;
					regs.r[3] = WIDTH*UNITS;
					regs.r[4] = 0;
					_kernel_swi(Wimp_ForceRedraw, &regs, &regs);
					break;
			case 2:		/* Open window */
					_kernel_swi(Wimp_OpenWindow, &regs, &regs);
					break;
                	case 3:		/* Close window */
                			_kernel_swi(Wimp_CloseWindow, &regs, &regs);
                			quit = 1;
                			break;
                	case 17:
                	case 18:
                			if (message[4] == 0)
                			{
                				quit = 1;
                                        }
                                        break;
                        default:
                          break;
		};
} while (quit == 0);

   end();
   return 0;
}
