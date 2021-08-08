/*
 * Mesa 3-D graphics library
 * Version:  3.1
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
 * $Log: xform.c,v $
 *
 * Note           1999/02/13 18:52:00  davidb
 * Started to move functions to RISCOS/xform_asm.s
 *
 * Revision 3.11  1998/11/08 22:37:53  brianp
 * fixed a typo
 *
 * Revision 3.10  1998/11/07 14:21:55  brianp
 * replaced static array initialization with gl_init_transformation()
 *
 * Revision 3.9  1998/11/07 13:40:07  brianp
 * added a bunch of const and volatile qualifiers
 *
 * Revision 3.8  1998/11/01 20:20:29  brianp
 * updated with Josh's new x86 code
 *
 * Revision 3.7  1998/10/29 04:10:21  brianp
 * removed dead code, added placeholders for 3D-Now! code
 *
 * Revision 3.6  1998/10/29 03:57:11  brianp
 * misc clean-up of new vertex transformation code
 *
 * Revision 3.5  1998/10/29 02:28:13  brianp
 * incorporated Keith Whitwell's transformation optimizations
 *
 * Revision 3.4  1998/09/25 03:12:27  brianp
 * simplified gl_xform_normals_3fv() per Keith Whitwell
 *
 * Revision 3.3  1998/08/20 04:15:48  brianp
 * added prototype 3.1 transformation functions
 *
 * Revision 3.2  1998/04/18 05:00:56  brianp
 * renamed USE_ASM to USE_X86_ASM
 *
 * Revision 3.1  1998/02/01 16:37:19  brianp
 * added GL_EXT_rescale_normal extension
 *
 * Revision 3.0  1998/01/31 21:08:31  brianp
 * initial rev
 *
 */


/*
 * Matrix/vertex/vector transformation stuff
 *
 *
 * NOTES:
 * 1. 4x4 transformation matrices are stored in memory in column major order.
 * 2. Points/vertices are to be thought of as column vectors.
 * 3. Transformation of a point p by a matrix M is: p' = M * p
 *
 * The volatile keyword is needed in a bunch of places to prevent
 * numerical problems on x86 CPUs.
 */


#ifdef PC_HEADER
#include "all.h"
#else
#include <math.h>
#include "asm_386.h"
#include "vb.h"
#include "context.h"
#include "mmath.h"
#include "types.h"
#include "xform.h"
#endif


/*
 * Transform a 4-element row vector (1x4 matrix) by a 4x4 matrix.  This
 * function is used for transforming clipping plane equations and spotlight
 * directions.
 * Mathematically,  u = v * m.
 * Input:  v - input vector
 *         m - transformation matrix
 * Output:  u - transformed vector
 */
/*
void gl_transform_vector( GLfloat u[4], const GLfloat v[4], const GLfloat m[16] )
{
   GLfloat v0=v[0], v1=v[1], v2=v[2], v3=v[3];
#define M(row,col)  m[col*4+row]
   u[0] = v0 * M(0,0) + v1 * M(1,0) + v2 * M(2,0) + v3 * M(3,0);
   u[1] = v0 * M(0,1) + v1 * M(1,1) + v2 * M(2,1) + v3 * M(3,1);
   u[2] = v0 * M(0,2) + v1 * M(1,2) + v2 * M(2,2) + v3 * M(3,2);
   u[3] = v0 * M(0,3) + v1 * M(1,3) + v2 * M(2,3) + v3 * M(3,3);
#undef M
}
*/

#ifndef USE_X86_ASM
/*
 * Apply a transformation matrix to an array of normal vectors:
 *   for i in 0 to n-1 do  v[i] = u[i] * m
 * where u[i] and v[i] are 3-element row vectors and m is a 16-element
 * transformation matrix.
 * If the normalize flag is true the normals will be scaled to length 1.
 * If the rescale flag is true then do normal rescaling.
 */
void gl_transform_normals_3fv( GLuint n,
			       const GLfloat *in,
			       GLuint in_stride,
			       const GLfloat m[16],
			       GLfloat out[][3],
			       GLboolean normalize,
			       GLboolean rescale)
{
   if (normalize) {
      /* Transform and normalize.
       * KW: rescale is a noop under these conditions...
       */
      GLuint i;
      GLfloat m0 = m[0],  m4 = m[4],  m8 = m[8];
      GLfloat m1 = m[1],  m5 = m[5],  m9 = m[9];
      GLfloat m2 = m[2],  m6 = m[6],  m10 = m[10];
      for (i = 0 ; i < n ; i++, in += in_stride) {
         GLdouble tx, ty, tz;
         {
            GLfloat ux = in[0],  uy = in[1],  uz = in[2];
            tx = ux * m0 + uy * m1 + uz * m2;
            ty = ux * m4 + uy * m5 + uz * m6;
            tz = ux * m8 + uy * m9 + uz * m10;
         }
         {
            GLdouble len, scale;
            len = GL_SQRT( tx*tx + ty*ty + tz*tz );
            scale = (len>1E-30) ? (1.0 / len) : 1.0;
            out[i][0] = tx * scale;
            out[i][1] = ty * scale;
            out[i][2] = tz * scale;
         }
      }
   }
   else if (rescale) {
      /* Transform and rescale */
      GLuint i;
      GLfloat m0 = m[0],  m4 = m[4],  m8 = m[8];
      GLfloat m1 = m[1],  m5 = m[5],  m9 = m[9];
      GLfloat m2 = m[2],  m6 = m[6],  m10 = m[10];
      GLfloat f = GL_SQRT( m2*m2 + m6*m6 + m10*m10 ); /* precompute */
      f = (f == 0.0F) ? 1.0F : (1.0F / f);
      for (i = 0 ; i < n ; i++, in += in_stride) {
	 GLfloat ux = in[0],  uy = in[1],  uz = in[2];
         out[i][0] = f * (ux * m0 + uy * m1 + uz * m2);
         out[i][1] = f * (ux * m4 + uy * m5 + uz * m6);
         out[i][2] = f * (ux * m8 + uy * m9 + uz * m10);
      }
   }
   else {
      /* Just transform */
      GLuint i;
      GLfloat m0 = m[0],  m4 = m[4],  m8 = m[8];
      GLfloat m1 = m[1],  m5 = m[5],  m9 = m[9];
      GLfloat m2 = m[2],  m6 = m[6],  m10 = m[10];
      for (i = 0 ; i < n ; i++, in += in_stride) {
	 GLfloat ux = in[0],  uy = in[1],  uz = in[2];
         out[i][0] = ux * m0 + uy * m1 + uz * m2;
         out[i][1] = ux * m4 + uy * m5 + uz * m6;
         out[i][2] = ux * m8 + uy * m9 + uz * m10;
      }
   }
}
#endif




