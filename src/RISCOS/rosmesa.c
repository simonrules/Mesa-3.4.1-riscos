/*
 * Mesa 3-D graphics library
 * Version:  3.4.1
 * Copyright (C) 1995-2001  Brian Paul
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * $Log: rosmesa.c $
 *
 * Version: 0.37a1
 * Updated: Mon 04th June 2001
 * by:	David Boddie
 *
 * Implemented some of the 16 bpp functions in assembler.
 *
 * Version: 0.36b6
 * Updated: Fri 04th May 2001
 * by:	 David Boddie
 *
 * Modified src/RISCOS/rosmesa.c and include/GL/rosmesa.h to conform to the
 * driver data structures used in Mesa 3.3 onwards. Updated the Makefile to
 * deal with the new distribution.
 *
 * Version: 0.36b5
 * Updated: Tue 14th November 2000
 * by:	 David Boddie
 *
 * Updated the documentation.
 *
 * Version: 0.36b4
 * Updated: Sat 26th August 2000
 * by:	 David Boddie
 * 
 * Updated the Makefile for the GLU distribution.
 *
 * Version: 0.36b3
 * Updated: Thu 20th July 2000
 * by:	 David Boddie
 * 
 * Uncommented the line
 * 
 *    (void) pv;
 * 
 * in the smooth_rgba_z_triangle function, hopefully stopping strange colour
 * problems with the teapot16 and teapot32 demonstrations.
 * 
 * Version: 0.36b2
 * Updated: Sat 29th April 2000
 * by:	 David Boddie
 * 
 * Updated this documentation since the latest release version of Mesa (3.2)
 * appears to work.
 * 
 * Version: 0.35b release for Mesa 3.1b3
 * Updated: Sun 26th September 1999
 * by:	 David Boddie
 * 
 * Changed Makefiles to work with the distribution of the new beta version.
 * (More files were added and the src directory structure was changed slightly.)
 * 
 * Version: 0.35b
 * Updated: Tue 01st June 1999
 * by:      David Boddie
 *
 * Updated the line and triangle drawing functions to cope with the new data
 * structures for the vertex buffer. Colours are now referred to using the
 * ColourPtr member. Transformation appears to be routed through functions in
 * the X86 directory. The ARM transformation functions may have to be rewritten.
 *
 * Version: 0.34b
 * Updated: Tue 20th April 1999
 * by:      David Boddie
 *
 * Found some problems with using the && operator instead of the bitwise AND
 * (&) operator in bit operations. 8 bpp mode should now work. 32 bpp problems
 * with the texcyl demo appear to be with the 0.33 versions and earlier rather
 * than with the 0.34a version. i.e. the demo appears as it does under Linux
 * whereas previously it appeared differently.
 *
 * Version: 0.34a
 * Updated: Tue 20th April 1999
 * by:      David Boddie
 *
 * Changed the available formats to just ROSMESA_RGB and ROSMESA_RGBA. An
 * additional parameter must now be passed to ROSMesaCreateContext containing
 * the required bits per pixel for that mode. ROSMESA_RGB supports 8 and 16 bpp
 * while ROSMESA_RGBA supports 32 bpp.
 *
 * Now use ROSMesaCreateContext( GLenum format, GLuint bpp, GLboolean doublebuffer )
 * to create a rendering context.
 *
 * Version: 0.33
 * Updated: Mon 09th November 1998
 * by:      David Boddie
 *
 * 32 bits per pixel functions now call assembler functions to read and write
 * pixels to the buffer. Non-dithered 16 bits per pixel functions have been
 * optimised to use register shifts rather than multiplication and division.
 * Dithering still needs to be sorted out for this mode then the functions
 * can be rewritten in assembler.
 *
 * Version: 0.32
 * Updated: Sat 20th June 1998
 * by:      David Boddie
 *
 * Added support for dithering via the GL_DITHER switch. This is changed by the
 * application through the glEnable ans glDisable commands (it is initially
 * enabled). When such an action occurs, the function pointed to by the context
 * structure ctx->Driver.Dither is called. This is called dither in ROSMesa and
 * it sets the ROSMesa driver flag ctx->DriverCtx->dither flag and calls the
 * rosmesa_setup_DD_pointers function to reset the plotting function pointers
 * for that context.
 *
 * Version: 0.31
 * Updated: Thu 18th June 1998
 * by:      David Boddie
 *
 * Added support for double buffering using the ROSMesaSwapBuffers function.
 * To do this, I had to add the context variables, buffer1, buffer2, and
 * buffer_mode.
 * The following function have now changed as a result:
 *
 * ROSMesaCreateContext - now takes a GLboolean argument which specifies
 *                        whether double buffering is used.
 * ROSMesaMakeCurrent   - takes an extra buffer argument which specifies
 *                        where the second buffer is stored.
 *
 * Version: 0.3
 * Updated: Tue 12th May 1998
 *          Wed 13th May 1998
 * by:      David Boddie
 *
 * Changed driver with hacks to implement scaling, as the scaling factors
 * have apparently been removed from the gl_create_visual call. This is,
 * I hope, a temporary measure.
 *
 * Version: 0.2
 * Updated: Sun 08th February 1998
 *          Tue 10th February 1998
 * by:      David Boddie
 *
 * Tidied documentation within relevant files. Works with Mesa 2.6 beta.
 *
 * Version: 0.1
 * Updated: Thu 22nd January 1998
 * by:      David Boddie
 *
 * Now using Mesa version 2.5 instead of version 2.4.
 * Added 16 bpp support for some functions, and rudimentary 8 bpp support.
 *
 * Version: 0.01
 * Created: Mon 10th November 1997
 * Creator: David Boddie
 * Initial revision.
 *
 * Derived from the off-screen renderer, "osmesa", and "svgamesa".
 * Very little added to the material on which it is based.
 */

/*
 * Acorn RISC OS Mesa renderer - renders to memory
 * Predominantly based on Brian Paul's off-screen renderer with modifications
 * to enable writing to RISC OS screen modes.
 *
 * Note that there is much in this file that has simply been adjusted to refer
 * to "rosmesa" rather than "osmesa". I realise that this is rather cheeky, but
 * I have found that many of the comments in particular are useful reading.
 *
 * If anyone feels that they would like to help with (or indeed take over) this
 * porting effort, or has useful advice as a C or OpenGL guru, then by all means
 * contact me (David Boddie) at: david@boddie.net
 *
 */

#ifdef DEBUG
#include <stdio.h>
#endif
#include "glheader.h"
#include "GL/rosmesa.h"
#include "context.h"
#include "depth.h"
#include "mem.h"
#include "matrix.h"
#include "types.h"
#include "vb.h"
#include "extensions.h"

#include "colours.c"
#include "ros_asm.h"

struct rosmesa_context {
   GLcontext gl_ctx;			/* The core GL/Mesa context */
   GLvisual *gl_visual;			/* Describes the buffers */
   GLframebuffer *gl_buffer;		/* Depth, stencil, accum, etc buffers */
   GLenum format;			/* either GL_RGBA or GL_RGB (or, in the future, GL_COLOR_INDEX) */

   void *buffer;			/* the context's address of the image buffer */
   void *buffer1;			/* the first image buffer - may be the address of the screen */
   void *buffer2;			/* the second image buffer */
   GLenum buffer_mode;			/* the current buffer being rendered to */

   GLint width, height;			/* size of image buffer */
   GLuint pixel;			/* current color index or RGBA pixel value */
   GLuint clearpixel;			/* pixel for clearing the color buffer */
   GLint red_index, green_index, blue_index;
   GLint red_shift, green_shift, blue_shift, alpha_shift;
/*   GLint red_scale, green_scale, blue_scale, alpha_scale;*/	/* current RGB(A) colour */
   GLuint bpp;				/* bits per pixel (8, 16, 32) */
   GLboolean yup;		/* TRUE  -> Y increases upward */
				/* FALSE -> Y increases downward */
   GLboolean dither;			/* dithering flag */
};

/*#ifdef THREADS
   * A context handle for each thread *
   * TODO: an array/table of contexts indexed by thread IDs *
#else
   * One current context for address space, all threads *
   static ROSMesaContext Current = NULL;
#endif*/

/* A forward declaration: */
static void rosmesa_update_state( GLcontext *ctx );

/*	static ROSMesaContext ROSMesa = NULL;	*/		/* the current context */

/*
 * Convert Mesa window Y coordinate to buffer Y coordinate:
 */
#define FLIP(Y)  (rosmesa->height-(Y)-1)

/*
   ---- Miscellaneous functions ----------------------------------------
*/

/* Could be useful for screen rendering with OS_VduVariables

static void get_buffer_size( GLcontext *ctx, GLuint *width, GLuint *height )
{
   *width = SVGAMesa->width = vga_getxdim();
   *height = SVGAMesa->height = vga_getydim();
}

*/

/*
   ---- Public Functions -----------------------------------------------
*/


/*
	Create a new RISC OS/Mesa context and return a handle to it
	or returns NULL if the parameters were not sensible.

	Notes:

	The user specifies a display format (ROSMESA_RGB, ROSMESA_RGBA)
	and a colour depth which will write to an area of memory which is
	presumably either a screen bank or sprite of the correct colour
	depth. The renderer does not know which type of memory is being
	written to, so it is possible to write to the screen in a mode
	with an incorrect colour depth. It is up to the user to ensure
	that this does not happen.

	Since the other renderers which render directly to the screen may
	assume that these properties of the image do not change	while
	the context is active, this is not true in RISC OS. Therefore it
	is probably a good idea not to do direct screen rendering at the
	moment, but later it could be possible to change the context
	properties on a screen mode change for direct screen writes.
*/

ROSMesaContext GLAPIENTRY ROSMesaCreateContext( GLenum format, GLuint bpp, GLboolean doublebuffer )
{
	ROSMesaContext ctx;
	GLboolean rgb_flag;
	GLboolean alpha_flag;
	GLfloat red_max = 255.0, green_max = 255.0, blue_max = 255.0, alpha_max = 255.0;
	GLint red_shift, green_shift, blue_shift, alpha_shift;
	GLint red_index = 0, green_index = 0, blue_index = 0;
	GLint red_bits, green_bits, blue_bits, alpha_bits;
	GLint index_bits;
/*	GLint bpp;*/						/* depth of screen image (bits)
								   used to distinguish between
								   RGB formats */

/* Check the number of bits per pixel specified is sensible (8,16,32) */

	switch (format)
	{
		/* Leave out < 8 bpp modes */

		case ROSMESA_RGB:
			switch (bpp)
			{
				case 8:
					red_max = green_max = blue_max = 7.0F;
					alpha_max = 255.0F;
/*					red_bits = green_bits = blue_bits = 4;*/
					red_bits = green_bits = blue_bits = 8;
			        						/* 8 bpp colour format for */
										/* RISC OS is strange...   */
										/* Bh|Gh|Gl|Bl|Rl|Th|Tl    */
			        	alpha_bits = 0;
					red_shift = green_shift = blue_shift = alpha_shift = 0; /* not used */
					rgb_flag = GL_TRUE;
					alpha_flag = GL_TRUE;
					index_bits = 0;
					break;

				case 16:	/* 16 bits per pixel */
					red_max = 31.0F;
					green_max = 31.0F;
					blue_max = 31.0F;
					alpha_max = 255.0F;
							/* 16 bpp colour format for				*/
							/* RISC OS is b|g|r					*/
							/*            5|5|5					*/
					red_bits = 8;
					green_bits = 8;
					blue_bits = 8;
			        	alpha_bits = 0;
					red_shift = 0;
					green_shift = 5;
					blue_shift = 10;
					alpha_shift = 15; /* not used */
					rgb_flag = GL_TRUE;
					alpha_flag = GL_TRUE;
					index_bits = 0;
					break;
				default:
/*					printf("Only 8 and 16 bits per pixel depths supported for RGB mode.\n");*/
					return NULL;
			}
			break;
		case ROSMESA_RGBA:
			if (bpp == 32)
			{
				red_max = green_max = blue_max = alpha_max = 255.0F;
		        	red_bits = green_bits = blue_bits = alpha_bits = 8;
				red_shift = 0;
				green_shift = 8;
				blue_shift = 16;
				alpha_shift = 24;
				red_index = 0;
				green_index = 1;
				blue_index = 2;
				rgb_flag = GL_TRUE;
				alpha_flag = GL_FALSE;
				index_bits = 0;
			}
			else
			{
/*				printf("Only 32 bits per pixel depth supported for RGBA mode.\n");*/
				return NULL;
			}
			break;
		default:
/*			printf("Sorry. Only RGB and RGBA modes supported.\n");*/
			return NULL;
        }

/* Allocate memory for context */
	ctx = (ROSMesaContext) CALLOC_STRUCT(rosmesa_context);

	if (ctx) {
		ctx->gl_visual = gl_create_visual( rgb_flag,
						    alpha_flag,    /* software alpha */
	                                            doublebuffer,  /* double buffering flag */
	                                            GL_FALSE,      /* stereo flag */
	                                            DEFAULT_SOFTWARE_DEPTH_BITS,
	                                            STENCIL_BITS,
	                                            ACCUM_BITS,
	                                            index_bits,
	                                            red_bits,
	                                            green_bits,
	                                            blue_bits,
	                                            alpha_bits);

		if (!ctx->gl_visual) {
			FREE(ctx);
			return NULL;
		}

		if (!_mesa_initialize_context(&ctx->gl_ctx,
		                              ctx->gl_visual,
/*		                              sharelist ? &sharelist->gl_ctx
		                                        : (GLcontext *) NULL,*/
		                              NULL,
		                              (void *) ctx, GL_TRUE )) {
		   _mesa_destroy_visual( ctx->gl_visual );
		   FREE(ctx);
		   return NULL;
		}
		gl_extensions_enable(&(ctx->gl_ctx),"GL_HP_occlusion_test");
		gl_extensions_enable(&(ctx->gl_ctx), "GL_ARB_texture_cube_map");
		gl_extensions_enable(&(ctx->gl_ctx), "GL_EXT_texture_env_combine");
		gl_extensions_enable(&(ctx->gl_ctx), "GL_EXT_texture_env_dot3");

		ctx->gl_buffer = gl_create_framebuffer( ctx->gl_visual,
	                                           ctx->gl_visual->DepthBits > 0,
	                                           ctx->gl_visual->StencilBits > 0,
	                                           ctx->gl_visual->AccumRedBits > 0,
	                                           ctx->gl_visual->AlphaBits > 0 );

		if (!ctx->gl_buffer) {
		   gl_destroy_visual( ctx->gl_visual );
		   gl_free_context_data( &ctx->gl_ctx );
		   FREE(ctx);
		   return NULL;
		}
		ctx->format = format;
		ctx->buffer = NULL;
		ctx->buffer1 = NULL;
		ctx->buffer2 = NULL;
		ctx->buffer_mode = GL_FRONT;
		ctx->width = 0;
		ctx->height = 0;
		ctx->bpp = bpp;
		ctx->pixel = 0;
		ctx->clearpixel = 0;
	/*	ctx->userRowLength = 0; Could be useful for window drawing... */
	/*	ctx->rowlength = 0;     Could be useful for window drawing... */
		ctx->yup = GL_FALSE;
		ctx->red_shift = red_shift;
		ctx->green_shift = green_shift;
		ctx->blue_shift = blue_shift;
		ctx->alpha_shift = alpha_shift;
		ctx->red_index = red_index;
		ctx->green_index = green_index;
		ctx->blue_index = blue_index;
	/*
		ctx->red_scale = red_max/255.0F;
		ctx->green_scale = green_max/255.0F;
		ctx->blue_scale = blue_max/255.0F;
		ctx->alpha_scale = alpha_max/255.0F;
	*/
		ctx->dither = GL_TRUE;	/* Dithering on by default */
	
	/*
		ctx->gl_ctx->Pixel.RedScale = red_max/255.0F;
		ctx->gl_ctx->Pixel.GreenScale = green_max/255.0F;
		ctx->gl_ctx->Pixel.BlueScale = blue_max/255.0F;
		ctx->gl_ctx->Pixel.AlphaScale = alpha_max/255.0F;
	*/
	}
	return ctx;
}



