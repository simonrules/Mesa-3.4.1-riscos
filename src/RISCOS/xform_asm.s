; $Id: RISCOS/xform_asm.s,v 0.1 1999/02/13 18:55:00 davidb Exp $ */

;
; Mesa 3-D graphics library
; Version:  3.1
; Copyright (C) 1995-1998  Brian Paul
;
; This library is free software; you can redistribute it and/or
; modify it under the terms of the GNU Library General Public
; License as published by the Free Software Foundation; either
; version 2 of the License, or (at your option) any later version.
;
; This library is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
; Library General Public License for more details.
;
; You should have received a copy of the GNU Library General Public
; License along with this library; if not, write to the Free
; Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
;

;
; $Log: xform_asm.s $
;
; Version: 0.1
; Created: Sat 13th February 1999 (actually earlier)
; by:      David Boddie
;
; First two functions rewritten in assembler. Demonstrations appear to work.
;
; It is worth noting that the floating point registers, f4-f7 should be
; preserved on the stack if they are changed during a function.
;
; Note that these functions may not work on systems other than those which use
; RISC OS 3.71, gcc, UnixLib, and APCS-2. In particular, in APCS-2, floating
; point arguments are not supported, so there appears to be no reason to stack
; floating point registers, f0-f3. Good luck!
;

; * Matrix/vertex/vector transformation stuff
; *
; *
; * NOTES:
; * 1. 4x4 transformation matrices are stored in memory in column major order.
; * 2. Points/vertices are to be thought of as column vectors.
; * 3. Transformation of a point p by a matrix M is: p' = M * p
; *
; * The volatile keyword is needed in a bunch of places to prevent
; * numerical problems on x86 CPUs.
; */

	area	|C$$code|,CODE,READONLY

;/*
; * Transform a 4-element row vector (1x4 matrix) by a 4x4 matrix.  This
; * function is used for transforming clipping plane equations and spotlight
; * directions.
; * Mathematically,  u = v * m.
; * Input:  v - input vector
; *         m - transformation matrix
; * Output:  u - transformed vector
; */

	export	gl_transform_vector

;	Inputs:			a1 = u (array of 4 GLfloats)
;				a2 = v (array of 4 GLfloats)
;				a3 = m (array of 16 GLfloats)

; void gl_transform_vector( GLfloat u[4], const GLfloat v[4], const GLfloat m[16] )

gl_transform_vector

	stmfd	sp!,{a1-a3}		; stack the arguments

;	sfmfd	f0,4,[sp]!		; stack the floating point registers (f0-f3)
	sfmfd	f4,4,[sp]!		; stack the floating point registers (f4-f7)

;	GLfloat v0=v[0], v1=v[1], v2=v[2], v3=v[3];
	lfm	f0,4,[a2],#4		; load the vector, v
;	ldfs	f0,[a2],#4
;	ldfs	f1,[a2],#4
;	ldfs	f2,[a2],#4
;	ldfs	f3,[a2],#4

;	#define M(row,col)  m[col*4+row]

;	u[0] = v0 * M(0,0) + v1 * M(1,0) + v2 * M(2,0) + v3 * M(3,0);
	ldfs	f4,[a3],#4	; load M(0,0)
	mufs	f4,f4,f0	; M(0,0) * v0		fml

	ldfs	f5,[a3],#4	; load M(1,0)
	mufs	f5,f5,f1	; M(1,0) * v1		fml
	adfs	f4,f4,f5

	ldfs	f5,[a3],#4	; load M(2,0)
	mufs	f5,f5,f2	; M(2,0) * v2		fml
	adfs	f4,f4,f5

	ldfs	f5,[a3],#4	; load M(3,0)
	mufs	f5,f5,f3	; M(3,0) * v3		fml
	adfs	f4,f4,f5

	stfs	f4,[a1],#4	; store u[0]