/*----------------------------------------------------------------------
 * Begin Keith's new code
 *
 * NOTE:  the units for stride values is a GLfloat.  That is, if
 * stride = 3 then the start of each floating point coordinate is 12 bytes
 * after its predecessor.  Have to be clear about this because OpenGL's
 * vertex array stride values are in bytes!
 *
 *----------------------------------------------------------------------
 */

/*
void gl_normalize_3fv( GLuint n,
		       const GLfloat *in,
		       GLuint in_stride,
		       GLfloat out[][3] )
{
   GLuint i;
   for ( i = 0 ; i < n ; i++, in += in_stride ) {
      const GLfloat x = in[0], y = in[1], z = in[2];
      GLdouble len = x * x + y * y + z * z;
      if (len > 1e-50) {
         len = 1.0 / GL_SQRT(len);
         out[i][0] = x * len;
         out[i][1] = y * len;
         out[i][2] = z * len;
      }
      else {
         out[i][0] = x;
         out[i][1] = y;
         out[i][2] = z;
      }
   }
}


/*
void gl_scale_3fv( GLuint n,
		   const GLfloat *in,
		   GLuint in_stride,
		   GLfloat out[][3],
		   GLfloat scale)
{
   GLuint i;
   for ( i = 0 ; i < n ; i++, in += in_stride ) {
      SCALE_SCALAR_3V( out[i], scale, in );
   }
}
*/


static void transform_points2_general( const GLmatrix *mat,
				       GLuint n,
				       const GLfloat *from,
				       GLfloat to[][4],
				       GLuint stride)
{
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0],  m4 = m[4],  m12 = m[12];
   const GLfloat m1 = m[1],  m5 = m[5],  m13 = m[13];
   const GLfloat m2 = m[2],  m6 = m[6],  m14 = m[14];
   const GLfloat m3 = m[3],  m7 = m[7],  m15 = m[15];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_GENERAL);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1];
      to[i][0] = m0 * ox + m4 * oy + m12;
      to[i][1] = m1 * ox + m5 * oy + m13;
      to[i][2] = m2 * ox + m6 * oy + m14;
      to[i][3] = m3 * ox + m7 * oy + m15;
   }
   END_FAST_MATH;
}

static void transform_points2_identity( const GLmatrix *mat,
					GLuint n,
					const GLfloat *from,
					GLfloat to[][4],
					GLuint stride )
{
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_IDENTITY);
   for (i=0;i<n;i++, from+=stride) {
      to[i][0] = from[0];
      to[i][1] = from[1];
      to[i][2] = 0.0F;
      to[i][3] = 1.0F;
   }
   END_FAST_MATH;
}

static void transform_points2_2d( const GLmatrix *mat,
				  GLuint n,
				  const GLfloat *from,
				  GLfloat to[][4],
				  GLuint stride )
{
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0], m1 = m[1], m4 = m[4], m5 = m[5];
   const GLfloat m12 = m[12], m13 = m[13];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_2D);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1];
      to[i][0] = m0 * ox + m4 * oy + m12;
      to[i][1] = m1 * ox + m5 * oy + m13;
      to[i][2] = 0.0F;
      to[i][3] = 1.0F;
   }
   END_FAST_MATH;
}

static void transform_points2_2d_no_rot( const GLmatrix *mat,
					 GLuint n,
					 const GLfloat *from,
					 GLfloat to[][4],
					 GLuint stride )
{
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0], m5 = m[5], m12 = m[12], m13 = m[13];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_2D_NO_ROT);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1];
      to[i][0] = m0 * ox           + m12;
      to[i][1] =           m5 * oy + m13;
      to[i][2] = 0.0F;
      to[i][3] = 1.0F;
   }
   END_FAST_MATH;
}

static void transform_points2_3d( const GLmatrix *mat,
				  GLuint n,
				  const GLfloat *from,
				  GLfloat to[][4],
				  GLuint stride )
{
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0], m1 = m[1], m2 = m[2], m4 = m[4], m5 = m[5];
   const GLfloat m6 = m[6], m12 = m[12], m13 = m[13], m14 = m[14];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_3D);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1];
      to[i][0] = m0 * ox + m4 * oy + m12;
      to[i][1] = m1 * ox + m5 * oy + m13;
      to[i][2] = m2 * ox + m6 * oy + m14;
      to[i][3] = 1.0F;
   }
   END_FAST_MATH;
}

static void transform_points3_general( const GLmatrix *mat,
				       GLuint n,
				       const GLfloat *from,
				       GLfloat to[][4],
				       GLuint stride )
{

   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0],  m4 = m[4],  m8 = m[8],  m12 = m[12];
   const GLfloat m1 = m[1],  m5 = m[5],  m9 = m[9],  m13 = m[13];
   const GLfloat m2 = m[2],  m6 = m[6],  m10 = m[10],  m14 = m[14];
   const GLfloat m3 = m[3],  m7 = m[7],  m11 = m[11],  m15 = m[15];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_GENERAL);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2];
      to[i][0] = m0 * ox + m4 * oy + m8  * oz + m12;
      to[i][1] = m1 * ox + m5 * oy + m9  * oz + m13;
      to[i][2] = m2 * ox + m6 * oy + m10 * oz + m14;
      to[i][3] = m3 * ox + m7 * oy + m11 * oz + m15;
   }

   END_FAST_MATH;
}

