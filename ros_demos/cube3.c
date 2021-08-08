/* cube3 */

/*
 * Smooth colour-shaded cube
 *
 * David Boddie (Tue 04th May 1999)
 * from an earlier demo by
 * Brian Paul, January 1996
 */

#include <stdio.h>
#include <stdlib.h>
#include "GL/rosmesa.h"
#include <math.h>
#include <string.h>
#include <sys/swis.h>
#include <kernel.h>

#define WIDTH 128
#define HEIGHT 128
#define HEADER 60
#define BPP 32
#define LOG2BPP 5
#define UNITS 4

static	_kernel_swi_regs	regs;
static	_kernel_oserror		error;

ROSMesaContext ctx;

   void *buffer;
   int	window_block[23] = {0, 256, 256, 256+(WIDTH*UNITS), 256+(HEIGHT*UNITS), 0, 0, -1, 0x87040012, 0xff070207, 0x000c0103, 0, -HEIGHT*UNITS, WIDTH*UNITS, 0, 0x2700003d, 0x00002000, 1, 0, 000, 0, 0, 0};
	char		task_name[16] = "Cube";
	unsigned int	task_handle;
   int	icon_block[9] = {0, 0, -HEIGHT*UNITS, WIDTH*UNITS, 0, 0x2102, 0, 0, 0};
   int	message[64];
   char	sprite_name[12] = "display";
   int	menu_block[15];



/*
 * Draw a tetrahedron of size, size.
 */

static void cube( GLfloat size )
{
   GLfloat red[4] = {0.8, 0.1, 0.0, 1.0 };
   GLfloat green[4] = {0.0, 0.8, 0.2, 1.0 };
   GLfloat blue[4] = {0.2, 0.2, 1.0, 1.0 };
   GLfloat yellow[4] = {0.8, 0.8, 0.2, 1.0 };
   GLfloat white[4] = {1.0, 1.0, 1.0, 1.0 };

   GLfloat shininess[] =   {60.0};
   GLfloat specular[]  =   { 0.7, 0.7, 0.7, 1.0 };

   glShadeModel( GL_SMOOTH );

   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

   /* draw front face */
   glNormal3f( 0.0, 0.0, 1.0 );

   glBegin( GL_QUAD_STRIP );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red );
      glVertex3f( -size, -size, size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green );
      glVertex3f(  size, -size, size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue );
      glVertex3f( -size,  size, size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white );
      glVertex3f(  size,  size, size );
   glEnd();

   /* draw left face */
   glNormal3f( -1.0, 0.0, 0.0 );

   glBegin( GL_QUAD_STRIP );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red );
      glVertex3f( -size, -size,  size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue );
      glVertex3f( -size,  size,  size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green );
      glVertex3f( -size, -size, -size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white );
      glVertex3f( -size,  size, -size );
   glEnd();

   /* draw right face */
   glNormal3f( 1.0, 0.0, 0.0 );

   glBegin( GL_QUAD_STRIP );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green );
      glVertex3f( size, -size,  size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue );
      glVertex3f( size, -size, -size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white );
      glVertex3f( size,  size,  size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red );
      glVertex3f( size,  size, -size );
   glEnd();

   /* draw back face */
   glNormal3f( 0.0, 0.0, -1.0 );

   glBegin( GL_QUAD_STRIP );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue );
      glVertex3f(  size, -size, -size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green );
      glVertex3f( -size, -size, -size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red );
      glVertex3f(  size,  size, -size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white );
      glVertex3f( -size,  size, -size );
   glEnd();

   /* draw top face */
   glNormal3f( 0.0, 1.0, 0.0 );

   glBegin( GL_QUAD_STRIP );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue );
      glVertex3f( -size,  size,  size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white );
      glVertex3f(  size,  size,  size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white );
      glVertex3f( -size,  size, -size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red );
      glVertex3f(  size,  size, -size );
   glEnd();

   /* draw bottom face */
   glNormal3f( 0.0, -1.0, 0.0 );

   glBegin( GL_QUAD_STRIP );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green );
      glVertex3f( -size, -size, -size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue );
      glVertex3f(  size, -size, -size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red );
      glVertex3f( -size, -size,  size );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green );
      glVertex3f(  size, -size,  size );
   glEnd();

}