/*
 * Destroy a RISC OS/Mesa rendering context.
 *
 * Input:  ctx - the context to destroy
 */
void GLAPIENTRY ROSMesaDestroyContext( ROSMesaContext ctx )
{
   if (ctx) {
      gl_destroy_visual( ctx->gl_visual );
      gl_destroy_framebuffer( ctx->gl_buffer );
      gl_free_context_data( &ctx->gl_ctx );
      FREE( ctx );
   }
}


/*
 * Bind an ROSMesaContext to an image buffer.  The image buffer is just a
 * block of memory which the client provides.  Its size must be at least
 * as large as width*height*sizeof(type).  Its address should be a multiple
 * of 4 if using RGBA mode.
 *
 * Image data is stored in the order of glDrawPixels:  row-major order
 * with the lower-left image pixel stored in the first array position
 * (ie. bottom-to-top).
 *
 * Since the only type initially supported is GL_UNSIGNED_BYTE, if the
 * context is in RGBA mode, each pixel will be stored as a 4-byte RGBA
 * value.  If the context is in color indexed mode, each pixel will be
 * stored as a 1-byte value.
 *
 * If the context's viewport hasn't been initialized yet, it will now be
 * initialized to (0,0,width,height).
 *
 * Input:  ctx - the rendering context
 *         buffer1 - the image buffer memory
 *         buffer2 - a second image buffer (if double buffering is enabled)
 *         type - data type for pixel components, only GL_UNSIGNED_BYTE
 *                supported now
 *         width, height - size of image buffer in pixels, at least 1
 * Return:  GL_TRUE if success, GL_FALSE if error because of invalid ctx,
 *          invalid buffer address, type!=GL_UNSIGNED_BYTE, width<1, height<1,
 *          width>internal limit or height>internal limit.
 */
GLboolean GLAPIENTRY ROSMesaMakeCurrent( ROSMesaContext ctx, void *buffer1, void *buffer2, GLenum type,
                             GLsizei width, GLsizei height )
{
   if (!ctx || !buffer1 || type!=GL_UNSIGNED_BYTE
       || width<1 || height<1 || width>MAX_WIDTH || height>MAX_HEIGHT) {
      return GL_FALSE;
   }

   rosmesa_update_state( &ctx->gl_ctx );

   /* Fail if buffer 2 is undefined and double buffering is enabled. <- Needs looking at. */
   if ((ctx->gl_visual->DBflag == GL_TRUE) && (!buffer2))
   {
   	return GL_FALSE;
   }

   gl_make_current( &ctx->gl_ctx, ctx->gl_buffer );

/* If at this point, the user has not specified the width and height of the viewport, then
   the screen's properties should be used. At the moment, nothing is done. */

/* Check the width and height parameters */
	if ((width<=0) || (height<=0))
	{
/*		printf("Negative or zero values of width or height are not allowed.\n");*/
	        return GL_FALSE;
	}

   ctx->buffer = buffer1;	/* Initially, the image buffer points to the first buffer */
   ctx->buffer1 = buffer1;
   ctx->buffer2 = buffer2;
   ctx->buffer_mode = GL_FRONT;
   ctx->width = width;
   ctx->height = height;

   /* init viewport */
   if (ctx->gl_ctx.Viewport.Width==0) {
      /* initialize viewport and scissor box to buffer size */
      _mesa_Viewport( 0, 0, width, height );
      ctx->gl_ctx.Scissor.Width = width;
      ctx->gl_ctx.Scissor.Height = height;
   }

   return GL_TRUE;
}




ROSMesaContext GLAPIENTRY ROSMesaGetCurrentContext( void )
{
   GLcontext *ctx = gl_get_current_context();
   if (ctx)
      return (ROSMesaContext) ctx;
   else
      return NULL;
}

/*
 * Swap front/back buffers for current context if double buffered.
 */
/*
void SVGAMesaSwapBuffers( void )
{
   if (SVGAMesa->gl_vis->DBflag) {
      vga_flip();
   }
}
*/

void GLAPIENTRY ROSMesaPixelStore( GLint pname, GLint value )
{
   ROSMesaContext ctx = ROSMesaGetCurrentContext();

   switch (pname) {
      case ROSMESA_ROW_LENGTH:
	break;
/*
         if (value<0) {
            gl_error( &ctx->gl_ctx, GL_INVALID_VALUE,
                      "ROSMesaPixelStore(value)" );
            return;
         }
         ctx->userRowLength = value;
         ctx->rowlength = value;
         break;
*/
      case ROSMESA_Y_UP:
         ctx->yup = value ? GL_TRUE : GL_FALSE;
         break;
      default:
         gl_error( &ctx->gl_ctx, GL_INVALID_ENUM, "ROSMesaPixelStore(pname)" );
         return;
   }

/*
   compute_row_addresses( ctx );
*/
}


void GLAPIENTRY ROSMesaGetIntegerv( GLint pname, GLint *value )
{
   ROSMesaContext ctx = ROSMesaGetCurrentContext();

   switch (pname) {
      case ROSMESA_WIDTH:
         *value = ctx->width;
         return;
      case ROSMESA_HEIGHT:
         *value = ctx->height;
         return;
      case ROSMESA_FORMAT:
         *value = ctx->format;
         return;
      case ROSMESA_TYPE:
         *value = GL_UNSIGNED_BYTE;
         return;
/*
      case ROSMESA_ROW_LENGTH:
         *value = ctx->rowlength;
         return;
*/
      case ROSMESA_Y_UP:
         *value = ctx->yup;
         return;
      default:
         gl_error( &ctx->gl_ctx, GL_INVALID_ENUM, "ROSMesaGetIntergerv(pname)" );
         return;
   }
}



/*
 * Return the depth buffer associated with an OSMesa context.
 * Input:  c - the OSMesa context
 * Output:  width, height - size of buffer in pixels
 *          bytesPerValue - bytes per depth value (2 or 4)
 *          buffer - pointer to depth buffer values
 * Return:  GL_TRUE or GL_FALSE to indicate success or failure.
 */
GLboolean GLAPIENTRY ROSMesaGetDepthBuffer( ROSMesaContext c, GLint *width, GLint *height,
                                GLint *bytesPerValue, void **buffer )
{
   if ((!c->gl_buffer) || (!c->gl_buffer->DepthBuffer)) {
      *width = 0;
      *height = 0;
      *bytesPerValue = 0;
      *buffer = 0;
      return GL_FALSE;
   }
   else {
      *width = c->gl_buffer->Width;
      *height = c->gl_buffer->Height;
      *bytesPerValue = sizeof(GLdepth);
      *buffer = c->gl_buffer->DepthBuffer;
      return GL_TRUE;
   }
}

/*
 * Return the color buffer associated with an OSMesa context.
 * Input:  c - the OSMesa context
 * Output:  width, height - size of buffer in pixels
 *          format - the pixel format (OSMESA_FORMAT)
 *          buffer - pointer to color buffer values
 * Return:  GL_TRUE or GL_FALSE to indicate success or failure.
 */
GLboolean GLAPIENTRY
ROSMesaGetColorBuffer( ROSMesaContext c, GLint *width,
                      GLint *height, GLint *format, void **buffer )
{
   if (!c->buffer) {
      *width = 0;
      *height = 0;
      *format = 0;
      *buffer = 0;
      return GL_FALSE;
   }
   else {
      *width = c->width;
      *height = c->height;
      *format = c->format;
      *buffer = c->buffer;
      return GL_TRUE;
   }
}


/* Added Tue 16th June 1998 to allow some form of bank switching */
/* Modified Wed 17th June 1998 to tidy it up - note that calling application
   still has to do something to change the displayed buffer as this function
   only changes wher Mesa writes to. */

void ROSMesaSwapBuffers( ROSMesaContext ctx )
{
	GLenum	mode;

	/* Change the current buffer mode */
	mode = ctx->buffer_mode;

	/* Change the buffer to the one referred to by the variable, mode
	   i.e. GL_FRONT refers to buffer1, GL_BACK refers to buffer2. */

	if (mode == GL_FRONT)
	{
		ctx->buffer_mode = GL_BACK;
		ctx->buffer = ctx->buffer2;
	}
	else
	{
		ctx->buffer_mode = GL_FRONT;
		ctx->buffer = ctx->buffer1;
	}
}


/**********************************************************************/
/*** Device Driver Functions                                        ***/
/**********************************************************************/


/*
 * Useful macros:
 */
#define PACK_RGBA(R,G,B,A)  (  ((R) << rosmesa->red_shift) \
                             | ((G) << rosmesa->green_shift) \
                             | ((B) << rosmesa->blue_shift) \
                             | ((A) << rosmesa->alpha_shift) )

#define PACK_RGB(R,G,B)  (  ((R) << rosmesa->red_shift) \
                             | ((G) << rosmesa->green_shift) \
                             | ((B) << rosmesa->blue_shift) )

#define PACK_RGBA2(R,G,B,A)  (  ((R) << rshift) \
                              | ((G) << gshift) \
                              | ((B) << bshift) \
                              | ((A) << ashift) )

#define UNPACK_RED(P)      (((P) >> rosmesa->red_shift) & 0xff)
#define UNPACK_GREEN(P)    (((P) >> rosmesa->green_shift) & 0xff)
#define UNPACK_BLUE(P)     (((P) >> rosmesa->blue_shift) & 0xff)
#define UNPACK_ALPHA(P)    (((P) >> rosmesa->alpha_shift) & 0xff)

/* Not used anymore
#define UNPACK_RED8(P)    ((((P) & 0x10) >> 3) | ((P) & 0x4) >> 2)
#define UNPACK_GREEN8(P)  ((((P) & 0x40) >> 5) | ((P) & 0x20) >> 5)
#define UNPACK_BLUE8(P)   ((((P) & 0x80) >> 6) | ((P) & 0x8) >> 3)
#define UNPACK_ALPHA8(P)  ((P) & 0x3)
*/

#define PIXELADDR1(X,Y)  ((GLubyte *) (rosmesa->buffer + (FLIP(Y) * rosmesa->width) + (X)))
#define PIXELADDR2(X,Y)  ((GLubyte *) (rosmesa->buffer + (FLIP(Y) * rosmesa->width * 2) + ((X)*2)))
#define PIXELADDR3(X,Y)  ((GLubyte *) (rosmesa->buffer + (FLIP(Y) * rosmesa->width * 3) + ((X)*3)))
#define PIXELADDR4(X,Y)  ((GLuint *)  (rosmesa->buffer + (FLIP(Y) * rosmesa->width * 4) + ((X)*4))) /* Note integers are word-sized */




static GLboolean set_draw_buffer( GLcontext *ctx, GLenum mode )
{
   if (mode==GL_FRONT_LEFT) {
      return GL_TRUE;
   }
   else {
      return GL_FALSE;
   }
}


static void set_read_buffer( GLcontext *ctx, GLframebuffer *buffer, GLenum mode )
{
   /* separate read buffer not supported */
   ASSERT(buffer == ctx->DrawBuffer);
   ASSERT(mode == GL_FRONT_LEFT);
}


static void clear_index( GLcontext *ctx, GLuint index )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   rosmesa->clearpixel = index;
}



static void clear_color( GLcontext *ctx,
                         GLubyte r, GLubyte g, GLubyte b, GLubyte a )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   switch (rosmesa->format)
   {
	case ROSMESA_RGBA:
		rosmesa->clearpixel = PACK_RGBA( r, g, b, a );
		break;
	case ROSMESA_RGB:
                if (rosmesa->bpp == 16)
		{
				if (rosmesa->dither)
				{
					rosmesa->clearpixel = ((GLuint)ceil( ((r * 31)/255) ) & 0x1f)
								| (((GLuint)ceil( ((g * 31)/255) ) & 0x1f) << 5)
								| (((GLuint)ceil( ((b * 31)/255) ) & 0x1f) << 10)
								| (
									(((GLuint)floor( ((r * 31)/255) ) & 0x1f)
									| (((GLuint)floor(((g * 31)/255)) & 0x1f) << 5)
									| (((GLuint)floor( ((b * 31)/255) ) & 0x1f) << 10) )
								<< 16);
				}
				else
				{
					rosmesa->clearpixel = (((r * 31)/255) & 0x1f) | ((((g * 31)/255) & 0x1f) << 5) | ((((b * 31)/255) & 0x1f) << 10);
				}
				/* More generally, rshift = 0, gshift = 5, bshift = 10 */
				/* Changed green quantity to 0x1f, and blue shift to 10 */
		}
		if (rosmesa->bpp == 8)
		{
				if (rosmesa->dither)
					{
						rosmesa->clearpixel = ( rosmesa_colourtable[(r>>4)][(g>>4)][(b>>4)][0]
									| ( rosmesa_colourtable[(r>>4)][(g>>4)][(b>>4)][1] << 8) );
					}
					else
					{
						rosmesa->clearpixel = rosmesa_colourtable[(r>>4)][(g>>4)][(b>>4)][0];
					}
		}
		break;
	default:
		break;
   }
}



static GLbitfield clear( GLcontext *ctx, GLbitfield mask,
                   GLboolean all, GLint x, GLint y, GLint width, GLint height )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint val = rosmesa->clearpixel;

   if (mask & GL_COLOR_BUFFER_BIT)
   {
	switch (rosmesa->format)
	{
		case ROSMESA_RGB:
			switch (rosmesa->bpp)
			{
				case 8:
					if (all)
 					{
						GLuint  i, n;
						GLubyte *ptr1 = (GLubyte *) rosmesa->buffer;

						/* Clear whole RGB buffer */
						n = rosmesa->width * rosmesa->height;
						for (i=0;i<n;i++)
						{
							ptr1[0] = (GLubyte)val;
							ptr1 += 1;
						}
					}
					else
					{
						/* Clear part of RGB buffer */
						GLuint i, j;

						for (i=0;i<height;i++)
						{
							GLubyte *ptr1 = PIXELADDR1( x, (y+i) );

							for (j=0;j<width;j++)
							{
								ptr1[0] = (GLubyte)val;
								ptr1 += 1;
							}
						}
					}
					break;
				case 16:
					if (all)
					{
						GLuint  i, n;
						GLubyte *ptr2 = (GLubyte *) rosmesa->buffer;

						/* Clear whole RGB buffer */
						n = rosmesa->width * rosmesa->height; /* *2 */
						for (i=0;i<n;i++)
						{
							ptr2[0] = (GLubyte)(val & 0xff);
							ptr2[1] = (GLubyte)((val & 0xff00) >> 8);
							ptr2 += 2;
						}
					}
					else
					{
						/* Clear part of RGB buffer */
						GLuint i, j;

						for (i=0;i<height;i++)
						{
							GLubyte *ptr2 = PIXELADDR2( x, (y+i) );

							for (j=0;j<width;j++)
							{
								ptr2[0] = (GLubyte)(val & 0xff);
								ptr2[1] = (GLubyte)((val & 0xff00) >> 8);
								ptr2 += 2;
							}
						}
					}
					break;
				default:
					/* No default */
					break;
			}
			break;

		case ROSMESA_RGBA:
			if (all)
			{
				/* Clear whole RGBA buffer */
				GLuint i, n, *ptr4;

				n = rosmesa->width * rosmesa->height; /* Buffer size in _pixels_ */
				ptr4 = (GLuint *) rosmesa->buffer;
				for (i=0;i<n;i++)	/* Number of pixels in buffer */
				{
					*ptr4++ = val; /* Increments in words */
				}
			}
			else
			{
				/* Clear part of RGBA buffer */
				GLuint i, j;

				for (i=0;i<height;i++)
				{
					GLuint *ptr4 = PIXELADDR4( x, (y+i) );
					for (j=0;j<width;j++)
					{
						*ptr4++ = val;
					}
				}
			}
			break;

		default:
			/* No default */
			break;
	} /* End switch */
   }
   return mask & (~GL_COLOR_BUFFER_BIT);
}