static void transform_points3_identity( const GLmatrix *mat,
					GLuint n,
					const GLfloat *from,
					GLfloat to[][4],
					GLuint stride )
{
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_IDENTITY);
   for (i=0;i<n;i++, from+=stride) {
      to[i][0] = from[0];
      to[i][1] = from[1];
      to[i][2] = from[2];
      to[i][3] = 1.0F;
   }
   END_FAST_MATH;
}

static void transform_points3_2d( const GLmatrix *mat,
				  GLuint n,
				  const GLfloat *from,
				  GLfloat to[][4],
				  GLuint stride )
{
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0], m1 = m[1], m4 = m[4], m5 = m[5];
   const GLfloat m12 = m[12], m13 = m[13];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_2D);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2];
      to[i][0] = m0 * ox + m4 * oy            + m12       ;
      to[i][1] = m1 * ox + m5 * oy            + m13       ;
      to[i][2] =                   +       oz             ;
      to[i][3] =                                      1.0F;
   }
   END_FAST_MATH;
}

static void transform_points3_2d_no_rot( const GLmatrix *mat,
					 GLuint n,
					 const GLfloat *from,
					 GLfloat to[][4],
					 GLuint stride )
{
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0], m5 = m[5], m12 = m[12], m13 = m[13];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_2D_NO_ROT);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2];
      to[i][0] = m0 * ox                      + m12       ;
      to[i][1] =           m5 * oy            + m13       ;
      to[i][2] =                   +       oz             ;
      to[i][3] =                                      1.0F;
   }
   END_FAST_MATH;
}

static void transform_points3_3d( const GLmatrix *mat,
				  GLuint n,
				  const GLfloat *from,
				  GLfloat to[][4],
				  GLuint stride )
{
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0], m1 = m[1], m2 = m[2], m4 = m[4], m5 = m[5];
   const GLfloat m6 = m[6], m8 = m[8], m9 = m[9], m10 = m[10];
   const GLfloat m12 = m[12], m13 = m[13], m14 = m[14];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_3D);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2];
      to[i][0] = m0 * ox + m4 * oy +  m8 * oz + m12       ;
      to[i][1] = m1 * ox + m5 * oy +  m9 * oz + m13       ;
      to[i][2] = m2 * ox + m6 * oy + m10 * oz + m14       ;
      to[i][3] =                                      1.0F;
   }
   END_FAST_MATH;
}

/* previously known as ortho...
 */
static void transform_points3_3d_no_rot( const GLmatrix *mat,
					 GLuint n,
					 const GLfloat *from,
					 GLfloat to[][4],
					 GLuint stride )
{
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0], m5 = m[5];
   const GLfloat m10 = m[10], m12 = m[12], m13 = m[13], m14 = m[14];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_3D_NO_ROT);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2];
      to[i][0] = m0 * ox                      + m12       ;
      to[i][1] =           m5 * oy            + m13       ;
      to[i][2] =                     m10 * oz + m14       ;
      to[i][3] =                                      1.0F;
   }
   END_FAST_MATH;
}

static void transform_points3_perspective( const GLmatrix *mat,
					   GLuint n,
					   const GLfloat *from,
					   GLfloat to[][4],
					   GLuint stride )
{
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0], m5 = m[5], m8 = m[8], m9 = m[9];
   const GLfloat m10 = m[10], m14 = m[14];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_PERSPECTIVE);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2];
      to[i][0] = m0 * ox           + m8  * oz       ;
      to[i][1] =           m5 * oy + m9  * oz       ;
      to[i][2] =                     m10 * oz + m14 ;
      to[i][3] =                          -oz       ;
   }
   END_FAST_MATH;
}



static void transform_points4_general( const GLmatrix *mat,
				       GLuint n,
				       const GLfloat *from,
				       GLfloat to[][4],
				       GLuint stride )
{
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0],  m4 = m[4],  m8 = m[8],  m12 = m[12];
   const GLfloat m1 = m[1],  m5 = m[5],  m9 = m[9],  m13 = m[13];
   const GLfloat m2 = m[2],  m6 = m[6],  m10 = m[10],  m14 = m[14];
   const GLfloat m3 = m[3],  m7 = m[7],  m11 = m[11],  m15 = m[15];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_GENERAL);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2], ow = from[3];
      to[i][0] = m0 * ox + m4 * oy + m8  * oz + m12 * ow;
      to[i][1] = m1 * ox + m5 * oy + m9  * oz + m13 * ow;
      to[i][2] = m2 * ox + m6 * oy + m10 * oz + m14 * ow;
      to[i][3] = m3 * ox + m7 * oy + m11 * oz + m15 * ow;
   }
   END_FAST_MATH;
}

static void transform_points4_identity( const GLmatrix *mat,
					GLuint n,
					const GLfloat *from,
					GLfloat to[][4],
					GLuint stride )
{
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_IDENTITY);
   for (i=0;i<n;i++, from+=stride) {
      to[i][0] = from[0];
      to[i][1] = from[1];
      to[i][2] = from[2];
      to[i][3] = from[3];
   }
   END_FAST_MATH;
}

static void transform_points4_2d( const GLmatrix *mat,
				  GLuint n,
				  const GLfloat *from,
				  GLfloat to[][4],
				  GLuint stride )
{
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0], m1 = m[1], m4 = m[4], m5 = m[5];
   const GLfloat m12 = m[12], m13 = m[13];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_2D);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2], ow = from[3];
      to[i][0] = m0 * ox + m4 * oy            + m12 * ow;
      to[i][1] = m1 * ox + m5 * oy            + m13 * ow;
      to[i][2] =                   +       oz           ;
      to[i][3] =                                      ow;
   }
   END_FAST_MATH;
}

static void transform_points4_2d_no_rot( const GLmatrix *mat,
					 GLuint n,
					 const GLfloat *from,
					 GLfloat to[][4],
					 GLuint stride )
{
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0], m5 = m[5], m12 = m[12], m13 = m[13];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_2D_NO_ROT);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2], ow = from[3];
      to[i][0] = m0 * ox                      + m12 * ow;
      to[i][1] =           m5 * oy            + m13 * ow;
      to[i][2] =                   +       oz           ;
      to[i][3] =                                      ow;
   }
   END_FAST_MATH;
}