;	u[1] = v0 * M(0,1) + v1 * M(1,1) + v2 * M(2,1) + v3 * M(3,1);
	ldfs	f4,[a3],#4	; load M(0,1)
	mufs	f4,f4,f0	; M(0,1) * v0		fml

	ldfs	f5,[a3],#4	; load M(1,1)
	mufs	f5,f5,f1	; M(1,1) * v1		fml
	adfs	f4,f4,f5

	ldfs	f5,[a3],#4	; load M(2,1)
	mufs	f5,f5,f2	; M(2,1) * v2		fml
	adfs	f4,f4,f5

	ldfs	f5,[a3],#4	; load M(3,1)
	mufs	f5,f5,f3	; M(3,1) * v3		fml
	adfs	f4,f4,f5

	stfs	f4,[a1],#4	; store u[1]

;	u[2] = v0 * M(0,2) + v1 * M(1,2) + v2 * M(2,2) + v3 * M(3,2);
	ldfs	f4,[a3],#4	; load M(0,2)
	mufs	f4,f5,f0	; M(0,2) * v0		fml

	ldfs	f5,[a3],#4	; load M(1,2)
	mufs	f5,f5,f1	; M(1,2) * v1		fml
	adfs	f4,f4,f5

	ldfs	f5,[a3],#4	; load M(2,2)
	mufs	f5,f5,f2	; M(2,2) * v2		fml
	adfs	f4,f4,f5

	ldfs	f5,[a3],#4	; load M(3,2)
	mufs	f5,f5,f3	; M(3,2) * v3		fml
	adfs	f4,f4,f5

	stfs	f4,[a1],#4	; store u[2]

;	u[3] = v0 * M(0,3) + v1 * M(1,3) + v2 * M(2,3) + v3 * M(3,3);
	ldfs	f4,[a3],#4	; load M(0,3)
	mufs	f4,f5,f0	; M(0,3) * v0		fml

	ldfs	f5,[a3],#4	; load M(1,3)
	mufs	f5,f5,f1	; M(1,3) * v1		fml
	adfs	f4,f4,f5

	ldfs	f5,[a3],#4	; load M(2,3)
	mufs	f5,f5,f2	; M(2,3) * v2		fml
	adfs	f4,f4,f5

	ldfs	f5,[a3],#4	; load M(3,3)
	mufs	f5,f5,f3	; M(3,3) * v3		fml
	adfs	f4,f4,f5

	stfs	f4,[a1],#4	; store u[2]

;	#undef M

	lfmfd	f4,4,[sp]!		; unstack the floating point registers (f4-f7)
;	lfmfd	f0,4,[sp]!		; unstack the floating point registers (f0-f3)

	ldmfd	sp!,{a1-a3}		; unstack the arguments

	movs	pc,lr


; /*----------------------------------------------------------------------
;  * Begin Keith's new code
;  *
;  * NOTE:  the units for stride values is a GLfloat.  That is, if
;  * stride = 3 then the start of each floating point coordinate is 12 bytes
;  * after its predecessor.  Have to be clear about this because OpenGL's
;  * vertex array stride values are in bytes!
;  *
;  *----------------------------------------------------------------------
;  */

	export	gl_normalize_3fv
;	import	GL_SQRT

;	Inputs:			a1 = (GLuint) n
;				a2 = address of GLfloats (GLfloat *in)
;				a3 = (GLuint) in_stride,
;				a4 = arrary of GLfloats out[][3]

gl_normalize_3fv

;	GLuint i;
	stmfd	sp!,{a1,a2,a4}	; stack the command registers and r4

;	sfmfd	f0,4,[sp]!	; stack the floating point registers
	sfmfd	f4,4,[sp]!	; stack the floating point registers
