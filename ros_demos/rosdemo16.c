/* rosdemo_w.c */

/* Demo of RISC OS Mesa rendering */

/*
 * See Mesa/include/GL/rosmesa.h for documentation of the ROSMesa functions.
 *
 * If you want to render BIG images you'll probably have to increase
 * MAX_WIDTH and MAX_HEIGHT in src/config.h.
 *
 * This program is in the public domain.
 *
 * Brian Paul
 */

/*
	Debug lines put in by David Boddie to trace the progress
	of the running program (Sun 09th November 1997).
	New program (Fri 14th November 1997).
*/

#include <stdio.h>
#include <stdlib.h>
#include "GL/rosmesa.h"
#include "glaux.h"
#include <sys/swis.h>
#include <kernel.h>


#define WIDTH 320
#define HEIGHT 256



static void render_image( void )
{
   GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
   GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
   GLfloat red_mat[]   = { 1.0, 0.2, 0.2, 1.0 };
   GLfloat green_mat[] = { 0.2, 1.0, 0.2, 1.0 };
   GLfloat blue_mat[]  = { 0.2, 0.2, 1.0, 1.0 };

   /* Debug statement */
/*
   fprintf(stderr, "In render function.\n");
*/
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   /* Debug statement */
/*
   fprintf(stderr, "Lighting set up.\n");
*/
   glDisable(GL_DITHER);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);

   /* Debug statement */
/*
   fprintf(stderr, "Lighting enabled.\n");
*/
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-2.5, 2.5, -2.5, 2.5, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);

   /* Debug statement */
/*
   fprintf(stderr, "Matrix mode set up.\n");
*/
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   /* Debug statement */
/*
   fprintf(stderr, "Cleared.\n");
*/
   glPushMatrix();
   glRotatef(20.0, 1.0, 0.0, 0.0);

   /* Debug statement */
/*
   fprintf(stderr, "Rotated.\n");
*/
   glPushMatrix();
   glTranslatef(-0.75, 0.5, 0.0);
   glRotatef(90.0, 1.0, 0.0, 0.0);
   glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red_mat );

   /* Debug statement */
/*
   fprintf(stderr, "Before torus created.\n");
*/

   auxSolidTorus(0.275, 0.85);
   glPopMatrix();

   /* Debug statement */
/*
   fprintf(stderr, "Torus created.\n");
*/

   glPushMatrix();
   glTranslatef(-0.75, -0.5, 0.0);
   glRotatef(270.0, 1.0, 0.0, 0.0);
   glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green_mat );
   auxSolidCone(1.0, 2.0);
   glPopMatrix();

   /* Debug statement */
/*
   fprintf(stderr, "Cone created.\n");
*/

   glPushMatrix();
   glTranslatef(0.75, 0.0, -1.0);
   glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue_mat );
   auxSolidSphere(1.0);
   glPopMatrix();


   /* Debug statement */
/*
   fprintf(stderr, "Sphere created.\n");
*/
   glPopMatrix();
}



int main()
{
   ROSMesaContext ctx;
   void *buffer;
   int temp[3];
   _kernel_swi_regs regs;
   int	mode_block[6] = {1, 320, 256, 4, -1, -1};


   /* Create an RGB-mode context (16 bits per pixel) */
   ctx = ROSMesaCreateContext( ROSMESA_RGB, 16, GL_FALSE );
   if (ctx==NULL)
   {
           printf("Couldn't create context.\n");
           return 0;
   }

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


/*
   fprintf(stdout, "&%x\n", (unsigned int)temp[2]);
   fprintf(stdout, "buffer = &%x\n", (unsigned int)buffer);
*/
   /* Bind the buffer to the context and make it current */
   ROSMesaMakeCurrent( ctx, buffer, NULL, GL_UNSIGNED_BYTE, WIDTH, HEIGHT );
   /* Debug statement */
/*
   fprintf(stderr, "Current context set.\n");
*/
   render_image();

/* Save the image */
/*
   regs.r[0] = 10;
   *filename = "$.Picture";
   printf("Writing %s...\n", *filename);

   regs.r[1] = (int) filename;
   regs.r[2] = 0xffd;
   regs.r[4] = (int) buffer;
   regs.r[5] = (int) buffer + (WIDTH * HEIGHT * 4);
   _kernel_swi(OS_File, &regs, &regs);
*/

   /* destroy the context */
   ROSMesaDestroyContext( ctx );

   return 0;
}