static void transform_points4_3d( const GLmatrix *mat,
				  GLuint n,
				  const GLfloat *from,
				  GLfloat to[][4],
				  GLuint stride )
{
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0], m1 = m[1], m2 = m[2], m4 = m[4], m5 = m[5];
   const GLfloat m6 = m[6], m8 = m[8], m9 = m[9], m10 = m[10];
   const GLfloat m12 = m[12], m13 = m[13], m14 = m[14];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_3D);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2], ow = from[3];
      to[i][0] = m0 * ox + m4 * oy +  m8 * oz + m12 * ow;
      to[i][1] = m1 * ox + m5 * oy +  m9 * oz + m13 * ow;
      to[i][2] = m2 * ox + m6 * oy + m10 * oz + m14 * ow;
      to[i][3] =                                      ow;
   }
   END_FAST_MATH;
}

static void transform_points4_3d_no_rot( const GLmatrix *mat,
					 GLuint n,
					 const GLfloat *from,
					 GLfloat to[][4],
					 GLuint stride )
{
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0], m5 = m[5];
   const GLfloat m10 = m[10], m12 = m[12], m13 = m[13], m14 = m[14];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_3D_NO_ROT);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2], ow = from[3];
      to[i][0] = m0 * ox                      + m12 * ow;
      to[i][1] =           m5 * oy            + m13 * ow;
      to[i][2] =                     m10 * oz + m14 * ow;
      to[i][3] =                                      ow;
   }
   END_FAST_MATH;
}

static void transform_points4_perspective( const GLmatrix *mat,
					   GLuint n,
					   const GLfloat *from,
					   GLfloat to[][4],
					   GLuint stride )
{
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0], m5 = m[5], m8 = m[8], m9 = m[9];
   const GLfloat m10 = m[10], m14 = m[14];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_PERSPECTIVE);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2], ow = from[3];
      to[i][0] = m0 * ox           + m8  * oz            ;
      to[i][1] =           m5 * oy + m9  * oz            ;
      to[i][2] =                     m10 * oz + m14 * ow ;
      to[i][3] =                          -oz            ;
   }
   END_FAST_MATH;
}



static void transform_cliptest_points3_3d_no_rot( const GLmatrix *mat,
                                                  GLuint n,
                                                  const GLfloat *from,
                                                  GLfloat to[][4],
                                                  GLuint stride,
                                                  GLubyte clipMask[],
                                                  GLubyte *orMask,
                                                  GLubyte *andMask )
{
   GLubyte tmpOrMask = *orMask;
   GLubyte tmpAndMask = *andMask;
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0],                          m12 = m[12];
   const GLfloat             m5 = m[5],              m13 = m[13];
   const GLfloat                         m10 = m[10],  m14 = m[14];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_3D_NO_ROT);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2];
      volatile GLfloat cx = m0 * ox                      + m12 ;
      volatile GLfloat cy =           m5 * oy            + m13 ;
      volatile GLfloat cz =                     m10 * oz + m14 ;
      GLubyte mask = 0;
      to[i][0] = cx;
      to[i][1] = cy;
      to[i][2] = cz;
      to[i][3] = 1.0F;
      if (cx >  1.0F)       mask |= CLIP_RIGHT_BIT;
      else if (cx < -1.0F)  mask |= CLIP_LEFT_BIT;
      if (cy >  1.0F)       mask |= CLIP_TOP_BIT;
      else if (cy < -1.0F)  mask |= CLIP_BOTTOM_BIT;
      if (cz >  1.0F)       mask |= CLIP_FAR_BIT;
      else if (cz < -1.0F)  mask |= CLIP_NEAR_BIT;
      if (mask) {
	 clipMask[i] |= mask;
	 tmpOrMask |= mask;
      }
      tmpAndMask &= mask;
   }
   *orMask = tmpOrMask;
   *andMask = tmpAndMask;
   END_FAST_MATH;
}


static void transform_cliptest_points3_perspective( const GLmatrix *mat,
                                                    GLuint n,
                                                    const GLfloat *from,
                                                    GLfloat to[][4],
                                                    GLuint stride,
                                                    GLubyte clipMask[],
                                                    GLubyte *orMask,
                                                    GLubyte *andMask )
{
   GLubyte tmpOrMask = *orMask;
   GLubyte tmpAndMask = *andMask;
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0],              m8 = m[8]              ;
   const GLfloat             m5 = m[5],  m9 = m[9]              ;
   const GLfloat                         m10 = m[10],  m14 = m[14];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_PERSPECTIVE);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2];
      volatile GLfloat cx = m0 * ox           + m8  * oz       ;
      volatile GLfloat cy =           m5 * oy + m9  * oz       ;
      volatile GLfloat cz =                     m10 * oz + m14 ;
      volatile GLfloat cw =                          -oz       ;
      GLubyte mask = 0;
      to[i][0] = cx;
      to[i][1] = cy;
      to[i][2] = cz;
      to[i][3] = cw;
      if (cx >  cw)       mask |= CLIP_RIGHT_BIT;
      else if (cx < -cw)  mask |= CLIP_LEFT_BIT;
      if (cy >  cw)       mask |= CLIP_TOP_BIT;
      else if (cy < -cw)  mask |= CLIP_BOTTOM_BIT;
      if (cz >  cw)       mask |= CLIP_FAR_BIT;
      else if (cz < -cw)  mask |= CLIP_NEAR_BIT;
      if (mask) {
	 clipMask[i] |= mask;
	 tmpOrMask |= mask;
      }
      tmpAndMask &= mask;
   }
   *orMask = tmpOrMask;
   *andMask = tmpAndMask;
   END_FAST_MATH;
}