static GLfloat view_rotx=20.0, view_roty=30.0, view_rotz=0.0, view_distance = -0.0;
static GLint cube1;
static int	view_direction = -1;

static void draw( void )
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glPushMatrix();
   /* Translate scene */
   glTranslatef( 0.0, 0.0, view_distance );
   /* Rotate scene */
   glRotatef( view_rotx, 1.0, 0.0, 0.0 );
   glRotatef( view_roty, 0.0, 1.0, 0.0 );
   glRotatef( view_rotz, 0.0, 0.0, 1.0 );

   glPushMatrix();
   glCallList(cube1);
   glPopMatrix();

   glPopMatrix();

   glFlush();
}


static void init( void )
{
   static GLfloat pos1[4] = {5.0, 5.0, -40.0, 1.0 };
   static GLfloat pos2[4] = {-5.0, -5.0, -40.0, 1.0 };
   static GLfloat dir1[4] = {-1.0, -1.0, -0.6, 1.0 };
   static GLfloat dir2[4] = { 1.0,  1.0,  0.6, 1.0 };
   static GLfloat specular1[4] = { 1.0, 1.0, 1.0, 1.0 };
   static GLfloat diffuse1[4] = { 0.5, 0.5, 0.5, 0.5 };
   static GLfloat ambient1[4] = { 0.0, 0.0, 0.0, 1.0 };
/*   static GLfloat lmodel_ambient[]  =   { 0.5, 0.5, 0.5, 1.0 }; */
/*   GLfloat w = (float) WIDTH / (float) HEIGHT;
   GLfloat h = 1.0;*/

/*
   glLightfv( GL_LIGHT0, GL_AMBIENT, ambient1 );
   glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse1 );
   glLightfv( GL_LIGHT0, GL_SPECULAR, specular1 );
*/
   glLightfv( GL_LIGHT0, GL_POSITION, pos1 );
/*   glLightfv( GL_LIGHT0, GL_SPOT_DIRECTION, dir1 );*/
   glLightfv( GL_LIGHT1, GL_POSITION, pos2 );
/*   glLightfv( GL_LIGHT1, GL_SPOT_DIRECTION, dir2 );*/

/*   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient); */
/*
   glEnable( GL_CULL_FACE );
*/
   glEnable( GL_LIGHT0 );
   glEnable( GL_LIGHT1 );
   glEnable( GL_LIGHTING );

/*
   glEnable(GL_BLEND);
*/
   glEnable(GL_AUTO_NORMAL);
   glEnable( GL_DEPTH_TEST );

   /* make the gears */
   cube1 = glGenLists(1);
   glNewList(cube1, GL_COMPILE);
   cube(2.0);
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

/* Translate the scene away from the camera */
   glTranslatef( 0.0, 0.0, -40.0 );

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
	strcpy((char *)&window_block[19], "Cube");

	strcpy((char *)menu_block, "Cube");
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
   ctx = ROSMesaCreateContext( ROSMESA_RGBA, 32, GL_FALSE );
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
					view_rotz += 2.0;
					if (view_rotz >= 360.0) view_rotz = view_rotz - 360.0;
/*
					if (view_direction == -1)
					{
						view_distance = view_distance - 0.5;
						if (view_distance <= -20.0) view_direction = 1;
					}
					else
					{
						view_distance = view_distance + 0.5;
						if (view_distance > -0.1) view_direction = -1;
					}
*/
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
                	case 6:		/* Button */
                			if (message[2] == 2)
                			{
                				/* Open menu */

                                        	/* Get pointer position */
                                        	regs.r[1] = (unsigned int)&message;
						_kernel_swi(Wimp_GetPointerInfo, &regs, &regs);
                                        	regs.r[1] = (unsigned int)&menu_block;
                                        	regs.r[2] = message[0] - 64;
                                        	regs.r[3] = message[1];
                                        	_kernel_swi(Wimp_CreateMenu, &regs, &regs);
                                        }
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

   end();
   return 0;
}