static GLbitfield clear_dither( GLcontext *ctx, GLbitfield mask,
                   GLboolean all, GLint x, GLint y, GLint width, GLint height )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;

   if (mask & GL_COLOR_BUFFER_BIT)
   {
	switch (rosmesa->format)
	{
		case ROSMESA_RGB:
			switch (rosmesa->bpp)
			{
				case 8:
					{
						GLuint val1 = (rosmesa->clearpixel & 0x00ff);
						GLuint val2 = ((rosmesa->clearpixel & 0xff00) >> 8);

						if (all)
						{
							GLuint  i, w, h, j;
							GLubyte *ptr2 = (GLubyte *) rosmesa->buffer;

							/* Clear whole RGB buffer */
							h = rosmesa->height;
							w = rosmesa->width /2;	/* /2 comes from plotting two pixels in an 8bpp mode */
							for (j=0; j<h; j++)
							{
								GLuint	val;

								for (i=0; i<w; i++)
								{
						                       ptr2[0] = (GLubyte)val1;
	                					       ptr2[1] = (GLubyte)val2;
						                       ptr2 += 2;
								}
								/* Every row, swap the two values around to avoid vertical striping
								  (this assumes that the width of the buffer is even.) */
								val = val1;
								val1 = val2;
								val2 = val;
							}
						}
						else
						{
							/* Clear part of RGB buffer */
							GLuint i, j;
							/* If the function x*y of the pixel is even then begin to write the pixels as above,
							   otherwise start to write them in opposite order so as to keep the dithering pattern */
							GLuint	val;

							if (((x*y) % 2)==1)
							{
								val = val1;
								val1 = val2;
								val2 = val;
							}

							for (i=0;i<height;i++)
							{
								GLubyte *ptr2 = PIXELADDR1( x, (y+i) );

								if ((width % 2) == 0)
								{
									for (j=0;j<width;j += 2)
									{
										ptr2[0] = (GLubyte)val1;
										ptr2[1] = (GLubyte)val2;
										ptr2 += 2;
									}
								}
								else
								{
									for (j=0;j<width-1;j += 2)
									{
										ptr2[0] = (GLubyte)val1;
										ptr2[1] = (GLubyte)val2;
										ptr2 += 2;
									}
									ptr2[0] = (GLubyte)val1;
								}
								/* Every row, swap the two values around to avoid vertical striping
								  (this assumes that the width of the buffer is even.) */
								val = val1;
								val1 = val2;
								val2 = val;
							}
						}
					}
					break;
				case 16:
					{
						GLuint	val = rosmesa->clearpixel;

						if (all)
						{
							GLuint  i, n;
							GLuint *ptr4 = (GLuint *) rosmesa->buffer;
							/* Clear whole RGB buffer */
							n = rosmesa->width * rosmesa->height;
							for (i=0; i<n; i += 2)
							{
								ptr4[0] = val;
								ptr4 += 1;
							}
						}
						else
						{
							GLuint	val1;
							GLuint	val2;
							GLuint	val;
							/* Clear part of RGB buffer */
							GLuint i, j;

							if (((x*y) % 2) == 0)
							{
								val1 = (rosmesa->clearpixel & 0xffff);
								val2 = ((rosmesa->clearpixel & 0xffff0000) >> 16);
							}
							else
							{
								val2 = (rosmesa->clearpixel & 0xffff);
								val1 = ((rosmesa->clearpixel & 0xffff0000) >> 16);
							}

							for (i=0;i<height;i++)
							{
								GLubyte *ptr2 = PIXELADDR2( x, (y+i) );
													/* use byte rather than int due to uncertainties
													   over behaviour of storing words to non-word
													   addresses */
								if ((width % 2) == 0)
								{
									for (j=0; j<width; j += 2)
									{
										ptr2[0] = (GLubyte)(val1 & 0xff);
										ptr2[1] = (GLubyte)((val1 & 0xff00) >> 8);
										ptr2[2] = (GLubyte)(val2 & 0xff);
										ptr2[3] = (GLubyte)((val2 & 0xff00) >> 8);
										ptr2 += 4;
									}
								}
								else
								{
									for (j=0; j<width-1; j += 2)
									{
										ptr2[0] = (GLubyte)(val1 & 0xff);
										ptr2[1] = (GLubyte)((val1 & 0xff00) >> 8);
										ptr2[2] = (GLubyte)(val2 & 0xff);
										ptr2[3] = (GLubyte)((val2 & 0xff00) >> 8);
										ptr2 += 4;
									}
									ptr2[0] = (GLubyte)(val1 & 0xff);
									ptr2[1] = (GLubyte)((val1 & 0xff00) >> 8);
								}
								/* Every row, swap the two values around to avoid vertical striping
								  (this assumes that the width of the buffer is even.) */
								val = val1;
								val1 = val2;
								val2 = val;
							}
						}
					}
					break;
				default:
					break;
			}
			break;
		case ROSMESA_RGBA:
			if (all)
			{
				/* Clear whole RGBA buffer */
				GLuint i, n, *ptr4;
				GLuint val = rosmesa->clearpixel;

				n = rosmesa->width * rosmesa->height; /* Buffer size in _pixels_ */
				ptr4 = (GLuint *) rosmesa->buffer;
				for (i=0;i<n;i++)	/* Number of pixels in buffer */
				{
					*ptr4++ = val;	/* Increments in words */
				}
			}
			else
			{
				/* Clear part of RGBA buffer */
				GLuint i, j;
				GLuint val = rosmesa->clearpixel;

				for (i=0;i<height;i++)
				{
					GLuint *ptr4 = PIXELADDR4( x, (y+i) );

					for (j=0;j<width;j++)
					{
						*ptr4++ = val;
					}
				}
			}
			break;
		default:
			/* No default */
			break;
	} /* End switch */
   }
   return mask & (~GL_COLOR_BUFFER_BIT);
}



static void set_index( GLcontext *ctx, GLuint index )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   rosmesa->pixel = index;
}



static void set_color( GLcontext *ctx,
                       GLubyte r, GLubyte g, GLubyte b, GLubyte a )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   switch (rosmesa->format)
   {
	case ROSMESA_RGBA:
		rosmesa->pixel = (GLuint) PACK_RGBA( r, g, b, a );
		break;
	case ROSMESA_RGB:
		if (rosmesa->bpp == 16)
		{
				if (rosmesa->dither)
				{
					rosmesa->pixel = ((GLuint)ceil(((r * 31)/255)) & 0x1f)
							| (((GLuint)ceil(((g * 31)/255)) & 0x1f) << 5)
							| (((GLuint)ceil(((b * 31)/255)) & 0x1f) << 10)
							| ( ( ((GLuint)floor(((r * 31)/255)) & 0x1f)
							| (((GLuint)floor(((g * 31)/255)) & 0x1f) << 5)
							| (((GLuint)floor(((b * 31)/255)) & 0x1f) << 10) ) << 16);
				}
				else
				{
					rosmesa->pixel = ((r>>3) & 0x1f) | (((g>>3) & 0x1f) << 5) | (((b>>3) & 0x1f) << 10);
				}
		}
		if (rosmesa->bpp == 8)
		{
				if (rosmesa->dither)
				{
					rosmesa->pixel = (GLuint)( rosmesa_colourtable[(r>>4)][(g>>4)][(b>>4)][0]
								   | ( rosmesa_colourtable[(r>>4)][(g>>4)][(b>>4)][1] << 8) );
				}
				else
				{
					rosmesa->pixel = (GLuint)rosmesa_colourtable[(r>>4)][(g>>4)][(b>>4)][0];
				}
		}
		break;
	default:
		break;
   }
}



static void buffer_size( GLcontext *ctx, GLuint *width, GLuint *height )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   *width = rosmesa->width;
   *height = rosmesa->height;
}


/* Dithering function - enables or disables the dithering */

static void dither(GLcontext *ctx, GLboolean enable)
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;

	if (enable) rosmesa->dither = GL_TRUE; else rosmesa->dither = GL_FALSE;
	rosmesa_update_state(ctx);
}

/**********************************************************************/
/*****        4 byte RGB and 1 byte CI pixel support funcs        *****/
/**********************************************************************/

static void write_rgba_span( const GLcontext *ctx,
                              GLuint n, GLint x, GLint y,
                              const GLubyte rgba[][4], const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint *ptr4 = PIXELADDR4( x, y );
#ifdef DEBUG 
fprintf(stderr, "write_rgba_span\n");
#endif
   if (mask)
   {
	asm_write_rgba_span_masked(n, (GLuint) mask, (GLuint) rgba, (GLuint) ptr4);
   }
   else
   {
	asm_write_rgba_span_unmasked(n, (GLuint) rgba, (GLuint) ptr4);
   }
}

static void write_rgb_span( const GLcontext *ctx,
                              GLuint n, GLint x, GLint y,
                              const GLubyte rgba[][3], const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint *ptr4 = PIXELADDR4( x, y );
#ifdef DEBUG 
fprintf(stderr, "write_rgb_span\n");
#endif
   if (mask)
   {
	asm_write_rgb_span_masked(n, (GLuint) mask, (GLuint) rgba, (GLuint) ptr4);
   }
   else
   {
	asm_write_rgb_span_unmasked(n, (GLuint) rgba, (GLuint) ptr4);
   }
}

static void write_monocolor_span( const GLcontext *ctx,
                                  GLuint n, GLint x, GLint y,
				  const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint *ptr4 = PIXELADDR4(x,y);
#ifdef DEBUG 
fprintf(stderr, "write_monocolor_span\n");
#endif

   asm_write_monocolor_span(n, (GLuint) mask, (GLuint) rosmesa->pixel, (GLuint) ptr4);
}



static void write_rgba_pixels( const GLcontext *ctx,
                                GLuint n, const GLint x[], const GLint y[],
                                const GLubyte rgba[][4], const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint	  parameters[4] = {(GLuint) rgba, (GLuint) mask, (GLuint) (rosmesa->buffer + ((rosmesa->height - 1)*(rosmesa->width * 4))), (GLuint) rosmesa->width};
#ifdef DEBUG 
fprintf(stderr, "write_rgba_pixels\n");
#endif

   asm_write_rgba_pixels(n, (GLuint) x, (GLuint) y, (GLuint) parameters);
}



static void write_monocolor_pixels( const GLcontext *ctx,
                                    GLuint n, const GLint x[], const GLint y[],
				    const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;

   GLuint	  parameters[4] = {(GLuint) rosmesa->pixel, (GLuint) mask, (GLuint) (rosmesa->buffer + ((rosmesa->height - 1)*(rosmesa->width * 4))), (GLuint) rosmesa->width};
#ifdef DEBUG 
fprintf(stderr, "write_monocolor_pixels\n");
#endif

   asm_write_monocolor_pixels(n, (GLuint) x, (GLuint) y, (GLuint) parameters);
}



/* Commented out the index functions, as they are not used.

static void write_index_span( const GLcontext *ctx,
                              GLuint n, GLint x, GLint y, const GLuint index[],
			      const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLubyte *ptr1 = PIXELADDR1(x,y);
   GLuint i;
   for (i=0;i<n;i++,ptr1++) {
      if (mask[i]) {
         *ptr1 = (GLubyte) index[i];
      }
   }
}

static void write_monoindex_span( const GLcontext *ctx,
                                  GLuint n, GLint x, GLint y,
				  const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLubyte *ptr1 = PIXELADDR1(x,y);
   GLuint i;
   for (i=0;i<n;i++,ptr1++) {
      if (mask[i]) {
         *ptr1 = (GLubyte) rosmesa->pixel;
      }
   }
}



static void write_index_pixels( const GLcontext *ctx,
                                GLuint n, const GLint x[], const GLint y[],
			        const GLuint index[], const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint i;
   for (i=0;i<n;i++) {
      if (mask[i]) {
         GLubyte *ptr1 = PIXELADDR1(x[i],y[i]);
         *ptr1 = (GLubyte) index[i];
      }
   }
}



static void write_monoindex_pixels( const GLcontext *ctx,
                                    GLuint n, const GLint x[], const GLint y[],
				    const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint i;
   for (i=0;i<n;i++) {
      if (mask[i]) {
         GLubyte *ptr1 = PIXELADDR1(x[i],y[i]);
         *ptr1 = (GLubyte) rosmesa->pixel;
      }
   }
}



static void read_index_span( const GLcontext *ctx,
                             GLuint n, GLint x, GLint y, GLuint index[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint i;
   GLubyte *ptr1 = PIXELADDR1(x,y);
   for (i=0;i<n;i++,ptr1++) {
      index[i] = (GLuint) *ptr1;
   }
}

static void read_index_pixels( const GLcontext *ctx,
                               GLuint n, const GLint x[], const GLint y[],
			       GLuint index[], const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint i;
   for (i=0;i<n;i++) {
      if (mask[i] ) {
         GLubyte *ptr1 = PIXELADDR1(x[i],y[i]);
         index[i] = (GLuint) *ptr1;
      }
   }
}


*/

static void read_rgba_span( const GLcontext *ctx,
                             GLuint n, GLint x, GLint y,
                             GLubyte rgba[][4] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;

   GLuint *ptr4 = PIXELADDR4(x,y);
#ifdef DEBUG 
fprintf(stderr, "read_rgba_span\n");
#endif

   asm_read_rgba_span(n, (GLuint) rgba, (GLuint) ptr4);
}


static void read_rgba_pixels( const GLcontext *ctx,
                               GLuint n, const GLint x[], const GLint y[],
			       GLubyte rgba[][4], const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint	  parameters[4] = {(GLuint) rgba, (GLuint) mask, (GLuint) (rosmesa->buffer + ((rosmesa->height - 1)*(rosmesa->width * 4))), (GLuint) rosmesa->width};
#ifdef DEBUG 
fprintf(stderr, "read_rgba_pixels\n");
#endif

   asm_read_rgba_pixels(n, (GLuint) x, (GLuint) y, (GLuint) parameters);
}