static void transform_cliptest_points3_general( const GLmatrix *mat,
                                                GLuint n,
                                                const GLfloat *from,
                                                GLfloat to[][4],
                                                GLuint stride,
                                                GLubyte clipMask[],
                                                GLubyte *orMask,
                                                GLubyte *andMask )
{
   GLubyte tmpOrMask = *orMask;
   GLubyte tmpAndMask = *andMask;
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0],  m4 = m[4],  m8 = m[8],  m12 = m[12];
   const GLfloat m1 = m[1],  m5 = m[5],  m9 = m[9],  m13 = m[13];
   const GLfloat m2 = m[2],  m6 = m[6],  m10 = m[10],  m14 = m[14];
   const GLfloat m3 = m[3],  m7 = m[7],  m11 = m[11],  m15 = m[15];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_GENERAL);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2];
      volatile GLfloat cx = m0 * ox + m4 * oy + m8  * oz + m12;
      volatile GLfloat cy = m1 * ox + m5 * oy + m9  * oz + m13;
      volatile GLfloat cz = m2 * ox + m6 * oy + m10 * oz + m14;
      volatile GLfloat cw = m3 * ox + m7 * oy + m11 * oz + m15;
      GLubyte mask = 0;
      to[i][0] = cx;
      to[i][1] = cy;
      to[i][2] = cz;
      to[i][3] = cw;
      if (cx >  cw)       mask |= CLIP_RIGHT_BIT;
      else if (cx < -cw)  mask |= CLIP_LEFT_BIT;
      if (cy >  cw)       mask |= CLIP_TOP_BIT;
      else if (cy < -cw)  mask |= CLIP_BOTTOM_BIT;
      if (cz >  cw)       mask |= CLIP_FAR_BIT;
      else if (cz < -cw)  mask |= CLIP_NEAR_BIT;
      if (mask) {
	 clipMask[i] |= mask;
	 tmpOrMask |= mask;
      }
      tmpAndMask &= mask;
   }
   *orMask = tmpOrMask;
   *andMask = tmpAndMask;
   END_FAST_MATH;
}

static void transform_cliptest_points4_perspective( const GLmatrix *mat,
                                                    GLuint n,
                                                    const GLfloat *from,
                                                    GLfloat to[][4],
                                                    GLuint stride,
                                                    GLubyte clipMask[],
                                                    GLubyte *orMask,
                                                    GLubyte *andMask )
{
   GLubyte tmpOrMask = *orMask;
   GLubyte tmpAndMask = *andMask;
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0],              m8 = m[8]              ;
   const GLfloat             m5 = m[5],  m9 = m[9]              ;
   const GLfloat                         m10 = m[10],  m14 = m[14];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_PERSPECTIVE);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2], ow = from[3];
      volatile GLfloat cx = m0 * ox +           m8  * oz           ;
      volatile GLfloat cy =           m5 * oy + m9  * oz           ;
      volatile GLfloat cz =                     m10 * oz + m14 * ow;
      volatile GLfloat cw =                          -oz           ;
      GLubyte mask = 0;
      to[i][0] = cx;
      to[i][1] = cy;
      to[i][2] = cz;
      to[i][3] = cw;
      if (cx >  cw)       mask |= CLIP_RIGHT_BIT;
      else if (cx < -cw)  mask |= CLIP_LEFT_BIT;
      if (cy >  cw)       mask |= CLIP_TOP_BIT;
      else if (cy < -cw)  mask |= CLIP_BOTTOM_BIT;
      if (cz >  cw)       mask |= CLIP_FAR_BIT;
      else if (cz < -cw)  mask |= CLIP_NEAR_BIT;
      if (mask) {
	 clipMask[i] |= mask;
	 tmpOrMask |= mask;
      }
      tmpAndMask &= mask;
   }
   *orMask = tmpOrMask;
   *andMask = tmpAndMask;
   END_FAST_MATH;
}

static void transform_cliptest_points4_3d_no_rot( const GLmatrix *mat,
                                                  GLuint n,
                                                  const GLfloat *from,
                                                  GLfloat to[][4],
                                                  GLuint stride,
                                                  GLubyte clipMask[],
                                                  GLubyte *orMask,
                                                  GLubyte *andMask )
{
   GLubyte tmpOrMask = *orMask;
   GLubyte tmpAndMask = *andMask;
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0],                           m12 = m[12];
   const GLfloat             m5 = m[5],               m13 = m[13];
   const GLfloat                        m10 = m[10],  m14 = m[14];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_3D_NO_ROT);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2], ow = from[3];
      volatile GLfloat cx = m0 * ox +                      m12 * ow;
      volatile GLfloat cy =           m5 * oy +            m13 * ow;
      volatile GLfloat cz =                     m10 * oz + m14 * ow;
      volatile GLfloat cw =                                      ow;
      GLubyte mask = 0;
      to[i][0] = cx;
      to[i][1] = cy;
      to[i][2] = cz;
      to[i][3] = cw;
      if (cx >  cw)       mask |= CLIP_RIGHT_BIT;
      else if (cx < -cw)  mask |= CLIP_LEFT_BIT;
      if (cy >  cw)       mask |= CLIP_TOP_BIT;
      else if (cy < -cw)  mask |= CLIP_BOTTOM_BIT;
      if (cz >  cw)       mask |= CLIP_FAR_BIT;
      else if (cz < -cw)  mask |= CLIP_NEAR_BIT;
      if (mask) {
	 clipMask[i] |= mask;
	 tmpOrMask |= mask;
      }
      tmpAndMask &= mask;
   }
   *orMask = tmpOrMask;
   *andMask = tmpAndMask;
   END_FAST_MATH;
}


