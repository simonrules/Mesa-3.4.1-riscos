/* $Id: rosmesa.h,v 0.3 1998/05/24 00:00:00 davidb Exp $ */

/*
 * Mesa 3-D graphics library
 * Version:  3.0
 * Copyright (C) 1995-1998  Brian Paul
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


/*
 * $Log: rosmesa.h $
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
 * rosmesa_setup_DD_function to reset the plotting function pointers for that
 * context.
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
 * by:      David Boddie
 * Tidied documentation within relevant files.
 *
 * Version: 0.1
 * Updated: Thu 22nd January 1998
 * by:      David Boddie
 * Now using Mesa version 2.5 instead of version 2.4.
 * Added 16 bpp support for some functions, and rudimentary 8 bpp support.
 *
 * Version: 0.01
 * Created: Mon 10th November 1997
 * Creator: David Boddie
 * Initial revision.
 * Derived from the off-screen renderer, "osmesa", and "svgamesa".
 * Very little added to the material on which it is based.
 */

/*
 * Acorn RISC OS Mesa renderer - renders to memory
 */

/*
 * RISC OS Mesa rendering interface.
 *
 * This is an interface to a renderer similar to the off-screen renderer,
 * "osmesa". It's purpose is to allow rendering to RISC OS format bitmaps,
 * which include screen banks.
 *
 * The following is taken from the off-screen renderer interface description:
 *
 * These are the API functions:
 *   ROSMesaCreateContext - create a new Off-Screen Mesa rendering context
 *   ROSMesaMakeCurrent - bind an OSMesaContext to a client's image buffer
 *                       and make the specified context the current one.
 *   ROSMesaDestroyContext - destroy an OSMesaContext
 *   ROSMesaGetCurrentContext - return thread's current context ID
 *   ROSMesaPixelStore - controls how pixels are stored in image buffer
 *   ROSMesaGetIntegerv - return OSMesa state parameters
 *
 * [Not sure about the ROSMesaPixelStore function - DB]
 *
 *
 * The limits on the width and height of an image buffer are MAX_WIDTH and
 * MAX_HEIGHT as defined in Mesa/src/config.h.  Defaults are 1280 and 1024.
 * You can increase them as needed but beware that many temporary arrays in
 * Mesa are dimensioned by MAX_WIDTH or MAX_WIDTH.
 */




#ifndef ROSMESA_H
#define ROSMESA_H