/**********************************************************************/
/*****                3 byte RGB pixel support funcs              *****/
/**********************************************************************/

/*
static void write_rgba_span3( const GLcontext *ctx,
                              GLuint n, GLint x, GLint y,
                              const GLubyte rgba[][4], const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLubyte *ptr3 = PIXELADDR3( x, y);
   GLuint i;
   GLint rind = rosmesa->red_index;
   GLint gind = rosmesa->green_index;
   GLint bind = rosmesa->blue_index;
   if (mask) {
      for (i=0;i<n;i++,ptr3+=3) {
         if (mask[i]) {
            ptr3[rind] = rgba[i][RCOMP];
            ptr3[gind] = rgba[i][GCOMP];
            ptr3[bind] = rgba[i][BCOMP];
         }
      }
   }
   else {
      for (i=0;i<n;i++,ptr3+=3) {
         ptr3[rind] = rgba[i][RCOMP];
         ptr3[gind] = rgba[i][GCOMP];
         ptr3[bind] = rgba[i][BCOMP];
      }
   }
}

static void write_rgb_span3( const GLcontext *ctx,
                              GLuint n, GLint x, GLint y,
                              const GLubyte rgba[][3], const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLubyte *ptr3 = PIXELADDR3( x, y);
   GLuint i;
   GLint rind = rosmesa->red_index;
   GLint gind = rosmesa->green_index;
   GLint bind = rosmesa->blue_index;
   if (mask) {
      for (i=0;i<n;i++,ptr3+=3) {
         if (mask[i]) {
            ptr3[rind] = rgba[i][RCOMP];
            ptr3[gind] = rgba[i][GCOMP];
            ptr3[bind] = rgba[i][BCOMP];
         }
      }
   }
   else {
      for (i=0;i<n;i++,ptr3+=3) {
         ptr3[rind] = rgba[i][RCOMP];
         ptr3[gind] = rgba[i][GCOMP];
         ptr3[bind] = rgba[i][BCOMP];
      }
   }
}

static void write_monocolor_span3( const GLcontext *ctx,
                                  GLuint n, GLint x, GLint y,
				  const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;

   GLubyte rval = UNPACK_RED(rosmesa->pixel);
   GLubyte gval = UNPACK_GREEN(rosmesa->pixel);
   GLubyte bval = UNPACK_BLUE(rosmesa->pixel);
   GLint   rind = rosmesa->red_index;
   GLint   gind = rosmesa->green_index;
   GLint   bind = rosmesa->blue_index;


   GLubyte *ptr3 = PIXELADDR3( x, y);
   GLuint i;
   for (i=0;i<n;i++,ptr3+=3) {
      if (mask[i]) {
         ptr3[rind] = rval;
         ptr3[gind] = gval;
         ptr3[bind] = bval;
      }
   }
}

static void write_rgba_pixels3( const GLcontext *ctx,
                                GLuint n, const GLint x[], const GLint y[],
                                const GLubyte rgba[][4], const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint i;
   GLint rind = rosmesa->red_index;
   GLint gind = rosmesa->green_index;
   GLint bind = rosmesa->blue_index;

   for (i=0;i<n;i++) {
      if (mask[i]) {
         GLubyte *ptr3 = PIXELADDR3(x[i],y[i]);
         ptr3[rind] = rgba[i][RCOMP];
         ptr3[gind] = rgba[i][GCOMP];
         ptr3[bind] = rgba[i][BCOMP];
      }
   }
}

static void write_monocolor_pixels3( const GLcontext *ctx,
                                    GLuint n, const GLint x[], const GLint y[],
				    const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint i;
   GLint rind = rosmesa->red_index;
   GLint gind = rosmesa->green_index;
   GLint bind = rosmesa->blue_index;
   GLubyte rval = UNPACK_RED(rosmesa->pixel);
   GLubyte gval = UNPACK_GREEN(rosmesa->pixel);
   GLubyte bval = UNPACK_BLUE(rosmesa->pixel);
   for (i=0;i<n;i++) {
      if (mask[i]) {
         GLubyte *ptr3 = PIXELADDR3(x[i],y[i]);
         ptr3[rind] = rval;
         ptr3[gind] = gval;
         ptr3[bind] = bval;
      }
   }
}

static void read_rgba_span3( const GLcontext *ctx,
                             GLuint n, GLint x, GLint y,
                             GLubyte rgba[][4] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint i;
   GLint rind = rosmesa->red_index;
   GLint gind = rosmesa->green_index;
   GLint bind = rosmesa->blue_index;
   GLubyte *ptr3 = PIXELADDR3( x, y);
   for (i=0;i<n;i++,ptr3+=3) {
      rgba[i][RCOMP]   = ptr3[rind];
      rgba[i][GCOMP] = ptr3[gind];
      rgba[i][BCOMP]  = ptr3[bind];
      rgba[i][ACOMP] = 0;
   }
}

static void read_rgba_pixels3( const GLcontext *ctx,
                               GLuint n, const GLint x[], const GLint y[],
			       GLubyte rgba[][4], const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint i;
   GLint rind = rosmesa->red_index;
   GLint gind = rosmesa->green_index;
   GLint bind = rosmesa->blue_index;
   for (i=0;i<n;i++) {
      if (mask[i]) {
         GLubyte *ptr3 = PIXELADDR3(x[i],y[i]);
         rgba[i][RCOMP] = ptr3[rind];
         rgba[i][GCOMP] = ptr3[gind];
         rgba[i][BCOMP] = ptr3[bind];
         rgba[i][ACOMP] = 0;
      }
   }
}
*/

/**********************************************************************/
/*****                2 byte RGB pixel support funcs              *****/
/**********************************************************************/

#ifndef USE_ARM_ASM
static void write_rgba_span2( const GLcontext *ctx,
                              GLuint n, GLint x, GLint y,
                              const GLubyte rgba[][4], const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLubyte *ptr2 = PIXELADDR2( x, y);
	GLuint i;

	#ifdef DEBUG
	fprintf(stderr, "write_rgba_span2\n");
	#endif

	if (mask)
	{
		for (i=0;i<n;i++,ptr2+=2)
		{
			if (mask[i])
			{
/*
				ptr2[0] = (( ((rgba[i][RCOMP]*31)/255) & 0x1f) | (( ((rgba[i][GCOMP]*31)/255) & 0x7) <<5));
				ptr2[1] = (( ((rgba[i][GCOMP]*31)/255) & 0x18) >> 3) | (( ((rgba[i][BCOMP]*31)/255) & 0x1f) << 2);
*/
				ptr2[0] = ((rgba[i][RCOMP]>>3) & 0x1f) | ( ((rgba[i][GCOMP]>>3) & 0x7) <<5);
				ptr2[1] = (((rgba[i][GCOMP]>>3) & 0x18) >> 3) | (((rgba[i][BCOMP]>>3) & 0x1f) << 2);
			}
		}
	}
	else
	{
		for (i=0;i<n;i++,ptr2+=2)
		{
/*
			ptr2[0] = (( ((rgba[i][RCOMP]*31)/255) & 0x1f) | (( ((rgba[i][GCOMP]*31)/255) & 0x7) <<5));
			ptr2[1] = (( ((rgba[i][GCOMP]*31)/255) & 0x18) >> 3) | (( ((rgba[i][BCOMP]*31)/255) & 0x1f) << 2);
*/
				ptr2[0] = ((rgba[i][RCOMP]>>3) & 0x1f) | ( ((rgba[i][GCOMP]>>3) & 0x7) <<5);
				ptr2[1] = (((rgba[i][GCOMP]>>3) & 0x18) >> 3) | (((rgba[i][BCOMP]>>3) & 0x1f) << 2);
		}
	}
}
#else
static void write_rgba_span2( const GLcontext *ctx,
                              GLuint n, GLint x, GLint y,
                              const GLubyte rgba[][4], const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLubyte *ptr2 = PIXELADDR2( x, y);

	#ifdef DEBUG
	fprintf(stderr, "write_rgba_span2\n");
	#endif

	if (mask)
	{
		asm_write_rgba_span_masked2(n, (GLuint) mask, (GLuint) rgba, (GLuint) ptr2);
	}
	else
	{
		asm_write_rgba_span_unmasked2(n, (GLuint) rgba, (GLuint) ptr2);
	}
}
#endif

#ifndef USE_ARM_ASM
static void write_rgb_span2( const GLcontext *ctx,
                              GLuint n, GLint x, GLint y,
                              const GLubyte rgba[][3], const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLubyte *ptr2 = PIXELADDR2( x, y);
	GLuint i;

	#ifdef DEBUG
	fprintf(stderr, "write_rgb_span2\n");
	#endif

	if (mask)
	{
		for (i=0;i<n;i++,ptr2+=2)
		{
			if (mask[i])
			{
			/*	Hack (Tue 12th May 1998)
		          	ptr2[0] = ((rgba[i][RCOMP] & 0x1f) | ((rgba[i][GCOMP] & 0x7) << 5));
			        ptr2[1] = (((rgba[i][GCOMP] & 0x18) >> 3) | ((rgba[i][BCOMP] & 0x1f) << 2));
			*/
			/*
				ptr2[0] = (( ((rgba[i][RCOMP]*31)/255) & 0x1f) | (( ((rgba[i][GCOMP]*31)/255) & 0x7) <<5));
				ptr2[1] = (( ((rgba[i][GCOMP]*31)/255) & 0x18) >> 3) | (( ((rgba[i][BCOMP]*31)/255) & 0x1f) << 2);
			*/
				ptr2[0] = ((rgba[i][RCOMP]>>3) & 0x1f) | ( ((rgba[i][GCOMP]>>3) & 0x7) <<5);
				ptr2[1] = (((rgba[i][GCOMP]>>3) & 0x18) >> 3) | (((rgba[i][BCOMP]>>3) & 0x1f) << 2);
			}
		}
	}
	else
	{
		for (i=0;i<n;i++,ptr2+=2)
		{
		/*	Hack (Tue 12th May 1998)
	          	ptr2[0] = ((rgba[i][RCOMP] & 0x1f) | ((rgba[i][GCOMP] & 0x7) << 5));
		        ptr2[1] = (((rgba[i][GCOMP] & 0x18) >> 3) | ((rgba[i][BCOMP] & 0x1f) << 2));
		*/
		/*
			ptr2[0] = (( ((rgba[i][RCOMP]*31)/255) & 0x1f) | (( ((rgba[i][GCOMP]*31)/255) & 0x7) <<5));
			ptr2[1] = (( ((rgba[i][GCOMP]*31)/255) & 0x18) >> 3) | (( ((rgba[i][BCOMP]*31)/255) & 0x1f) << 2);
		*/
			ptr2[0] = ((rgba[i][RCOMP]>>3) & 0x1f) | ( ((rgba[i][GCOMP]>>3) & 0x7) <<5);
			ptr2[1] = (((rgba[i][GCOMP]>>3) & 0x18) >> 3) | (((rgba[i][BCOMP]>>3) & 0x1f) << 2);
		}
	}
}
#else
static void write_rgb_span2( const GLcontext *ctx,
                              GLuint n, GLint x, GLint y,
                              const GLubyte rgba[][3], const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLubyte *ptr2 = PIXELADDR2( x, y);

	#ifdef DEBUG
	fprintf(stderr, "write_rgb_span2\n");
	#endif

	if (mask)
	{
		asm_write_rgb_span_masked2(n, (GLuint) mask, (GLuint) rgba, (GLuint) ptr2);
	}
	else
	{
		asm_write_rgb_span_unmasked2(n, (GLuint) rgba, (GLuint) ptr2);
	}
}
#endif

static void write_monocolor_span2( const GLcontext *ctx,
                                  GLuint n, GLint x, GLint y,
				  const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;

/*
   GLubyte rval = UNPACK_RED(rosmesa->pixel);
   GLubyte gval = UNPACK_GREEN(rosmesa->pixel);
   GLubyte bval = UNPACK_BLUE(rosmesa->pixel);
*/
   GLuint val = rosmesa->pixel;
   GLubyte *ptr2 = PIXELADDR2( x, y);
   GLuint i;
#ifdef DEBUG 
fprintf(stderr, "write_monocolor_span2\n");
#endif
   for (i=0;i<n;i++,ptr2+=2) {
      if (mask[i]) {
/*
          	ptr2[0] = ((rval & 0x1f) | ((gval & 0x7) << 5));
	        ptr2[1] = (((gval & 0x18) >> 3) | ((bval & 0x1f) << 2));
*/
		ptr2[0] = (GLubyte)(val & 0xff);
		ptr2[1] = (GLubyte)((val & 0xff00) >> 8);
      }
   }
}

#ifndef USE_ARM_ASM
static void write_rgba_pixels2( const GLcontext *ctx,
                                GLuint n, const GLint x[], const GLint y[],
                                const GLubyte rgba[][4], const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint i;

#ifdef DEBUG
fprintf(stderr, "write_rgba_pixels2\n");
#endif

   for (i=0;i<n;i++) {
      if (mask[i]) {
         GLubyte *ptr2 = PIXELADDR2(x[i],y[i]);
/*	Hack (Tue 12th May 1998)
	 ptr2[0] = ((rgba[i][RCOMP] & 0x1f) | ((rgba[i][GCOMP] & 0x7) << 5));
	 ptr2[1] = (((rgba[i][GCOMP] & 0x18) >> 3) | ((rgba[i][BCOMP] & 0x1f) << 2));
*/
/*
	ptr2[0] = (( ((rgba[i][RCOMP]*31)/255) & 0x1f) | (( ((rgba[i][GCOMP]*31)/255) & 0x7) <<5));
	ptr2[1] = (( ((rgba[i][GCOMP]*31)/255) & 0x18) >> 3) | (( ((rgba[i][BCOMP]*31)/255) & 0x1f) << 2);
*/
	ptr2[0] = ((rgba[i][RCOMP]>>3) & 0x1f) | ( ((rgba[i][GCOMP]>>3) & 0x7) <<5);
	ptr2[1] = (((rgba[i][GCOMP]>>3) & 0x18) >> 3) | (((rgba[i][BCOMP]>>3) & 0x1f) << 2);
      }
   }
}
#else
static void write_rgba_pixels2( const GLcontext *ctx,
                                GLuint n, const GLint x[], const GLint y[],
                                const GLubyte rgba[][4], const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLuint	  parameters[4] = {(GLuint) rgba, (GLuint) mask, (GLuint) (rosmesa->buffer + ((rosmesa->height - 1)*(rosmesa->width * 2))), (GLuint) rosmesa->width};

	#ifdef DEBUG
	fprintf(stderr, "write_rgba_pixels2\n");
	#endif

	asm_write_rgba_pixels2(n, (GLuint) x, (GLuint) y, (GLuint) parameters);
}
#endif

static void write_monocolor_pixels2( const GLcontext *ctx,
                                    GLuint n, const GLint x[], const GLint y[],
				    const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint i;
/*
   GLubyte rval = UNPACK_RED(rosmesa->pixel);
   GLubyte gval = UNPACK_GREEN(rosmesa->pixel);
   GLubyte bval = UNPACK_BLUE(rosmesa->pixel);
*/
   GLuint val = rosmesa->pixel;
#ifdef DEBUG 
fprintf(stderr, "write_monocolor_pixels2\n");
#endif
   for (i=0;i<n;i++) {
      if (mask[i]) {
         GLubyte *ptr2 = PIXELADDR2(x[i],y[i]);
/*
          	ptr2[0] = ((rval & 0x1f) | ((gval & 0x7) << 5));
	        ptr2[1] = (((gval & 0x18) >> 3) | ((bval & 0x1f) << 2));
*/
		ptr2[0] = (GLubyte)(val & 0xff);
		ptr2[1] = (GLubyte)((val & 0xff00) >> 8);
      }
   }
}