static void transform_cliptest_points4_general( const GLmatrix *mat,
                                                GLuint n,
                                                const GLfloat *from,
                                                GLfloat to[][4],
                                                GLuint stride,
                                                GLubyte clipMask[],
                                                GLubyte *orMask,
                                                GLubyte *andMask )
{
   GLubyte tmpOrMask = *orMask;
   GLubyte tmpAndMask = *andMask;
   const GLfloat *m = mat->m;
   const GLfloat m0 = m[0],  m4 = m[4],  m8 = m[8],  m12 = m[12];
   const GLfloat m1 = m[1],  m5 = m[5],  m9 = m[9],  m13 = m[13];
   const GLfloat m2 = m[2],  m6 = m[6],  m10 = m[10],  m14 = m[14];
   const GLfloat m3 = m[3],  m7 = m[7],  m11 = m[11],  m15 = m[15];
   GLuint i;
   START_FAST_MATH;
   ASSERT(mat->type == MATRIX_GENERAL);
   for (i=0;i<n;i++, from+=stride) {
      const GLfloat ox = from[0], oy = from[1], oz = from[2], ow = from[3];
      volatile GLfloat cx = m0 * ox + m4 * oy + m8  * oz + m12 * ow;
      volatile GLfloat cy = m1 * ox + m5 * oy + m9  * oz + m13 * ow;
      volatile GLfloat cz = m2 * ox + m6 * oy + m10 * oz + m14 * ow;
      volatile GLfloat cw = m3 * ox + m7 * oy + m11 * oz + m15 * ow;
      GLubyte mask = 0;
      to[i][0] = cx;
      to[i][1] = cy;
      to[i][2] = cz;
      to[i][3] = cw;
      if (cx >  cw)       mask |= CLIP_RIGHT_BIT;
      else if (cx < -cw)  mask |= CLIP_LEFT_BIT;
      if (cy >  cw)       mask |= CLIP_TOP_BIT;
      else if (cy < -cw)  mask |= CLIP_BOTTOM_BIT;
      if (cz >  cw)       mask |= CLIP_FAR_BIT;
      else if (cz < -cw)  mask |= CLIP_NEAR_BIT;
      if (mask) {
	 clipMask[i] |= mask;
	 tmpOrMask |= mask;
      }
      tmpAndMask &= mask;
   }
   *orMask = tmpOrMask;
   *andMask = tmpAndMask;
   END_FAST_MATH;
}



static void cliptest_points4( GLuint n,
			      CONST GLfloat vClip[][4],
			      GLubyte clipMask[],
			      GLubyte *orMask,
			      GLubyte *andMask )
{
   GLubyte tmpOrMask = *orMask;
   GLubyte tmpAndMask = *andMask;
   GLuint i;
   START_FAST_MATH;
   for (i=0;i<n;i++) {
      const GLfloat cx = vClip[i][0], cy = vClip[i][1];
      const GLfloat cz = vClip[i][2], cw = vClip[i][3];
      GLubyte mask = 0;
      if (cx >  cw)       mask |= CLIP_RIGHT_BIT;
      else if (cx < -cw)  mask |= CLIP_LEFT_BIT;
      if (cy >  cw)       mask |= CLIP_TOP_BIT;
      else if (cy < -cw)  mask |= CLIP_BOTTOM_BIT;
      if (cz >  cw)       mask |= CLIP_FAR_BIT;
      else if (cz < -cw)  mask |= CLIP_NEAR_BIT;
      clipMask[i] |= mask;
      tmpOrMask |= mask;
      tmpAndMask &= mask;
   }

   *orMask = tmpOrMask;
   *andMask = tmpAndMask;
   END_FAST_MATH;
}

static void cliptest_points3( GLuint n,
			      CONST GLfloat vClip[][4],
			      GLubyte clipMask[],
			      GLubyte *orMask,
			      GLubyte *andMask )
{
   GLubyte tmpOrMask = *orMask;
   GLubyte tmpAndMask = *andMask;
   GLuint i;
   START_FAST_MATH;
   for (i=0;i<n;i++) {
      const GLfloat cx = vClip[i][0], cy = vClip[i][1], cz = vClip[i][2];
      GLubyte mask = 0;
      if (cx >  1.0)       mask |= CLIP_RIGHT_BIT;
      else if (cx < -1.0)  mask |= CLIP_LEFT_BIT;
      if (cy >  1.0)       mask |= CLIP_TOP_BIT;
      else if (cy < -1.0)  mask |= CLIP_BOTTOM_BIT;
      if (cz >  1.0)       mask |= CLIP_FAR_BIT;
      else if (cz < -1.0)  mask |= CLIP_NEAR_BIT;
      clipMask[i] |= mask;
      tmpOrMask |= mask;
      tmpAndMask &= mask;
   }

   *orMask = tmpOrMask;
   *andMask = tmpAndMask;
   END_FAST_MATH;
}

static void cliptest_points2( GLuint n,
			      CONST GLfloat vClip[][4],
			      GLubyte clipMask[],
			      GLubyte *orMask,
			      GLubyte *andMask )
{
   GLubyte tmpOrMask = *orMask;
   GLubyte tmpAndMask = *andMask;
   GLuint i;
   START_FAST_MATH;
   for (i=0;i<n;i++) {
      const GLfloat cx = vClip[i][0], cy = vClip[i][1];
      GLubyte mask = 0;
      if (cx >  1.0)       mask |= CLIP_RIGHT_BIT;
      else if (cx < -1.0)  mask |= CLIP_LEFT_BIT;
      if (cy >  1.0)       mask |= CLIP_TOP_BIT;
      else if (cy < -1.0)  mask |= CLIP_BOTTOM_BIT;
      clipMask[i] |= mask;
      tmpOrMask |= mask;
      tmpAndMask &= mask;
   }

   *orMask = tmpOrMask;
   *andMask = tmpAndMask;
   END_FAST_MATH;
}



typedef void (*transform_func)( const GLmatrix *mat,
				GLuint n,
				const GLfloat *from,
				GLfloat to[][4],
				GLuint stride );

typedef void (*clip_func)( GLuint n,
			   CONST GLfloat vClip[][4],
			   GLubyte clipMask[],
			   GLubyte *orMask,
			   GLubyte *andMask );

typedef void (*project_and_clip_func)( const GLmatrix *mat,
				       GLuint n,
				       const GLfloat *from,
				       GLfloat to[][4],
				       GLuint stride,
				       GLubyte clipMask[],
				       GLubyte *orMask,
				       GLubyte *andMask );