#ifdef __cplusplus
extern "C" {
#endif


#include "GL/gl.h"



#define ROSMESA_MAJOR_VERSION 0
#define ROSMESA_MINOR_VERSION 36



/*
 * Values for the format parameter of ROSMesaCreateContext()
 * New in version 2.0 of OSMesa.
 */
#define ROSMESA_COLOR_INDEX	GL_COLOR_INDEX
#define ROSMESA_RGBA		GL_RGBA
#define ROSMESA_BGRA		0x1
#define ROSMESA_ARGB		0x2
#define ROSMESA_RGB		GL_RGB
#define ROSMESA_BGR		0x4


/*
 * ROSMesaPixelStore() parameters:
 * New in version 2.0.
 */
#define ROSMESA_ROW_LENGTH	0x10
#define ROSMESA_Y_UP		0x11


/*
 * Accepted by ROSMesaGetIntegerv:
 */
#define ROSMESA_WIDTH		0x20
#define ROSMESA_HEIGHT		0x21
#define ROSMESA_FORMAT		0x22
#define ROSMESA_TYPE		0x23



typedef struct rosmesa_context *ROSMesaContext;

/*
 * Create a RISC OS Mesa rendering context.  The only attribute needed is
 * an RGBA vs Color-Index mode flag.
 *
 * Input:  format	- either ROSMESA_RGB or ROSMESA_RGBA
 *         bpp		- 8, 16 for RGB, 32 for RGBA
 *         doublebuffer	- GL_TRUE or GL_FALSE
 *
 * Return:  an ROSMesaContext or 0 if error
 */
GLAPI ROSMesaContext GLAPIENTRY ROSMesaCreateContext( GLenum format, GLuint bpp, GLboolean doublebuffer );




/*
 * Destroy a RISC OS rendering context.
 *
 * Input:  ctx - the context to destroy
 */
GLAPI void GLAPIENTRY ROSMesaDestroyContext( ROSMesaContext ctx );



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
 *         buffer2 - a second image buffer (to be used if double buffering is
 *                   selected)
 *         type - data type for pixel components, only GL_UNSIGNED_BYTE
 *                supported now
 *         width, height - size of image buffer in pixels, at least 1
 * Return:  GL_TRUE if success, GL_FALSE if error because of invalid ctx,
 *          invalid buffer address, type!=GL_UNSIGNED_BYTE, width<1, height<1,
 *          width>internal limit or height>internal limit.
 */
GLAPI GLboolean GLAPIENTRY ROSMesaMakeCurrent( ROSMesaContext ctx,
                                    void *buffer1, void *buffer2, GLenum type,
                                    GLsizei width, GLsizei height );




/*
 * Return the current RISC OS Mesa rendering context handle.
 */
GLAPI ROSMesaContext GLAPIENTRY ROSMesaGetCurrentContext( void );



/*
 * Set pixel store/packing parameters for the current context.
 * This is similar to glPixelStore.
 * Input:  pname - ROSMESA_ROW_LENGTH
 *                    specify actual pixels per row in image buffer
 *                    0 = same as image width (default)
 *                 ROSMESA_Y_UP
 *                    zero = Y coordinates increase downward
 *                    non-zero = Y coordinates increase upward (default)
 *         value - the value for the parameter pname
 *
 * New in version 2.0.
 */
GLAPI void GLAPIENTRY ROSMesaPixelStore( GLint pname, GLint value );



/*
 * Return context info.  This is like glGetIntegerv.
 * Input:  pname -
 *                 ROSMESA_WIDTH  return current image width
 *                 ROSMESA_HEIGHT  return current image height
 *                 ROSMESA_FORMAT  return image format
 *                 ROSMESA_TYPE  return color component data type
 *                 ROSMESA_ROW_LENGTH return row length in pixels
 *                 ROSMESA_Y_UP returns 1 or 0 to indicate Y axis direction
 *         value - pointer to integer in which to return result.
 */
GLAPI void GLAPIENTRY ROSMesaGetIntegerv( GLint pname, GLint *value );



/*
 * Return the depth buffer associated with an ROSMesa context.
 * Input:  c - the OSMesa context
 * Output:  width, height - size of buffer in pixels
 *          bytesPerValue - bytes per depth value (2 or 4)
 *          buffer - pointer to depth buffer values
 * Return:  GL_TRUE or GL_FALSE to indicate success or failure.
 *
 * New in Mesa 2.4.
 */
GLAPI GLboolean GLAPIENTRY ROSMesaGetDepthBuffer( ROSMesaContext c,
                                       GLint *width, GLint *height,
                                       GLint *bytesPerValue, void **buffer );


/*
 * Return the color buffer associated with an OSMesa context.
 * Input:  c - the OSMesa context
 * Output:  width, height - size of buffer in pixels
 *          format - buffer format (OSMESA_FORMAT)
 *          buffer - pointer to depth buffer values
 * Return:  GL_TRUE or GL_FALSE to indicate success or failure.
 *
 * New in Mesa 3.3 (OSMesa); new in Mesa 3.4.1 (ROSMesa).
 */
GLAPI GLboolean GLAPIENTRY ROSMesaGetColorBuffer( ROSMesaContext c,
                                                 GLint *width, GLint *height,
                                                 GLint *format,
                                                 void **buffer );


/* Added Tue 16th June 1998 to allow some form of bank switching. */
/* Modified Wed 17th June 1998 to tidy it up. */

GLAPI void GLAPIENTRY ROSMesaSwapBuffers ( ROSMesaContext ctx );

#ifdef __cplusplus
}
#endif


#endif