#ifndef USE_ARM_ASM
static void read_rgba_span2( const GLcontext *ctx,
                             GLuint n, GLint x, GLint y,
                             GLubyte rgba[][4] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLuint i;
	GLubyte *ptr2 = PIXELADDR2( x, y);

	#ifdef DEBUG
	fprintf(stderr, "read_rgba_span2\n");
	#endif

	for (i=0;i<n;i++,ptr2+=2)
	{
	/*	Hack (Tue 12th May 1998)
		rgba[i][RCOMP]   = (ptr2[0] & 0x1f);
		rgba[i][GCOMP] = (((ptr2[1] & 0x3) << 3) | ((ptr2[0] & 0xe0) >> 5));
		rgba[i][BCOMP]  = (ptr2[1] >> 2) & 0x1f;
		rgba[i][ACOMP] = 0;
	*/
	/*
		rgba[i][RCOMP] = ((ptr2[0] & 0x1f)*255)/31;
		rgba[i][GCOMP] = ((((ptr2[1] & 0x3) << 3) | ((ptr2[0] & 0xe0) >> 5))*255)/31;
		rgba[i][BCOMP]  = (((ptr2[1] >> 2) & 0x1f)*255)/31;
		rgba[i][ACOMP] = 0;
	*/
		rgba[i][RCOMP] = (ptr2[0] & 0x1f) << 3;
		rgba[i][GCOMP] = ((((ptr2[1] & 0x3) << 3) | ((ptr2[0] & 0xe0) >> 5)) << 3);
		rgba[i][BCOMP]  = (((ptr2[1] >> 2) & 0x1f) << 3);
		rgba[i][ACOMP] = 0;
	}
}
#else
static void read_rgba_span2( const GLcontext *ctx,
                             GLuint n, GLint x, GLint y,
                             GLubyte rgba[][4] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLubyte *ptr2 = PIXELADDR2( x, y);

	#ifdef DEBUG
	fprintf(stderr, "read_rgba_span2\n");
	#endif

	asm_read_rgba_span2(n, (GLuint) rgba, (GLuint) ptr2);
}
#endif

#ifndef USE_ARM_ASM
static void read_rgba_pixels2( const GLcontext *ctx,
                               GLuint n, const GLint x[], const GLint y[],
			       GLubyte rgba[][4], const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLuint i;

	#ifdef DEBUG
	fprintf(stderr, "read_rgba_pixels2\n");
	#endif

	for (i=0;i<n;i++)
	{
		if (mask[i])
		{
			GLubyte *ptr2 = PIXELADDR2(x[i],y[i]);
		/*	Hack (Tue 12th May 1998)
			rgba[i][RCOMP]   = (ptr2[0] & 0x1f);
			rgba[i][GCOMP] = (((ptr2[1] & 0x3) << 3) | ((ptr2[0] & 0xe0) >> 5));
			rgba[i][BCOMP]  = (ptr2[1] >> 2) & 0x1f;
			rgba[i][ACOMP] = 0;
		*/
		/*
			rgba[i][RCOMP] = ((ptr2[0] & 0x1f)*255)/31;
			rgba[i][GCOMP] = ((((ptr2[1] & 0x3) << 3) | ((ptr2[0] & 0xe0) >> 5))*255)/31;
			rgba[i][BCOMP]  = (((ptr2[1] >> 2) & 0x1f)*255)/31;
			rgba[i][ACOMP] = 0;
		*/
			rgba[i][RCOMP] = (ptr2[0] & 0x1f) << 3;
			rgba[i][GCOMP] = ((((ptr2[1] & 0x3) << 3) | ((ptr2[0] & 0xe0) >> 5)) << 3);
			rgba[i][BCOMP]  = (((ptr2[1] >> 2) & 0x1f) << 3);
			rgba[i][ACOMP] = 0;
		}
	}
}
#else
static void read_rgba_pixels2( const GLcontext *ctx,
                               GLuint n, const GLint x[], const GLint y[],
			       GLubyte rgba[][4], const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLuint	  parameters[4] = {(GLuint) rgba, (GLuint) mask, (GLuint) (rosmesa->buffer + ((rosmesa->height - 1)*(rosmesa->width * 2))), (GLuint) rosmesa->width};

	#ifdef DEBUG
	fprintf(stderr, "read_rgba_pixels2\n");
	#endif

	asm_read_rgba_pixels2(n, (GLuint) x, (GLuint) y, (GLuint) parameters);
}
#endif


/* ******************** Dithered 2 byte functions **************************************** */