/*
 * Table of optimized vertex transformation functions.
 * The first array index is the vertex size (0D, 1D, 2D, 3D, 4D)
 * The second array index is the matrix type (one of MATRIX_* constants)
 */
static transform_func transform_tab[5][7];


/*
 * Table of optimized cliptest functions.
 * The array index is the vertex size (0D, 1D, 2D, 3D, 4D)
 */
static clip_func clip_tab[5];


/*
 * Table of project and clip functions.
 * The first array index is the vertex size (0D, 1D, 2D, 3D, 4D)
 * The second array index is the matrix type (one of MATRIX_* constants)
 */
static project_and_clip_func project_clip_tab[5][7];



/*
 * This is called only once.  It initializes several tables with pointers
 * to optimized transformation functions.  This is where we can test for
 * AMD 3Dnow! capability, Intel Katmai, etc. and hook in the right code.
 */
void gl_init_transformation( void )
{
   /*
    * Point transformation (modelview and sometimes projection)
    */

   /* 0-D points (never happens) */
   transform_tab[0][MATRIX_GENERAL]     = 0;
   transform_tab[0][MATRIX_IDENTITY]    = 0;
   transform_tab[0][MATRIX_3D_NO_ROT]   = 0;
   transform_tab[0][MATRIX_PERSPECTIVE] = 0;
   transform_tab[0][MATRIX_2D]          = 0;
   transform_tab[0][MATRIX_2D_NO_ROT]   = 0;
   transform_tab[0][MATRIX_3D]          = 0;

   /* 1-D points (never happens) */
   transform_tab[1][MATRIX_GENERAL]     = 0;
   transform_tab[1][MATRIX_IDENTITY]    = 0;
   transform_tab[1][MATRIX_3D_NO_ROT]   = 0;
   transform_tab[1][MATRIX_PERSPECTIVE] = 0;
   transform_tab[1][MATRIX_2D]          = 0;
   transform_tab[1][MATRIX_2D_NO_ROT]   = 0;
   transform_tab[1][MATRIX_3D]          = 0;

   /* 2-D points */
   transform_tab[2][MATRIX_GENERAL]     = transform_points2_general;
   transform_tab[2][MATRIX_IDENTITY]    = transform_points2_identity;
   transform_tab[2][MATRIX_3D_NO_ROT]   = transform_points2_3d;      /*yes!*/
   transform_tab[2][MATRIX_PERSPECTIVE] = transform_points2_general; /*yes!*/
   transform_tab[2][MATRIX_2D]          = transform_points2_2d;
   transform_tab[2][MATRIX_2D_NO_ROT]   = transform_points2_2d_no_rot;
   transform_tab[2][MATRIX_3D]          = transform_points2_3d;

   /* 3-D points */
#if defined(USE_X86_ASM)
   transform_tab[3][MATRIX_GENERAL]     = asm_transform_points3_general;
   transform_tab[3][MATRIX_IDENTITY]    = asm_transform_points3_identity;
   transform_tab[3][MATRIX_3D_NO_ROT]   = transform_points3_3d_no_rot; /*yes!*/
   transform_tab[3][MATRIX_PERSPECTIVE] = transform_points3_perspective;/*yes*/
   transform_tab[3][MATRIX_2D]          = asm_transform_points3_2d;
   transform_tab[3][MATRIX_2D_NO_ROT]   = asm_transform_points3_2d_no_rot;
   transform_tab[3][MATRIX_3D]          = asm_transform_points3_3d;
#elif defined(USE_3DNOW_ASM)
   /* AMD 3D-Now! functions hook in here */
#else
   transform_tab[3][MATRIX_GENERAL]     = transform_points3_general;
   transform_tab[3][MATRIX_IDENTITY]    = transform_points3_identity;
   transform_tab[3][MATRIX_3D_NO_ROT]   = transform_points3_3d_no_rot;
   transform_tab[3][MATRIX_PERSPECTIVE] = transform_points3_perspective;
   transform_tab[3][MATRIX_2D]          = transform_points3_2d;
   transform_tab[3][MATRIX_2D_NO_ROT]   = transform_points3_2d_no_rot;
   transform_tab[3][MATRIX_3D]          = transform_points3_3d;
#endif

   /* 4-D points */
#if defined(USE_X86_ASM)
   transform_tab[4][MATRIX_GENERAL]     = asm_transform_points4_general;
   transform_tab[4][MATRIX_IDENTITY]    = asm_transform_points4_identity;
   transform_tab[4][MATRIX_3D_NO_ROT]   = asm_transform_points4_3d_no_rot;
   transform_tab[4][MATRIX_PERSPECTIVE] = asm_transform_points4_perspective;
   transform_tab[4][MATRIX_2D]          = asm_transform_points4_2d;
   transform_tab[4][MATRIX_2D_NO_ROT]   = asm_transform_points4_2d_no_rot;
   transform_tab[4][MATRIX_3D]          = asm_transform_points4_3d;
#elif defined(USE_3DNOW_ASM)
   /* AMD 3D-Now! functions hook in here */
#else
   transform_tab[4][MATRIX_GENERAL]     = transform_points4_general;
   transform_tab[4][MATRIX_IDENTITY]    = transform_points4_identity;
   transform_tab[4][MATRIX_3D_NO_ROT]   = transform_points4_3d_no_rot;
   transform_tab[4][MATRIX_PERSPECTIVE] = transform_points4_perspective;
   transform_tab[4][MATRIX_2D]          = transform_points4_2d;
   transform_tab[4][MATRIX_2D_NO_ROT]   = transform_points4_2d_no_rot;
   transform_tab[4][MATRIX_3D]          = transform_points4_3d;
#endif


   /*
    * Clip testing
    */
#ifdef USE_X86_ASM
   clip_tab[0] = 0;
   clip_tab[1] = 0;
   clip_tab[2] = asm_cliptest_points4,
   clip_tab[3] = asm_cliptest_points4,
   clip_tab[4] = asm_cliptest_points4,
#else
   clip_tab[0] = 0;
   clip_tab[1] = 0;
   clip_tab[2] = cliptest_points2;
   clip_tab[3] = cliptest_points3;
   clip_tab[4] = cliptest_points4;
#endif


   /*
    * Projection and clip testing
    */

   /* 0-D points (never happens) */
   project_clip_tab[0][MATRIX_GENERAL]     = 0;
   project_clip_tab[0][MATRIX_IDENTITY]    = 0;
   project_clip_tab[0][MATRIX_3D_NO_ROT]   = 0;
   project_clip_tab[0][MATRIX_PERSPECTIVE] = 0;
   project_clip_tab[0][MATRIX_2D]          = 0;
   project_clip_tab[0][MATRIX_2D_NO_ROT]   = 0;
   project_clip_tab[0][MATRIX_3D]          = 0;

   /* 1-D points (never happens) */
   project_clip_tab[1][MATRIX_GENERAL]     = 0;
   project_clip_tab[1][MATRIX_IDENTITY]    = 0;
   project_clip_tab[1][MATRIX_3D_NO_ROT]   = 0;
   project_clip_tab[1][MATRIX_PERSPECTIVE] = 0;
   project_clip_tab[1][MATRIX_2D]          = 0;
   project_clip_tab[1][MATRIX_2D_NO_ROT]   = 0;
   project_clip_tab[1][MATRIX_3D]          = 0;

   /* 2-D points (never happens) */
   project_clip_tab[2][MATRIX_GENERAL]     = 0;
   project_clip_tab[2][MATRIX_IDENTITY]    = 0;
   project_clip_tab[2][MATRIX_3D_NO_ROT]   = 0;
   project_clip_tab[2][MATRIX_PERSPECTIVE] = 0;
   project_clip_tab[2][MATRIX_2D]          = 0;
   project_clip_tab[2][MATRIX_2D_NO_ROT]   = 0;
   project_clip_tab[2][MATRIX_3D]          = 0;

   /* 3-D points */
#if defined(USE_X86_ASM)
   project_clip_tab[3][MATRIX_GENERAL]     = 0;
   project_clip_tab[3][MATRIX_IDENTITY]    = 0;
   project_clip_tab[3][MATRIX_3D_NO_ROT]   = 0;
   project_clip_tab[3][MATRIX_PERSPECTIVE] = 0;
   project_clip_tab[3][MATRIX_2D]          = 0;
   project_clip_tab[3][MATRIX_2D_NO_ROT]   = 0;
   project_clip_tab[3][MATRIX_3D]          = 0;
#elif defined(USE_3DNOW_ASM)
   /* AMD 3D-Now! functions hook in here */
#else
   project_clip_tab[3][MATRIX_GENERAL]     = transform_cliptest_points3_general;
   project_clip_tab[3][MATRIX_IDENTITY]    = 0;
   project_clip_tab[3][MATRIX_3D_NO_ROT]   = transform_cliptest_points3_3d_no_rot;
   project_clip_tab[3][MATRIX_PERSPECTIVE] = transform_cliptest_points3_perspective;
   project_clip_tab[3][MATRIX_2D]          = 0;
   project_clip_tab[3][MATRIX_2D_NO_ROT]   = 0;
   project_clip_tab[3][MATRIX_3D]          = 0;
#endif

   /* 4-D points */
#if defined(USE_X86_ASM)
   project_clip_tab[4][MATRIX_GENERAL]     = 0;
   project_clip_tab[4][MATRIX_IDENTITY]    = 0;
   project_clip_tab[4][MATRIX_3D_NO_ROT]   = 0;
   project_clip_tab[4][MATRIX_PERSPECTIVE] = 0;
   project_clip_tab[4][MATRIX_2D]          = 0;
   project_clip_tab[4][MATRIX_2D_NO_ROT]   = 0;
   project_clip_tab[4][MATRIX_3D]          = 0;
#elif defined(USE_3DNOW_ASM)
   /* AMD 3D-Now! functions hook in here */
#else
   project_clip_tab[4][MATRIX_GENERAL]     = transform_cliptest_points4_general;
   project_clip_tab[4][MATRIX_IDENTITY]    = 0;
   project_clip_tab[4][MATRIX_3D_NO_ROT]   = transform_cliptest_points4_3d_no_rot;
   project_clip_tab[4][MATRIX_PERSPECTIVE] = transform_cliptest_points4_perspective;
   project_clip_tab[4][MATRIX_2D]          = 0;
   project_clip_tab[4][MATRIX_2D_NO_ROT]   = 0;
   project_clip_tab[4][MATRIX_3D]          = 0;
#endif

}