;	stfs	f4,[sp,#-4]!
;	stfs	f5,[sp,#-4]!
;	stfs	f6,[sp,#-4]!
;	stfs	f7,[sp,#-4]!

	ldfd	f0,gl_normalize_3fv_const

;	for ( i = 0 ; i < n ; i++, in += in_stride ) {

				; start loop at n and work downwards to n = 1

gl_normalize_3fv_loop1

;	const GLfloat x = in[0], y = in[1], z = in[2];
	ldfs	f4,[a2]		; x = in[0]
	mufd	f1,f4,f4

	ldfs	f5,[a2,#4]	; y = in[1]
	mufd	f2,f5,f5

	ldfs	f6,[a2,#8]	; z = in[2]
	mufd	f3,f6,f6

;	GLdouble len = x * x + y * y + z * z;
	adfd	f7,f1,f2
	adfd	f7,f7,f3	; len = x*x + y*y + z*z

;	if (len > 1e-50) {
	cmf	f7,f0		; compare len to 1.0e-50	cmfd

;	   len = 1.0 / GL_SQRT(len);
	 sqtgtd	f1,f7		; f1 = sqrt(len)
	 rdfgtd	f7,f1,#1.0	; len = 1.0 / sqrt(len)

;	 out[i][0] = x * len;
	 mufgts	f2,f4,f7	; x * len
	 stfgts	f2,[a4],#4
;	 out[i][1] = y * len;
	 mufgts	f2,f5,f7	; y * len
	 stfgts	f2,[a4],#4
;	 out[i][2] = z * len;
	 mufgts	f2,f6,f7	; z * len
	 stfgts	f2,[a4],#4

;	else
;	 out[i][0] = x;
	 stfles	f4,[a4],#4
;	 out[i][1] = y;
	 stfles	f5,[a4],#4
;	 out[i][2] = z;
	 stfles	f6,[a4],#4

	add	a2,a2,a3,lsl#2	; in += in_stride (*4 since each element is a float)
	subs	a1,a1,#1	; i--
	bgt	gl_normalize_3fv_loop1	; while i > 0

	lfmfd	f4,4,[sp]!	; unstack the floating point registers
;	lfmfd	f0,4,[sp]!	; unstack the floating point registers
;	ldfs	f7,[sp],#4
;	ldfs	f6,[sp],#4
;	ldfs	f5,[sp],#4
;	ldfs	f4,[sp],#4

	ldmfd	sp!,{a1,a2,a4}	; unstack the command registers and r4

	movs	pc,lr

gl_normalize_3fv_const
	dcfd	1.0e-50		; was 1.0e-50


	export	gl_scale_3fv

;	Inputs:			a1 = n (GLuint)
;				a2 = in (array of GLfloats)
;				a3 = in_stride (GLuint)
;				a4 = out (array of GLfloats)
;				(stack) = scale (GLfloat)

gl_scale_3fv

	stmfd	sp!,{a1,a2,a4}		; stack arguments
					; sp | sp+4 | sp+8 | sp+12 | sp+16 | sp+20
					; a1 |  a2  |  a3  |   a4  |   r4  | scale

;	sfmfd	f0,2,[sp]!		; stack f0 and f1

	ldfs	f0,[sp,#20]		; load scale argument from stack

; for ( i = 0 ; i < n ; i++, in += in_stride ) {

gl_scale_3fv_loop1

;	SCALE_SCALAR_3V( out[i], scale, in );
; which is (from macros.h)
;	#define SCALE_SCALAR_3V( DST, S, SRCB ) 	\
;	do {						\
;		DST[0] = S * SRCB[0];			\
;		DST[1] = S * SRCB[1];			\
;		DST[2] = S * SRCB[2];			\
;	} while (0)

	ldfs	f1,[a2]			; load in[0]
	mufs	f1,f1,f0		; in[0] * scale
	stfs	f1,[a4],#4		; out[][0] = in[0] * scale

	ldfs	f1,[a2,#4]		; load in[1]
	mufs	f1,f1,f0		; in[1] * scale
	stfs	f1,[a4],#4		; out[][1] = in[1] * scale

	ldfs	f1,[a2,#8]		; load in[2]
	mufs	f1,f1,f0		; in[2] * scale
	stfs	f1,[a4],#4		; out[][2] = in[2] * scale

	add	a2,a2,a3,lsl#2		; in += in_stride*sizeof(GLfloat)
	subs	a1,a1,#1		; n--
	bgt	gl_scale_3fv_loop1	; while n>0

;	lfmfd	f0,2,[sp]!

	ldmfd	sp!,{a1,a2,a4}		; unstack arguments

	movs	pc,lr