#ifndef USE_ARM_ASM
static void write_rgba_span2_dither( const GLcontext *ctx,
                              GLuint n, GLint x, GLint y,
                              const GLubyte rgba[][4], const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLubyte *ptr2 = PIXELADDR2( x, y);
	GLuint i, mult, remainder;

	#ifdef DEBUG
	fprintf(stderr, "write_rgba_span2_dither\n");
	#endif

	if (((x+y) % 2) == 0)
	{
		/* Even pixel start */
		if (mask)
		{
			remainder = n % 2;
			mult = (n - remainder);

			i = 0;
			while (i<mult)
			{
				if (mask[i])
				{
					ptr2[0] = (( (GLuint)ceil(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
					ptr2[1] = (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)ceil(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
				}
				if (mask[i+1])
				{
					ptr2[2] = (( (GLuint)floor(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
					ptr2[3] = (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)floor(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
				}
				i += 2; ptr2 += 4;
			}
			if ( (remainder) && mask[i] )
			{
				ptr2[0] = (( (GLuint)ceil(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[1] = (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)ceil(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
			}
		}
		else
		{
			remainder = n % 2;
			mult = (n - remainder);

			i = 0;
			while (i<mult)
			{
				ptr2[0] = (( (GLuint)ceil(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[1] = (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)ceil(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
				ptr2[2] = (( (GLuint)floor(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[3] = (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)floor(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
				i += 2; ptr2 += 4;
			}
			if (remainder)
			{
				ptr2[0] = (( (GLuint)ceil(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[1] = (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)ceil(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
			}
		}
	}
	else
	{
		/* Odd pixel start */
		if (mask)
		{
			remainder = n % 2;
			mult = (n - remainder);

			i = 0;
			while (i<mult)
			{
				if (mask[i])
				{
					ptr2[0] = (( (GLuint)floor(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
					ptr2[1] = (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)floor(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
				}
				if (mask[i+1])
				{
					ptr2[2] = (( (GLuint)ceil(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
					ptr2[3] = (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)ceil(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
				}
				i += 2; ptr2 += 4;
			}
			if ( (remainder) && mask[i] )
			{
				ptr2[0] = (( (GLuint)floor(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[1] = (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)floor(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
			}
		}
		else
		{
			remainder = n % 2;
			mult = (n - remainder);

			i = 0;
			while (i<mult)
			{
				ptr2[0] = (( (GLuint)floor(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[1] = (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)floor(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
				ptr2[2] = (( (GLuint)ceil(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[3] = (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)ceil(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
				i += 2; ptr2 += 4;
			}
			if (remainder)
			{
				ptr2[0] = (( (GLuint)floor(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[1] = (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)floor(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
			}
		}
	}
}
#else
static void write_rgba_span2_dither( const GLcontext *ctx,
                              GLuint n, GLint x, GLint y,
                              const GLubyte rgba[][4], const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLubyte *ptr2 = PIXELADDR2( x, y);

	#ifdef DEBUG
	fprintf(stderr, "write_rgba_span2_dither\n");
	#endif

	if (((x+y) % 2) == 0)
	{
		/* Even pixel start */
		if (mask)
		{
			asm_write_rgba_span_masked_dither_even2(n, (GLuint) mask, (GLuint) rgba, (GLuint) ptr2);
		}
		else
		{
			asm_write_rgba_span_unmasked_dither_even2(n, (GLuint) rgba, (GLuint) ptr2);
		}
	}
	else
	{
		/* Odd pixel start */
		if (mask)
		{
			asm_write_rgba_span_masked_dither_odd2(n, (GLuint) mask, (GLuint) rgba, (GLuint) ptr2);
		}
		else
		{
			asm_write_rgba_span_unmasked_dither_odd2(n, (GLuint) rgba, (GLuint) ptr2);
		}
	}
}
#endif

#ifndef USE_ARM_ASM
static void write_rgb_span2_dither( const GLcontext *ctx,
                              GLuint n, GLint x, GLint y,
                              const GLubyte rgba[][3], const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLubyte *ptr2 = PIXELADDR2( x, y);
	GLuint i, mult, remainder;

	#ifdef DEBUG
	fprintf(stderr, "write_rgb_span2_dither\n");
	#endif

	if (((x+y) % 2) == 0)
	{
		/* Even pixel start */
		if (mask)
		{
			remainder = n % 2;
			mult = (n - remainder);

			i = 0;
			while (i<mult)
			{
				if (mask[i])
				{
					ptr2[0] = (( (GLuint)ceil(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
					ptr2[1] = (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)ceil(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
				}
				if (mask[i+1])
				{
					ptr2[2] = (( (GLuint)floor(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
					ptr2[3] = (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)floor(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
				}
				i += 2; ptr2 += 4;
			}
			if ( (remainder) && mask[i] )
			{
				ptr2[0] = (( (GLuint)ceil(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[1] = (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)ceil(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
			}
		}
		else
		{
			remainder = n % 2;
			mult = (n - remainder);

			i = 0;
			while (i<mult)
			{
				ptr2[0] = (( (GLuint)ceil(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[1] = (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)ceil(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
				ptr2[2] = (( (GLuint)floor(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[3] = (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)floor(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
				i += 2; ptr2 += 4;
			}
			if (remainder)
			{
				ptr2[0] = (( (GLuint)ceil(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[1] = (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)ceil(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
			}
		}
	}
	else
	{
		/* Odd pixel start */
		if (mask)
		{
			remainder = n % 2;
			mult = (n - remainder);

			i = 0;
			while (i<mult)
			{
				if (mask[i])
				{
					ptr2[0] = (( (GLuint)floor(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
					ptr2[1] = (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)floor(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
				}
				if (mask[i+1])
				{
					ptr2[2] = (( (GLuint)ceil(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
					ptr2[3] = (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)ceil(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
				}
				i += 2; ptr2 += 4;
			}
			if ( (remainder) && mask[i] )
			{
				ptr2[0] = (( (GLuint)floor(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[1] = (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)floor(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
			}
		}
		else
		{
			remainder = n % 2;
			mult = (n - remainder);

			i = 0;
			while (i<mult)
			{
				ptr2[0] = (( (GLuint)floor(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[1] = (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)floor(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
				ptr2[2] = (( (GLuint)ceil(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[3] = (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)ceil(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
				i += 2; ptr2 += 4;
			}
			if (remainder)
			{
				ptr2[0] = (( (GLuint)floor(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[1] = (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)floor(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
			}
		}
	}
}
#else
static void write_rgb_span2_dither( const GLcontext *ctx,
                              GLuint n, GLint x, GLint y,
                              const GLubyte rgba[][3], const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLubyte *ptr2 = PIXELADDR2( x, y);

	#ifdef DEBUG
	fprintf(stderr, "write_rgb_span2_dither\n");
	#endif

	if (((x+y) % 2) == 0)
	{
		/* Even pixel start */
		if (mask)
		{
			asm_write_rgb_span_masked_dither_even2(n, (GLuint) mask, (GLuint) rgba, (GLuint) ptr2);
		}
		else
		{
			asm_write_rgb_span_unmasked_dither_even2(n, (GLuint) rgba, (GLuint) ptr2);
		}
	}
	else
	{
		/* Odd pixel start */
		if (mask)
		{
			asm_write_rgb_span_masked_dither_odd2(n, (GLuint) mask, (GLuint) rgba, (GLuint) ptr2);
		}
		else
		{
			asm_write_rgb_span_unmasked_dither_odd2(n, (GLuint) rgba, (GLuint) ptr2);
		}
	}
}
#endif

static void write_monocolor_span2_dither( const GLcontext *ctx,
                                  GLuint n, GLint x, GLint y,
				  const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;

	GLuint	val1;
	GLuint	val2;
	GLubyte	*ptr2 = PIXELADDR2( x, y);
	GLuint	i, mult, remainder;
#ifdef DEBUG 
fprintf(stderr, "write_monocolor_span2_dither\n");
#endif

	/* If starting on an odd pixel then swap the colour values. */
	if (((x+y) % 2) == 0)
	{
		val1 = (rosmesa->pixel & 0xffff);
		val2 = (rosmesa->pixel & 0xffff0000) >> 16;
	}
	else
	{
		val2 = (rosmesa->pixel & 0xffff);
		val1 = (rosmesa->pixel & 0xffff0000) >> 16;
	}

	remainder = n % 2;
	mult = (n - remainder);

	i = 0;
	while (i<mult)
	{
		if (mask[i])
		{
			ptr2[0] = (GLubyte)(val1 & 0xff);
			ptr2[1] = (GLubyte)((val1 & 0xff00) >> 8);
		}
		if (mask[i+1])
		{
			ptr2[2] = (GLubyte)(val2 & 0xff);
			ptr2[3] = (GLubyte)((val2 & 0xff00) >> 8);
		}
		i += 2; ptr2 += 4;
	}
	if ( (remainder) && mask[i] )
	{
		ptr2[0] = (GLubyte)(val1 & 0xff);
		ptr2[1] = (GLubyte)((val1 & 0xff00) >> 8);
	}
}

static void write_rgba_pixels2_dither( const GLcontext *ctx,
                                GLuint n, const GLint x[], const GLint y[],
                                const GLubyte rgba[][4], const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLuint i;
#ifdef DEBUG 
fprintf(stderr, "write_rgba_pixels2_dither\n");
#endif

	for (i=0;i<n;i++)
	{
		if (mask[i])
		{
			GLubyte *ptr2 = PIXELADDR2(x[i],y[i]);
			if (((x[i] * y[i]) % 2) == 0)
			{
				ptr2[0] = (( (GLuint)ceil(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[1] = (( (GLuint)ceil(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)ceil(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
			}
			else
			{
				ptr2[0] = (( (GLuint)floor(((rgba[i][RCOMP]*31)/255)) & 0x1f) | (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x7) <<5));
				ptr2[1] = (( (GLuint)floor(((rgba[i][GCOMP]*31)/255)) & 0x18) >> 3) | (( (GLuint)floor(((rgba[i][BCOMP]*31)/255)) & 0x1f) << 2);
			}
		}
	}
}

static void write_monocolor_pixels2_dither( const GLcontext *ctx,
                                    GLuint n, const GLint x[], const GLint y[],
				    const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLuint i, m;
	GLuint val[2] = {(rosmesa->pixel & 0xffff), ((rosmesa->pixel & 0xffff0000) >> 16)};
#ifdef DEBUG 
fprintf(stderr, "write_monocolor_pixels2_dither\n");
#endif
	for (i=0; i<n; i++)
	{
		if (mask[i])
		{
			GLubyte *ptr2 = PIXELADDR2(x[i],y[i]);
			m = ((x[i] * y[i]) % 2);
			ptr2[0] = (GLubyte)(val[m] & 0xff);
			ptr2[1] = (GLubyte)((val[m] & 0xff00) >> 8);
		}
	}
}

static void read_rgba_span2_dither( const GLcontext *ctx,
                             GLuint n, GLint x, GLint y,
                             GLubyte rgba[][4] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLuint i;
	GLubyte *ptr2 = PIXELADDR2( x, y);
#ifdef DEBUG 
fprintf(stderr, "read_rgba_span2_dither\n");
#endif

	for (i=0;i<n;i++,ptr2+=2)
	{
		rgba[i][RCOMP] = ((ptr2[0] & 0x1f)*255)/31;
		rgba[i][GCOMP] = ((((ptr2[1] & 0x3) << 3) | ((ptr2[0] & 0xe0) >> 5))*255)/31;
		rgba[i][BCOMP]  = (((ptr2[1] >> 2) & 0x1f)*255)/31;
		rgba[i][ACOMP] = 0;
	}
}

static void read_rgba_pixels2_dither( const GLcontext *ctx,
                               GLuint n, const GLint x[], const GLint y[],
			       GLubyte rgba[][4], const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLuint i;
#ifdef DEBUG 
fprintf(stderr, "read_rgba_pixels2_dither\n");
#endif
	for (i=0;i<n;i++)
	{
		if (mask[i])
		{
			GLubyte *ptr2 = PIXELADDR2(x[i],y[i]);
			rgba[i][RCOMP] = ((ptr2[0] & 0x1f)*255)/31;
			rgba[i][GCOMP] = ((((ptr2[1] & 0x3) << 3) | ((ptr2[0] & 0xe0) >> 5))*255)/31;
			rgba[i][BCOMP]  = (((ptr2[1] >> 2) & 0x1f)*255)/31;
			rgba[i][ACOMP] = 0;
		}
	}
}


/**********************************************************************/
/*****                1 byte RGB pixel support funcs              *****/
/**********************************************************************/

static void write_rgba_span1( const GLcontext *ctx,
                              GLuint n, GLint x, GLint y,
                              const GLubyte rgba[][4], const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLubyte *ptr1 = PIXELADDR1( x, y);
   GLuint i;
#ifdef DEBUG 
fprintf(stderr, "write_rgba_span1\n");
#endif

   if (mask) {
      for (i=0;i<n;i++,ptr1+=1) {
         if (mask[i]) {
/*	Hack (Tue 12th May 1998)
          	ptr1[0] = rosmesa_colourtable[rgba[i][RCOMP]][rgba[i][GCOMP]][rgba[i][BCOMP]];
*/
/*	Thu 18th March 1999
         	ptr1[0] = rosmesa_colourtable[((rgba[i][RCOMP]*15)/255)][((rgba[i][GCOMP]*15)/255)][((rgba[i][BCOMP]*15)/255)][0];
*/
         	ptr1[0] = rosmesa_colourtable[(rgba[i][RCOMP]>>4)][(rgba[i][GCOMP]>>4)][(rgba[i][BCOMP]>>4)][0];
         }
      }
   }
   else {
      for (i=0;i<n;i++,ptr1+=1) {
/*	Hack (Tue 12th May 1998)
          	ptr1[0] = rosmesa_colourtable[rgba[i][RCOMP]][rgba[i][GCOMP]][rgba[i][BCOMP]];
*/
/*	Thu 18th March 1999
		ptr1[0] = rosmesa_colourtable[((rgba[i][RCOMP]*15)/255)][((rgba[i][GCOMP]*15)/255)][((rgba[i][BCOMP]*15)/255)][0];
*/
         	ptr1[0] = rosmesa_colourtable[(rgba[i][RCOMP]>>4)][(rgba[i][GCOMP]>>4)][(rgba[i][BCOMP]>>4)][0];
      }
   }
}

static void write_rgb_span1( const GLcontext *ctx,
                              GLuint n, GLint x, GLint y,
                              const GLubyte rgba[][3], const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLubyte *ptr1 = PIXELADDR1( x, y);
   GLuint i;
#ifdef DEBUG 
fprintf(stderr, "write_rgb_span1\n");
#endif
   if (mask) {
      for (i=0;i<n;i++,ptr1+=1) {
         if (mask[i]) {
/*	Hack (Tue 12th May 1998)
          	ptr1[0] = rosmesa_colourtable[rgba[i][RCOMP]][rgba[i][GCOMP]][rgba[i][BCOMP]];
*/
/*	Thu 18th March 1999
         	ptr1[0] = rosmesa_colourtable[((rgba[i][RCOMP]*15)/255)][((rgba[i][GCOMP]*15)/255)][((rgba[i][BCOMP]*15)/255)][0];
*/
         	ptr1[0] = rosmesa_colourtable[(rgba[i][RCOMP]>>4)][(rgba[i][GCOMP]>>4)][(rgba[i][BCOMP]>>4)][0];
         }
      }
   }
   else {
      for (i=0;i<n;i++,ptr1+=1) {
/*	Hack (Tue 12th May 1998)
          	ptr1[0] = rosmesa_colourtable[rgba[i][RCOMP]][rgba[i][GCOMP]][rgba[i][BCOMP]];
*/
/*	Thu 18th March 1999
      		ptr1[0] = rosmesa_colourtable[((rgba[i][RCOMP]*15)/255)][((rgba[i][GCOMP]*15)/255)][((rgba[i][BCOMP]*15)/255)][0];
*/
         	ptr1[0] = rosmesa_colourtable[(rgba[i][RCOMP]>>4)][(rgba[i][GCOMP]>>4)][(rgba[i][BCOMP]>>4)][0];
      }
   }
}

static void write_monocolor_span1( const GLcontext *ctx,
                                  GLuint n, GLint x, GLint y,
				  const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;

/*
   GLubyte rval = UNPACK_RED(rosmesa->pixel);
   GLubyte gval = UNPACK_GREEN(rosmesa->pixel);
   GLubyte bval = UNPACK_BLUE(rosmesa->pixel);
*/
   GLuint val = rosmesa->pixel;

   GLubyte *ptr1 = PIXELADDR1( x, y);
   GLuint i;
#ifdef DEBUG 
fprintf(stderr, "write_monocolor_span1\n");
#endif

   for (i=0;i<n;i++,ptr1+=1) {
      if (mask[i]) {
		ptr1[0] = (GLubyte)val;
      }
   }
}

static void write_rgba_pixels1( const GLcontext *ctx,
                                GLuint n, const GLint x[], const GLint y[],
                                const GLubyte rgba[][4], const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint i;
#ifdef DEBUG 
fprintf(stderr, "write_rgba_pixels1\n");
#endif

   for (i=0;i<n;i++) {
      if (mask[i]) {
         GLubyte *ptr1 = PIXELADDR1(x[i],y[i]);
/*	Hack (Tue 12th May 1998)
	 ptr1[0] = rosmesa_colourtable[rgba[i][RCOMP]][rgba[i][GCOMP]][rgba[i][BCOMP]];
*/
/*	Thu 18th March 1999
	ptr1[0] = rosmesa_colourtable[((rgba[i][RCOMP]*15)/255)][((rgba[i][GCOMP]*15)/255)][((rgba[i][BCOMP]*15)/255)][0];
*/
       	ptr1[0] = rosmesa_colourtable[(rgba[i][RCOMP]>>4)][(rgba[i][GCOMP]>>4)][(rgba[i][BCOMP]>>4)][0];
      }
   }
}

static void write_monocolor_pixels1( const GLcontext *ctx,
                                    GLuint n, const GLint x[], const GLint y[],
				    const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint i;
/*
   GLubyte rval = UNPACK_RED(rosmesa->pixel);
   GLubyte gval = UNPACK_GREEN(rosmesa->pixel);
   GLubyte bval = UNPACK_BLUE(rosmesa->pixel);
*/
   GLuint val = rosmesa->pixel;
#ifdef DEBUG 
fprintf(stderr, "write_monocolor_pixels1\n");
#endif

   for (i=0;i<n;i++) {
      if (mask[i]) {
         GLubyte *ptr1 = PIXELADDR1(x[i],y[i]);
       	 ptr1[0] = (GLubyte)val;
      }
   }
}

static void read_rgba_span1( const GLcontext *ctx,
                             GLuint n, GLint x, GLint y,
                             GLubyte rgba[][4] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint i;
   GLubyte *ptr1 = PIXELADDR1( x, y);
#ifdef DEBUG 
fprintf(stderr, "read_rgba_span1\n");
#endif

   for (i=0;i<n;i++,ptr1+=1) {
/*	Hack (Tue 12th May 1998)
      rgba[i][RCOMP]   = rosmesa_redtable[ptr1[0]];
      rgba[i][GCOMP] = rosmesa_greentable[ptr1[0]];
      rgba[i][BCOMP]  = rosmesa_bluetable[ptr1[0]];
      rgba[i][ACOMP] = 0;
*/
/*	Thu 18th March 1999
	rgba[i][RCOMP] = ((rosmesa_redtable[ptr1[0]][0]*255)/15);
	rgba[i][GCOMP] = ((rosmesa_greentable[ptr1[0]][0]*255)/15);
	rgba[i][BCOMP] = ((rosmesa_bluetable[ptr1[0]][0]*255)/15);
	rgba[i][ACOMP] = 0;
*/
	rgba[i][RCOMP] = (rosmesa_redtable[ptr1[0]][0]<<4);
	rgba[i][GCOMP] = (rosmesa_greentable[ptr1[0]][0]<<4);
	rgba[i][BCOMP] = (rosmesa_bluetable[ptr1[0]][0]<<4);
	rgba[i][ACOMP] = 0;
   }
}

static void read_rgba_pixels1( const GLcontext *ctx,
                               GLuint n, const GLint x[], const GLint y[],
			       GLubyte rgba[][4], const GLubyte mask[] )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
   GLuint i;
#ifdef DEBUG 
fprintf(stderr, "read_rgba_pixels1\n");
#endif

   for (i=0;i<n;i++) {
      if (mask[i]) {
         GLubyte *ptr1 = PIXELADDR1(x[i],y[i]);
/*	Hack (Tue 12th May 1998)
         rgba[i][RCOMP]   = rosmesa_redtable[ptr1[0]];
         rgba[i][GCOMP] = rosmesa_greentable[ptr1[0]];
         rgba[i][BCOMP]  = rosmesa_bluetable[ptr1[0]];
         rgba[i][ACOMP] = 0;
*/
/*	Thu 18th March 1999
	rgba[i][RCOMP] = ((rosmesa_redtable[ptr1[0]][0]*255)/15);
	rgba[i][GCOMP] = ((rosmesa_greentable[ptr1[0]][0]*255)/15);
	rgba[i][BCOMP] = ((rosmesa_bluetable[ptr1[0]][0]*255)/15);
	rgba[i][ACOMP] = 0;
*/
	rgba[i][RCOMP] = (rosmesa_redtable[ptr1[0]][0]<<4);
	rgba[i][GCOMP] = (rosmesa_greentable[ptr1[0]][0]<<4);
	rgba[i][BCOMP] = (rosmesa_bluetable[ptr1[0]][0]<<4);
	rgba[i][ACOMP] = 0;
      }
   }
}

/* ************* Dithered pixel functions ***************************** */

static void write_rgba_span1_dither( const GLcontext *ctx,
                              GLuint n, GLint x, GLint y,
                              const GLubyte rgba[][4], const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLubyte *ptr2 = PIXELADDR1( x, y);	/* using two bytes */
	GLuint	i, mult, remainder;
	GLuint	i1, i2;
#ifdef DEBUG 
fprintf(stderr, "write_rgba_span1_dither\n");
#endif

	/* If starting on an even pixel then use the left and right pixels for dithering
	   otherwise swap them round. */
	if (((x+y) % 2) == 0) { i1 = 0; i2 = 1; } else { i1 = 1; i2 = 0; }

	if (mask)
	{
		remainder = (n % 2);
		mult = n - remainder;

		i = 0;
		while (i<mult)
		{
			if (mask[i]) ptr2[0] = rosmesa_colourtable[(rgba[i][RCOMP]>>4)][(rgba[i][GCOMP]>>4)][(rgba[i][BCOMP]>>4)][i1];
			if (mask[i+1]) ptr2[1] = rosmesa_colourtable[(rgba[i][RCOMP]>>4)][(rgba[i][GCOMP]>>4)][(rgba[i][BCOMP]>>4)][i2];
			i += 2; ptr2 += 2;
		}
	}
	else
	{
		remainder = (n % 2);
		mult = n - remainder;

		i = 0;
		while (i<mult)
		{
			ptr2[0] = rosmesa_colourtable[(rgba[i][RCOMP]>>4)][(rgba[i][GCOMP]>>4)][(rgba[i][BCOMP]>>4)][i1];
			ptr2[1] = rosmesa_colourtable[(rgba[i][RCOMP]>>4)][(rgba[i][GCOMP]>>4)][(rgba[i][BCOMP]>>4)][i2];
			i += 2; ptr2 += 2;
		}
		if (remainder) ptr2[0] = rosmesa_colourtable[(rgba[i][RCOMP]>>4)][(rgba[i][GCOMP]>>4)][(rgba[i][BCOMP]>>4)][i1];
	}
}

static void write_rgb_span1_dither( const GLcontext *ctx,
                              GLuint n, GLint x, GLint y,
                              const GLubyte rgba[][3], const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLubyte *ptr2 = PIXELADDR1( x, y);	/* using two bytes */
	GLuint	i, mult, remainder;
	GLuint	i1, i2;
#ifdef DEBUG 
fprintf(stderr, "write_rgb_span1_dither\n");
#endif

	/* If starting on an even pixel then use the left and right pixels for dithering
	   otherwise swap them round. */
	if (((x+y) % 2) == 0) { i1 = 0; i2 = 1; } else { i1 = 1; i2 = 0; }

	if (mask)
	{
		remainder = (n % 2);
		mult = n - remainder;

		i = 0;
		while (i<mult)
		{
			if (mask[i]) ptr2[0] = rosmesa_colourtable[(rgba[i][RCOMP]>>4)][(rgba[i][GCOMP]>>4)][(rgba[i][BCOMP]>>4)][i1];
			if (mask[i+1]) ptr2[1] = rosmesa_colourtable[(rgba[i][RCOMP]>>4)][(rgba[i][GCOMP]>>4)][(rgba[i][BCOMP]>>4)][i2];
			i += 2; ptr2 += 2;
		}
		if ( (mask[i]) && remainder) ptr2[0] = rosmesa_colourtable[(rgba[i][RCOMP]>>4)][(rgba[i][GCOMP]>>4)][(rgba[i][BCOMP]>>4)][i1];
	}
	else
	{
		remainder = (n % 2);
		mult = n - remainder;

		i = 0;
		while (i<mult)
		{
			ptr2[0] = rosmesa_colourtable[(rgba[i][RCOMP]>>4)][(rgba[i][GCOMP]>>4)][(rgba[i][BCOMP]>>4)][i1];
			ptr2[1] = rosmesa_colourtable[(rgba[i][RCOMP]>>4)][(rgba[i][GCOMP]>>4)][(rgba[i][BCOMP]>>4)][i2];
			i += 2; ptr2 += 2;
		}
		if (remainder) ptr2[0] = rosmesa_colourtable[(rgba[i][RCOMP]>>4)][(rgba[i][GCOMP]>>4)][(rgba[i][BCOMP]>>4)][i1];

	}
}

static void write_monocolor_span1_dither( const GLcontext *ctx,
                                  GLuint n, GLint x, GLint y,
				  const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;

	GLuint val1 = (rosmesa->pixel & 0x00ff);
	GLuint val2 = ((rosmesa->pixel & 0xff00) >> 8);

	GLubyte *ptr2 = PIXELADDR1( x, y);
	GLuint	val;
	GLuint	i, mult, remainder;
#ifdef DEBUG 
fprintf(stderr, "write_monocolor_span1_dither\n");
#endif

	/* If starting on an odd pixel then swap the colour values around to keep the dither pattern. */
	if (((x+y) % 2)==1)
	{
		val = val1;
		val1 = val2;
		val2 = val;
	}

	remainder = n % 2;
	mult = n - remainder;

	i = 0;
	while (i<mult)
	{
		if (mask[i]) ptr2[0] = (GLubyte)val1;
		if (mask[i+1]) ptr2[1] = (GLubyte)val2;
		i += 2; ptr2 += 2;
	}
	if ( (mask[i]) && remainder) ptr2[0] = (GLubyte)val1;
}

static void write_rgba_pixels1_dither( const GLcontext *ctx,
                                GLuint n, const GLint x[], const GLint y[],
                                const GLubyte rgba[][4], const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLuint i, m;
#ifdef DEBUG 
fprintf(stderr, "write_rgba_pixels1_dither\n");
#endif

	for (i=0; i<n; i++)
	{
		if (mask[i])
		{
			GLubyte *ptr1 = PIXELADDR1(x[i],y[i]);
			m = ((x[i] * y[i]) % 2);
			ptr1[0] = rosmesa_colourtable[(rgba[i][RCOMP]>>4)][(rgba[i][GCOMP]>>4)][(rgba[i][BCOMP]>>4)][m];
		}
	}
}

static void write_monocolor_pixels1_dither( const GLcontext *ctx,
                                    GLuint n, const GLint x[], const GLint y[],
				    const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLuint i, m;

	GLuint val[2] = {(rosmesa->pixel & 0x00ff), ((rosmesa->pixel & 0xff00) >> 8)};
#ifdef DEBUG 
fprintf(stderr, "write_monocolor_pixels1_dither\n");
#endif

	for (i=0; i<n; i++)
	{
		if (mask[i])
		{
	        	GLubyte *ptr1 = PIXELADDR1(x[i],y[i]);
			m = ((x[i] * y[i]) % 2);
			ptr1[0] = (GLubyte)val[m];
		}
	}
}

static void read_rgba_span1_dither( const GLcontext *ctx,
                             GLuint n, GLint x, GLint y,
                             GLubyte rgba[][4] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLuint i, m;
	GLubyte *ptr1 = PIXELADDR1( x, y);
#ifdef DEBUG 
fprintf(stderr, "read_rgba_span1_dither\n");
#endif

	/* Determine whether reading begins from an odd or even pixel. */
	m = ((x+y) % 2);
	for (i=0; i<n; i++, ptr1 += 1)
	{
		/* Take rgb value by looking at which value could produced it in an odd or even pixel. */
		rgba[i][RCOMP] = (rosmesa_redtable[ptr1[0]][m]<<4);
		rgba[i][GCOMP] = (rosmesa_greentable[ptr1[0]][m]<<4);
		rgba[i][BCOMP] = (rosmesa_bluetable[ptr1[0]][m]<<4);
		rgba[i][ACOMP] = 0;
		/* Refer to other pixel in the inverse table. */
		m = 1 - m;
	}
}

static void read_rgba_pixels1_dither( const GLcontext *ctx,
                               GLuint n, const GLint x[], const GLint y[],
			       GLubyte rgba[][4], const GLubyte mask[] )
{
	ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
	GLuint i, m;
#ifdef DEBUG 
fprintf(stderr, "read_rgba_pixels1_dither\n");
#endif

	for (i=0; i<n; i++)
	{
		if (mask[i])
		{
			GLubyte *ptr1 = PIXELADDR1(x[i],y[i]);
			/* Determine whether reading an odd or even pixel. */
			m = ((x[i] * y[i]) % 2);

			rgba[i][RCOMP] = (rosmesa_redtable[ptr1[0]][m]<<4);
			rgba[i][GCOMP] = (rosmesa_greentable[ptr1[0]][m]<<4);
			rgba[i][BCOMP] = (rosmesa_bluetable[ptr1[0]][m]<<4);
			rgba[i][ACOMP] = 0;
		}
	}
}

/* Old conversion system
((r & 0x2)<<3) | ((r & 0x1)<<2) | ((g & 0x2)<<5) | ((g & 0x1)<<5) | ((b & 0x2)<<6) | ((b & 0x1)<<3) | (a & 0x3);
*/

/**********************************************************************/
/*****                   Optimized line rendering                 *****/
/**********************************************************************/


/*
 * Draw a flat-shaded, RGB line into an osmesa buffer.
 */
static void flat_rgba_line( GLcontext *ctx,
                             GLuint vert0, GLuint vert1, GLuint pvert )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
/* Updated by David Boddie (Tue 01st June 1999) */
   GLubyte *color = ctx->VB->ColorPtr->data[pvert];
   unsigned long pixel = PACK_RGBA( color[0], color[1], color[2], color[3] );

#ifdef DEBUG 
fprintf(stderr, "flat_rgba_line\n");
#endif

#define INTERP_XY 1
#define CLIP_HACK 1
#define PLOT(X,Y) { GLuint *ptr4 = PIXELADDR4(X,Y); *ptr4 = pixel; }

#include "linetemp.h"
}

/*
 * Draw a flat-shaded, RGB line into an osmesa buffer.
 * (16 bpp version)
 */
static void flat_rgba_line2( GLcontext *ctx,
                              GLuint vert0, GLuint vert1, GLuint pvert )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
/* Updated by David Boddie (Tue 01st June 1999) */
   GLubyte *color = ctx->VB->ColorPtr->data[pvert];
   unsigned long pixel = ( ((color[0]*31)/255) & 0x1f) | (( ((color[1]*31)/255) & 0x1f) << 5) | (( ((color[2]*31)/255) & 0x1f) << 10);

#ifdef DEBUG 
fprintf(stderr, "flat_rgba_line2\n");
#endif

#define INTERP_XY 1
#define CLIP_HACK 1
#define PLOT(X,Y) { GLubyte *ptr2 = PIXELADDR2(X,Y); ptr2[0] = (pixel & 0xff); ptr2[1] = ((pixel & 0xff00) >> 8); }

#include "linetemp.h"
}

/*
 * Draw a flat-shaded, RGB line into an osmesa buffer.
 * (8 bpp version)
 */
static void flat_rgba_line1( GLcontext *ctx,
                              GLuint vert0, GLuint vert1, GLuint pvert )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx->DriverCtx;
/* Updated by David Boddie (Tue 01st June 1999) */
   GLubyte *color = ctx->VB->ColorPtr->data[pvert];
   unsigned long pixel = rosmesa_colourtable[(color[0]>>4)][(color[1]>>4)][(color[2]>>4)][0];

/* Temporarily use only one pixel from the colour table - DB (Sat 20th June 1998). */

#ifdef DEBUG 
fprintf(stderr, "flat_rgba_line1\n");
#endif

#define INTERP_XY 1
#define CLIP_HACK 1
#define PLOT(X,Y) { GLubyte *ptr1 = PIXELADDR1(X,Y); *ptr1 = pixel; }

#include "linetemp.h"
}

/*
 * Draw a flat-shaded, Z-less, RGB line into a rosmesa buffer.
 */
static void flat_rgba_z_line( GLcontext *ctx,
                               GLuint vert0, GLuint vert1, GLuint pvert )
{
/* Updated by David Boddie (Tue 01st June 1999, Sat 05th May 2001) */
   ROSMesaContext rosmesa = (ROSMesaContext) ctx;
   GLubyte *color = ctx->VB->ColorPtr->data[pvert];
   unsigned long pixel = PACK_RGBA( color[0], color[1], color[2], color[3] );

#ifdef DEBUG 
fprintf(stderr, "flat_rgba_z_line\n");
#endif

#define INTERP_XY 1
#define INTERP_Z 1
#define DEPTH_TYPE DEFAULT_SOFTWARE_DEPTH_TYPE
#define CLIP_HACK 1
#define PLOT(X,Y)				\
	if (Z < *zPtr) {			\
	   GLuint *ptr4 = PIXELADDR4(X,Y);	\
	   *ptr4 = pixel;			\
	   *zPtr = Z;				\
	}

#include "linetemp.h"
}

/*
 * Draw a flat-shaded, Z-less, RGB line into an osmesa buffer.
 * (16 bpp version)
 */
static void flat_rgba_z_line2( GLcontext *ctx,
                                GLuint vert0, GLuint vert1, GLuint pvert )
{
/* Updated by David Boddie (Tue 01st June 1999, Sat 05th May 2001) */
   ROSMesaContext rosmesa = (ROSMesaContext) ctx;
   GLubyte *color = ctx->VB->ColorPtr->data[pvert];
   unsigned long pixel = ( ((color[0]*31)/255) & 0x1f) | (( ((color[1]*31)/255) & 0x1f) << 5)  | (( ((color[2]*31)/255) & 0x1f) << 10);

#ifdef DEBUG 
fprintf(stderr, "flat_rgba_z_line2\n");
#endif

#define INTERP_XY 1
#define INTERP_Z 1
#define DEPTH_TYPE DEFAULT_SOFTWARE_DEPTH_TYPE
#define CLIP_HACK 1
#define PLOT(X,Y)				\
	if (Z < *zPtr) {			\
	   GLubyte *ptr2 = PIXELADDR2(X,Y);	\
	   ptr2[0] = (pixel & 0xff);            \
	   ptr2[1] = ((pixel & 0xff00) >> 8);   \
	   *zPtr = Z;				\
	}

#include "linetemp.h"
}

/*
 * Draw a flat-shaded, Z-less, RGB line into an osmesa buffer.
 * (8 bpp version)
 */
static void flat_rgba_z_line1( GLcontext *ctx,
                                GLuint vert0, GLuint vert1, GLuint pvert )
{
/* Updated by David Boddie (Tue 01st June 1999, Sat 05th May 2001) */
   ROSMesaContext rosmesa = (ROSMesaContext) ctx;
   GLubyte *color = ctx->VB->ColorPtr->data[pvert];
   unsigned long pixel = rosmesa_colourtable[(color[0]>>4)][(color[1]>>4)][(color[2]>>4)][0];

/* Temporarily use only one pixel from the colour table - DB (Sat 20th June 1998). */

#ifdef DEBUG 
fprintf(stderr, "flat_rgba_z_line1\n");
#endif

#define INTERP_XY 1
#define INTERP_Z 1
#define DEPTH_TYPE DEFAULT_SOFTWARE_DEPTH_TYPE
#define CLIP_HACK 1
#define PLOT(X,Y)				\
	if (Z < *zPtr) {			\
	   GLubyte *ptr1 = PIXELADDR1(X,Y);	\
	   *ptr1 = pixel;			\
	   *zPtr = Z;				\
	}

#include "linetemp.h"
}

/*
 * Draw a flat-shaded, alpha-blended, RGB line into an osmesa buffer.
 */
static void flat_blend_rgba_line( GLcontext *ctx,
                                 GLuint vert0, GLuint vert1, GLuint pvert )
{
/* Updated by David Boddie (Tue 01st June 1999, Sat 05th May 2001) */
   ROSMesaContext rosmesa = (ROSMesaContext) ctx;
   struct vertex_buffer *VB = ctx->VB;
   GLint rshift = rosmesa->red_shift;
   GLint gshift = rosmesa->green_shift;
   GLint bshift = rosmesa->blue_shift;
   GLint avalue = VB->ColorPtr->data[pvert][3];
   GLint msavalue = 255 - avalue;
   GLint rvalue = VB->ColorPtr->data[pvert][0]*avalue;
   GLint gvalue = VB->ColorPtr->data[pvert][1]*avalue;
   GLint bvalue = VB->ColorPtr->data[pvert][2]*avalue;

#ifdef DEBUG 
fprintf(stderr, "flat_blend_rgba_line\n");
#endif

#define INTERP_XY 1
#define CLIP_HACK 1
#define PLOT(X,Y)					\
   { GLuint *ptr4 = PIXELADDR4(X,Y); \
     GLuint  pixel = 0; \
     pixel |=((((((*ptr4) >> rshift) & 0xff)*msavalue+rvalue)>>8) << rshift);\
     pixel |=((((((*ptr4) >> gshift) & 0xff)*msavalue+gvalue)>>8) << gshift);\
     pixel |=((((((*ptr4) >> bshift) & 0xff)*msavalue+bvalue)>>8) << bshift);\
     *ptr4 = pixel; \
   }

#include "linetemp.h"
}

/*
 * Draw a flat-shaded, Z-less, alpha-blended, RGB line into an osmesa buffer.
 */
static void flat_blend_rgba_z_line( GLcontext *ctx,
                                   GLuint vert0, GLuint vert1, GLuint pvert )
{
/* Updated by David Boddie (Sat 05th May 2001) */
   ROSMesaContext rosmesa = (ROSMesaContext) ctx;
   struct vertex_buffer *VB = ctx->VB;
   GLint rshift = rosmesa->red_shift;
   GLint gshift = rosmesa->green_shift;
   GLint bshift = rosmesa->blue_shift;
   GLint avalue = VB->ColorPtr->data[pvert][3];
   GLint msavalue = 256 - avalue;
   GLint rvalue = VB->ColorPtr->data[pvert][0]*avalue;
   GLint gvalue = VB->ColorPtr->data[pvert][1]*avalue;
   GLint bvalue = VB->ColorPtr->data[pvert][2]*avalue;

#ifdef DEBUG 
fprintf(stderr, "flat_blend_rgba_z_line\n");
#endif

#define INTERP_XY 1
#define INTERP_Z 1
#define DEPTH_TYPE DEFAULT_SOFTWARE_DEPTH_TYPE
#define CLIP_HACK 1
#define PLOT(X,Y)				\
	if (Z < *zPtr) {			\
   { GLuint *ptr4 = PIXELADDR4(X,Y); \
     GLuint  pixel = 0; \
     pixel |=((((((*ptr4) >> rshift) & 0xff)*msavalue+rvalue)>>8) << rshift);\
     pixel |=((((((*ptr4) >> gshift) & 0xff)*msavalue+gvalue)>>8) << gshift);\
     pixel |=((((((*ptr4) >> bshift) & 0xff)*msavalue+bvalue)>>8) << bshift);\
     *ptr4 = pixel; \
   } \
	}

#include "linetemp.h"
}

/*
 * Draw a flat-shaded, Z-less, alpha-blended, RGB line into an osmesa buffer.
 */
static void flat_blend_rgba_z_line_write( GLcontext *ctx,
                                   GLuint vert0, GLuint vert1, GLuint pvert )
{
/* Updated by David Boddie (Sat 05th May 2001) */
   ROSMesaContext rosmesa = (ROSMesaContext) ctx;
   struct vertex_buffer *VB = ctx->VB;
   GLint rshift = rosmesa->red_shift;
   GLint gshift = rosmesa->green_shift;
   GLint bshift = rosmesa->blue_shift;
   GLint avalue = VB->ColorPtr->data[pvert][3];
   GLint msavalue = 256 - avalue;
   GLint rvalue = VB->ColorPtr->data[pvert][0]*avalue;
   GLint gvalue = VB->ColorPtr->data[pvert][1]*avalue;
   GLint bvalue = VB->ColorPtr->data[pvert][2]*avalue;

#ifdef DEBUG 
fprintf(stderr, "flat_blend_rgba_z_line_write\n");
#endif

#define INTERP_XY 1
#define INTERP_Z 1
#define DEPTH_TYPE DEFAULT_SOFTWARE_DEPTH_TYPE
#define CLIP_HACK 1
#define PLOT(X,Y)				\
	if (Z < *zPtr) {			\
   { GLuint *ptr4 = PIXELADDR4(X,Y); \
     GLuint  pixel = 0; \
     pixel |=((((((*ptr4) >> rshift) & 0xff)*msavalue+rvalue)>>8) << rshift);\
     pixel |=((((((*ptr4) >> gshift) & 0xff)*msavalue+gvalue)>>8) << gshift);\
     pixel |=((((((*ptr4) >> bshift) & 0xff)*msavalue+bvalue)>>8) << bshift);\
     *ptr4 = pixel; \
   } \
	   *zPtr = Z;				\
	}

#include "linetemp.h"
}


/*
 * Analyze context state to see if we can provide a fast line drawing
 * function, like those in lines.c.  Otherwise, return NULL.
 */
static line_func choose_line_function( GLcontext *ctx )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx;

   if (ctx->Line.SmoothFlag)              return NULL;
   if (ctx->Texture.ReallyEnabled)              return NULL;
   if (ctx->Light.ShadeModel!=GL_FLAT)    return NULL;

   if (ctx->Line.Width==1.0F
       && ctx->Line.StippleFlag==GL_FALSE) {

       if (ctx->RasterMask==DEPTH_BIT
           && ctx->Depth.Func==GL_LESS
           && ctx->Depth.Mask==GL_TRUE) {
           switch(rosmesa->format) {
       		case ROSMESA_RGBA:
       		case ROSMESA_BGRA:
       		case ROSMESA_ARGB:
       			return flat_rgba_z_line;
       			break;
       		case ROSMESA_RGB:
       			switch (rosmesa->bpp)
       			{
       				case 16:
       					return flat_rgba_z_line2;
		       			break;
		       		case 8:
		       			return flat_rgba_z_line1;
       					break;
       				default:
       					return NULL;
       					break;
                        }
                        break;
       		default:
       			return NULL;
       			break;
           }
       }

       if (ctx->RasterMask==0) {
           switch(rosmesa->format) {
       		case ROSMESA_RGBA:
       		case ROSMESA_BGRA:
       		case ROSMESA_ARGB:
       			return flat_rgba_line;
       			break;
       		case ROSMESA_RGB:
       			switch (rosmesa->bpp)
       			{
       				case 16:
		       			return flat_rgba_line2;
		       			break;
		       		case 8:
		       			return flat_rgba_line1;
       					break;
       				default:
       					return NULL;
       					break;
                        }
                        break;
       		default:
       			return NULL;
       			break;
           }
       }

       if (ctx->RasterMask==(DEPTH_BIT|BLEND_BIT)
           && ctx->Depth.Func==GL_LESS
           && ctx->Depth.Mask==GL_TRUE
           && ctx->Color.BlendSrcRGB==GL_SRC_ALPHA
           && ctx->Color.BlendDstRGB==GL_ONE_MINUS_SRC_ALPHA
           && ctx->Color.BlendSrcA==GL_SRC_ALPHA
           && ctx->Color.BlendDstA==GL_ONE_MINUS_SRC_ALPHA
           && ctx->Color.BlendEquation==GL_FUNC_ADD_EXT) {
           switch(rosmesa->format) {
       		case ROSMESA_RGBA:
       		case ROSMESA_BGRA:
       		case ROSMESA_ARGB:
       			return flat_blend_rgba_z_line_write;
       			break;
       		default:
       			return NULL;
       			break;
           }
       }

       if (ctx->RasterMask==(DEPTH_BIT|BLEND_BIT)
           && ctx->Depth.Func==GL_LESS
           && ctx->Depth.Mask==GL_FALSE
           && ctx->Color.BlendSrcRGB==GL_SRC_ALPHA
           && ctx->Color.BlendDstRGB==GL_ONE_MINUS_SRC_ALPHA
           && ctx->Color.BlendSrcA==GL_SRC_ALPHA
           && ctx->Color.BlendDstA==GL_ONE_MINUS_SRC_ALPHA
           && ctx->Color.BlendEquation==GL_FUNC_ADD_EXT) {
           switch(rosmesa->format) {
       		case ROSMESA_RGBA:
       		case ROSMESA_BGRA:
       		case ROSMESA_ARGB:
       			return flat_blend_rgba_z_line;
       			break;
       		default:
       			return NULL;
       			break;
           }
       }

       if (ctx->RasterMask==BLEND_BIT
           && ctx->Color.BlendSrcRGB==GL_SRC_ALPHA
           && ctx->Color.BlendDstRGB==GL_ONE_MINUS_SRC_ALPHA
           && ctx->Color.BlendSrcA==GL_SRC_ALPHA
           && ctx->Color.BlendDstA==GL_ONE_MINUS_SRC_ALPHA
           && ctx->Color.BlendEquation==GL_FUNC_ADD_EXT) {
           switch(rosmesa->format) {
       		case ROSMESA_RGBA:
       		case ROSMESA_BGRA:
       		case ROSMESA_ARGB:
       			return flat_blend_rgba_line;
       			break;
       		default:
       			return NULL;
       			break;
           }
       }

   }
   return NULL;
}


/**********************************************************************/
/*****                 Optimized triangle rendering               *****/
/**********************************************************************/


/*
 * Smooth-shaded, z-less triangle, RGBA color.
 */
static void smooth_rgba_z_triangle( GLcontext *ctx, GLuint v0, GLuint v1,
                                     GLuint v2, GLuint pv )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx;
   GLint rshift = rosmesa->red_shift;
   GLint gshift = rosmesa->green_shift;
   GLint bshift = rosmesa->blue_shift;
   GLint ashift = rosmesa->alpha_shift;
   (void) pv;

#ifdef DEBUG 
fprintf(stderr, "smooth_rgba_z_triangle\n");
#endif

#define INTERP_Z 1
#define DEPTH_TYPE DEFAULT_SOFTWARE_DEPTH_TYPE
#define INTERP_RGB 1
#define INTERP_ALPHA 1
#define INNER_LOOP( LEFT, RIGHT, Y )				\
{								\
   GLint i, len = RIGHT-LEFT;					\
   GLuint *img = PIXELADDR4(LEFT,Y);   				\
   for (i=0;i<len;i++,img++) {					\
      GLdepth z = FixedToDepth(ffz);				\
      if (z < zRow[i]) {					\
         *img = PACK_RGBA2( FixedToInt(ffr), FixedToInt(ffg),	\
		            FixedToInt(ffb), FixedToInt(ffa) );	\
         zRow[i] = z;						\
      }								\
      ffr += fdrdx;  ffg += fdgdx;  ffb += fdbdx;  ffa += fdadx;\
      ffz += fdzdx;						\
   }								\
}
#include "tritemp.h"
}




/*
 * Flat-shaded, z-less triangle, RGBA color.
 */
static void flat_rgba_z_triangle( GLcontext *ctx, GLuint v0, GLuint v1,
                                   GLuint v2, GLuint pv )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx;
#define INTERP_Z 1
#define DEPTH_TYPE DEFAULT_SOFTWARE_DEPTH_TYPE
#define SETUP_CODE			\
   GLubyte r = VB->ColorPtr->data[pv][0];	\
   GLubyte g = VB->ColorPtr->data[pv][1];	\
   GLubyte b = VB->ColorPtr->data[pv][2];	\
   GLubyte a = VB->ColorPtr->data[pv][3];	\
   GLuint pixel = PACK_RGBA(r,g,b,a);

#ifdef DEBUG
fprintf(stderr, "flat_rgba_z_triangle\n");
#endif

#define INNER_LOOP( LEFT, RIGHT, Y )	\
{					\
   GLint i, len = RIGHT-LEFT;		\
   GLuint *img = PIXELADDR4(LEFT,Y);   	\
   for (i=0;i<len;i++,img++) {		\
      GLdepth z = FixedToDepth(ffz);	\
      if (z < zRow[i]) {		\
         *img = pixel;			\
         zRow[i] = z;			\
      }					\
      ffz += fdzdx;			\
   }					\
}
#include "tritemp.h"
}



/*
 * Return pointer to an accelerated triangle function if possible.
 */
static triangle_func choose_triangle_function( GLcontext *ctx )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx;

   if (rosmesa->format!=ROSMESA_RGBA) return NULL;

   if (ctx->Polygon.SmoothFlag)     return NULL;
   if (ctx->Polygon.StippleFlag)    return NULL;
   if (ctx->Texture.ReallyEnabled)        return NULL;

   if (ctx->RasterMask==DEPTH_BIT
       && ctx->Depth.Func==GL_LESS
       && ctx->Depth.Mask==GL_TRUE
       && rosmesa->format==ROSMESA_RGBA) {
      if (ctx->Light.ShadeModel==GL_SMOOTH) {
         return smooth_rgba_z_triangle;
      }
      else {
         return flat_rgba_z_triangle;
      }
   }
   return NULL;
}



static const GLubyte *get_string( GLcontext *ctx, GLenum name )
{
   (void) ctx;
   switch (name) {
      case GL_RENDERER:
         return (const GLubyte *) "Mesa RISC OS";
      default:
         return NULL;
   }
}


static void rosmesa_update_state( GLcontext *ctx )
{
   ROSMesaContext rosmesa = (ROSMesaContext) ctx;

   ASSERT((void *) rosmesa == (void *) ctx->DriverCtx);

   ctx->Driver.GetString = get_string;
   ctx->Driver.UpdateState = rosmesa_update_state;

   ctx->Driver.SetDrawBuffer = set_draw_buffer;
   ctx->Driver.SetReadBuffer = set_read_buffer;
   ctx->Driver.Color = set_color;
   ctx->Driver.Index = set_index;
   ctx->Driver.ClearIndex = clear_index;
   ctx->Driver.ClearColor = clear_color;
   ctx->Driver.Dither = dither;

   if (rosmesa->dither)
   {
   	ctx->Driver.Clear = clear_dither;
   }
   else
   {
   	ctx->Driver.Clear = clear;
   }

   ctx->Driver.GetBufferSize = buffer_size;

   ctx->Driver.PointsFunc = NULL;
   ctx->Driver.LineFunc = choose_line_function( ctx );
   ctx->Driver.TriangleFunc = choose_triangle_function( ctx );

   /* RGB(A) span/pixel functions */
   switch (rosmesa->format)
   {
	case ROSMESA_RGB:
		if (rosmesa->bpp == 8)
		{
				if (rosmesa->dither == GL_TRUE)
				{
					ctx->Driver.WriteRGBASpan = write_rgba_span1_dither;
					ctx->Driver.WriteRGBSpan = write_rgb_span1_dither;
					ctx->Driver.WriteRGBAPixels = write_rgba_pixels1_dither;
					ctx->Driver.WriteMonoRGBASpan = write_monocolor_span1_dither;
					ctx->Driver.WriteMonoRGBAPixels = write_monocolor_pixels1_dither;
					ctx->Driver.ReadRGBASpan = read_rgba_span1_dither;
					ctx->Driver.ReadRGBAPixels = read_rgba_pixels1_dither;
				}
				else
				{
					ctx->Driver.WriteRGBASpan = write_rgba_span1;
					ctx->Driver.WriteRGBSpan = write_rgb_span1;
					ctx->Driver.WriteRGBAPixels = write_rgba_pixels1;
					ctx->Driver.WriteMonoRGBASpan = write_monocolor_span1;
					ctx->Driver.WriteMonoRGBAPixels = write_monocolor_pixels1;
					ctx->Driver.ReadRGBASpan = read_rgba_span1;
					ctx->Driver.ReadRGBAPixels = read_rgba_pixels1;
				}
		}
		if (rosmesa->bpp == 16)
		{
				if (rosmesa->dither == GL_TRUE)
				{
					ctx->Driver.WriteRGBASpan = write_rgba_span2_dither;
					ctx->Driver.WriteRGBSpan = write_rgb_span2_dither;
					ctx->Driver.WriteRGBAPixels = write_rgba_pixels2_dither;
					ctx->Driver.WriteMonoRGBASpan = write_monocolor_span2_dither;
					ctx->Driver.WriteMonoRGBAPixels = write_monocolor_pixels2_dither;
					ctx->Driver.ReadRGBASpan = read_rgba_span2_dither;
					ctx->Driver.ReadRGBAPixels = read_rgba_pixels2_dither;
				}
				else
				{
					ctx->Driver.WriteRGBASpan = write_rgba_span2;
					ctx->Driver.WriteRGBSpan = write_rgb_span2;
					ctx->Driver.WriteRGBAPixels = write_rgba_pixels2;
					ctx->Driver.WriteMonoRGBASpan = write_monocolor_span2;
					ctx->Driver.WriteMonoRGBAPixels = write_monocolor_pixels2;
					ctx->Driver.ReadRGBASpan = read_rgba_span2;
					ctx->Driver.ReadRGBAPixels = read_rgba_pixels2;
				}
		}
		if ((rosmesa->bpp != 8) && (rosmesa->bpp != 16))
		{
				/* Should return error */
				gl_error(ctx, GL_INVALID_VALUE, "Bad number of bits per pixel for RGB mode (should be 8 or 16).");
		}
		break;

	case ROSMESA_RGBA:
		if (rosmesa->bpp == 32)
		{
			ctx->Driver.WriteRGBASpan = write_rgba_span;
			ctx->Driver.WriteRGBSpan = write_rgb_span;
			ctx->Driver.WriteRGBAPixels = write_rgba_pixels;
			ctx->Driver.WriteMonoRGBASpan = write_monocolor_span;
			ctx->Driver.WriteMonoRGBAPixels = write_monocolor_pixels;
			ctx->Driver.ReadRGBASpan = read_rgba_span;
			ctx->Driver.ReadRGBAPixels = read_rgba_pixels;
		}
		else
		{
			/* Should report error */
			gl_error(ctx, GL_INVALID_VALUE, "Bad number of bits per pixel for RGBA mode (should be 32).");
		}
		break;
	default:
		/* No default */
		gl_error(ctx, GL_INVALID_VALUE, "No rendering mode specified (RGB or RGBA).");
		break;
   }

   /* CI span/pixel functions */
/*
   ctx->Driver.WriteIndexSpan = write_index_span;
   ctx->Driver.WriteMonoindexSpan = write_monoindex_span;
   ctx->Driver.WriteIndexPixels = write_index_pixels;
   ctx->Driver.WriteMonoindexPixels = write_monoindex_pixels;
   ctx->Driver.ReadIndexSpan = read_index_span;
   ctx->Driver.ReadIndexPixels = read_index_pixels;
*/
}