void gl_project_and_cliptest_points( const GLmatrix *mat,
				     GLuint n,
				     const GLfloat *from,
				     GLfloat to[][4],
				     GLuint stride,
				     GLubyte clipMask[],
				     GLubyte *orMask,
				     GLubyte *andMask,
				     GLuint vec_size)

{
   if (project_clip_tab[vec_size][mat->type]) {
      (project_clip_tab[vec_size][mat->type])(mat, n, from, to, stride,
					      clipMask, orMask, andMask);
   }
   else {
      (transform_tab[vec_size][mat->type])(mat, n, from, to, stride);

#ifdef USE_X86_ASM
      asm_cliptest_points4(n, to, clipMask, orMask, andMask);
#else
      if (vec_size < 4 && !TEST_MAT_FLAGS(mat, MAT_FLAGS_3D))
	 vec_size = 4;
      else if (vec_size < 3 && (mat->type != MATRIX_2D ||
				mat->type != MATRIX_2D_NO_ROT))
	 vec_size = 3;

      (clip_tab[vec_size])(n, to, clipMask, orMask, andMask);
#endif
   }
}


void gl_transform_points( const GLmatrix *mat,
			  GLuint n,
			  const GLfloat *from,
			  GLfloat to[][4],
			  GLuint stride,
			  GLuint vec_size)
{
/*    printf("gl_t_p: sz: %d type: %d from: %x stride: %d\n", */
/* 	  vec_size, mat->type, from, stride); fflush(stdout); */
   (transform_tab[vec_size][mat->type])(mat, n, from, to, stride);
}
