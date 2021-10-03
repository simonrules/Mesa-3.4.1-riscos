// s.ros_asm
// Assembler functions for writing pixels to a VIDC style framebuffer
// Author:	David Boddie
// Last updated:	Wed 30th May 2001


// ******************** 32 bpp functions ********************

// -----	Masked rgba pixel span -----

	.globl	asm_write_rgba_span_masked

//	Inputs:			a1 = n		(r0)
//				a2 = &mask	(r1)
//				a3 = &rgba	(r2)
//				a4 = ptr4	(r3)

asm_write_rgba_span_masked:

	stmfd	sp!,{r4,lr}

	sub	a1,a1,#1	// Start at (n-1)th pixel
	add	a4,a4,a1,lsl#2	// Modify ptr4 to start at the end of the span
	add	a3,a3,a1,lsl#2	// Modify rgba to end of the array

loop_start_rgba_span_m:

	ldrb	r4,[a2,a1]	// load value from the mask array
	cmp	r4,#0

// Hypothesis: interleaving instructions may make execution faster

	ldrne	r4,[a3],#-4	// load word from rgba if mask is non-zero
	subeq	a3,a3,#4	// update rgba
	strne	r4,[a4],#-4	// store word at ptr4
	subeq	a4,a4,#4	// update ptr4

	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_rgba_span_m	// repeat if r0 >= 0

	ldmfd	sp!,{r4,pc}^



// -----	Unmasked rgba pixel span -----

	.globl	asm_write_rgba_span_unmasked

//	Inputs:			a1 = n		(r0)
//				a2 = &rgba	(r1)
//				a3 = ptr4	(r2)

asm_write_rgba_span_unmasked:

	stmfd	sp!,{r4,lr}

	sub	a1,a1,#1	// Start at (n-1)th pixel
	add	a3,a3,a1,lsl#2	// Modify ptr4 to start at the end of the span
	add	a2,a2,a1,lsl#2	// Modify rgba to end of the span

loop_start_rgba_span_um:

	ldr	r4,[a2],#-4	// load word from rgba

	str	r4,[a3],#-4	// store word at ptr4

//	sub	a3,a3,#4	// update ptr4
//	sub	a2,a2,#4	// update rgba
	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_rgba_span_um	// repeat if r0 >= 0

	ldmfd	sp!,{r4,pc}^



// -----	Masked rgb pixel span -----

	.globl	asm_write_rgb_span_masked

//	Inputs:			a1 = n		(r0)
//				a2 = &mask	(r1)
//				a3 = &rgba	(r2)
//				a4 = ptr4	(r3)

asm_write_rgb_span_masked:

	stmfd	sp!,{r4,r5,lr}

	sub	a1,a1,#1	// Start at (n-1)th pixel
	add	a4,a4,a1,lsl#2	// Modify ptr4 to start at the end of the span
	add	a3,a3,a1,lsl#1	// Modify rgba to end of the array
	add	a3,a3,a1

loop_start_rgb_span_m:

	ldrb	r4,[a2,a1]	// load value from the mask array
	cmp	r4,#0		// if it is zero then
	subeq	a4,a4,#4	// update ptr4
	beq	next_pixel_rgb_span_m	// jump to next pixel

	ldrb	r4,[a3]		// load first byte from rgba
	ldrb	r5,[a3,#1]
	orr	r4,r4,r5,lsl#16
	ldrb	r5,[a3,#2]
	orr	r4,r4,r5,lsl#24
	orr	r4,r4,#0xff000000

	str	r4,[a4],#-4	// store word at ptr4

next_pixel_rgb_span_m:
	sub	a3,a3,#3	// update rgba
	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_rgb_span_m	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,pc}^



// -----	Unmasked rgb pixel span -----

	.globl	asm_write_rgb_span_unmasked

//	Inputs:			a1 = n		(r0)
//				a2 = &rgba	(r1)
//				a3 = ptr4	(r2)

asm_write_rgb_span_unmasked:

	stmfd	sp!,{r4,r5,lr}

	sub	a1,a1,#1	// Start at (n-1)th pixel
	add	a3,a3,a1,lsl#2	// Modify ptr4 to start at the end of the span
	add	a2,a2,a1,lsl#1	// Modify rgba to end of the span
	add	a2,a2,a1

loop_start_rgb_span_um:

	ldrb	r4,[a2]		// load first byte from rgba
	ldrb	r5,[a2,#1]
	orr	r4,r4,r5,lsl#16
	ldrb	r5,[a2,#2]
	orr	r4,r4,r5,lsl#24
	orr	r4,r4,#0xff000000

	str	r4,[a3],#-4	// store word at ptr4

//	sub	a3,a3,#4	// update ptr4
	sub	a2,a2,#3	// update rgba
	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_rgb_span_um	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,pc}^



// -----	Monocolor pixel span -----

	.globl	asm_write_monocolor_span

//	Inputs:			a1 = n
//				a2 = &mask
//				a3 = colour
//				a4 = ptr4

asm_write_monocolor_span:

	stmfd	sp!,{r4,r5,lr}

	sub	r4,a1,#1	// Start at (n-1)th pixel
	add	a4,a4,r4,lsl#2	// Modify ptr4 to start at the end of the span

loop_start_monocolor_span:

	ldrb	r5,[a2,r4]	// load value from the mask array

	cmp	r5,#0
	strne	a3,[a4],#-4	// store word at ptr4 if mask is non-zero

	subeq	a4,a4,#4	// update ptr4
	subs	r4,r4,#1	// subtract 1 from the counter
	bge	loop_start_monocolor_span	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,pc}^



// -----	Write rgba pixels -----

	.globl	asm_write_rgba_pixels

//	Inputs:			a1 = n
//				a2 = x[]
//				a3 = y[]
//				a4 = parameters[] (rgba[], mask[], buffer end, width)

asm_write_rgba_pixels:

	stmfd	sp!,{r4,r5,r6,r7,r8,lr}

	sub	a1,a1,#1	// Start at (n-1)th pixel
	ldr	r5,[a4]		// r5 = rgba[]
	ldr	r6,[a4,#4]	// r6 = mask[]
	ldr	r7,[a4,#8]	// r7 = buffer end
	ldr	a4,[a4,#12]	// a4 = width
	mov	a4,a4,lsl#2	// a4 = width * pixel depth

loop_start_write_rgba_pixels:

	ldrb	r8,[r6,a1]	// load value from the mask array
	cmp	r8,#0
	beq	next_pixel_write_rgba_pixels

	ldr	r4,[a2,a1,lsl#2]// load x position
	mov	r4,r4,lsl#2	// pixel = x * pixel size
	ldr	r8,[a3,a1,lsl#2]// load y position
	mul	r8,a4,r8	// y * width
	sub	r4,r4,r8	// x - (y * width)

	ldr	r8,[r5,a1,lsl#2]// load word from rgba if mask is non-zero
	str	r8,[r7,r4]	// store word at buffer + x - (y * width)

next_pixel_write_rgba_pixels:

	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_write_rgba_pixels	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,r7,r8,pc}^



// -----	Array of monocolor pixels -----

	.globl	asm_write_monocolor_pixels

//	Inputs:			a1 = n
//				a2 = x[]
//				a3 = y[]
//				a4 = parameters[] (colour, mask[], buffer end, width)

asm_write_monocolor_pixels:

	stmfd	sp!,{r4,r5,r6,r7,r8,lr}

	sub	a1,a1,#1	// Start at (n-1)th pixel
	ldr	r5,[a4]		// r5 = colour
	ldr	r6,[a4,#4]	// r6 = mask[]
	ldr	r7,[a4,#8]	// r7 = buffer end
	ldr	a4,[a4,#12]	// a4 = width
	mov	a4,a4,lsl#2	// a4 = width * pixel depth

loop_start_monocolor_pixels:

	ldrb	r8,[r6,a1]	// load value from the mask array
	cmp	r8,#0
	beq	next_pixel_monocolor_pixels

	ldr	r4,[a2,a1,lsl#2]// load x position
	mov	r4,r4,lsl#2	// pixel = x * pixel size
	ldr	r8,[a3,a1,lsl#2]// load y position
	mul	r8,a4,r8	// y * width
	sub	r4,r4,r8	// x - (y * width)

	str	r5,[r7,r4]	// store colour at buffer + x - (y * width)

next_pixel_monocolor_pixels:

	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_monocolor_pixels	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,r7,r8,pc}^



// -----	Read rgba span -----

	.globl	asm_read_rgba_span

//	Inputs:			a1 = n		(r0)
//				a2 = &rgba	(r1)
//				a3 = ptr4	(r2)

asm_read_rgba_span:

	stmfd	sp!,{r4,lr}

	sub	a1,a1,#1	// Start at (n-1)th pixel
	add	a3,a3,a1,lsl#2	// Modify ptr4 to start at the end of the span
	add	a2,a2,a1,lsl#2	// Modify rgba to end of the array

loop_start_rgba_read_span:

	ldr	r4,[a3],#-4	// load value from ptr4

	str	r4,[a2],#-4	// store word in rgba

//	sub	a3,a3,#4	// update ptr4
//	sub	a2,a2,#4	// update rgba
	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_rgba_read_span	// repeat if r0 >= 0

	ldmfd	sp!,{r4,pc}^



// -----	Read rgba pixels -----

	.globl	asm_read_rgba_pixels

//	Inputs:			a1 = n
//				a2 = x[]
//				a3 = y[]
//				a4 = parameters[] (rgba[], mask[], buffer end, width)

asm_read_rgba_pixels:

	stmfd	sp!,{r4,r5,r6,r7,r8,lr}

	sub	a1,a1,#1	// Start at (n-1)th pixel
	ldr	r5,[a4]		// r5 = rgba[]
	ldr	r6,[a4,#4]	// r6 = mask[]
	ldr	r7,[a4,#8]	// r7 = buffer end
	ldr	a4,[a4,#12]	// a4 = width
	mov	a4,a4,lsl#2	// a4 = width * pixel depth

loop_start_read_rgba_pixels:

	ldrb	r8,[r6,a1]	// load value from the mask array
	cmp	r8,#0
	beq	next_pixel_read_rgba_pixels

	ldr	r4,[a2,a1,lsl#2]// load x position
	mov	r4,r4,lsl#2	// pixel = x * pixel size
	ldr	r8,[a3,a1,lsl#2]// load y position
	mul	r8,a4,r8	// y * width
	sub	r4,r4,r8	// x - (y * width)

	ldr	r8,[r7,r4]	// load word from buffer + x - (y * width)
	str	r8,[r5,a1,lsl#2]// store word in rgba if mask is non-zero

next_pixel_read_rgba_pixels:

	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_read_rgba_pixels	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,r7,r8,pc}^




// ******************** 16 bpp functions ********************

// -----	Masked rgba pixel span -----

	.globl	asm_write_rgba_span_masked2

//	Inputs:			a1 = n		(r0)
//				a2 = &mask	(r1)
//				a3 = &rgba	(r2)
//				a4 = &ptr2	(r3)

asm_write_rgba_span_masked2:

	stmfd	sp!,{r4,r5,r6,lr}

	subs	a1,a1,#1	// Start at (n-1)th pixel
//	// If there is only one pixel then jump to a special case
//	beq	rgba_span_m2_one_pixel

	addgt	a4,a4,a1,lsl#1	// Modify ptr2 to start at the end of the span
				// ptr2 = ptr2 + (n * 2)
	addgt	a3,a3,a1,lsl#2	// Modify rgba to end of the array
				// rgba = rgba + (n * 4)

//	// If the last pixel of the span lies in the lower half of a word then
//	// write it before proceeding
//	ands	r4,a4,#2
//	beq	rgba_span_m2_upper_aligned


//rgba_span_m2_upper_aligned
//
//	// At this point, there should be more than one pixel to write and
//	// pairs of pixels should be word-aligned

loop_start_rgba_span_m2:

	ldrb	r4,[a2,a1]	// load value from the mask array
	cmp	r4,#0

	ldrne	r4,[a3],#-4		// load word from rgba if mask is non-zero
	subeq	a3,a3,#4		// otherwise update rgba

	// Convert rgb values to 16 bpp format
	// (Note that Mesa internally stores rgba as R(0) G(1) B(2) A(3)
	// so that reading a word containing these bytes gives
	// aaaaaaaabbbbbbbbggggggggrrrrrrrr

	// Upper byte (blue and upper green):
	andne	r5,r4,#0x00f80000	// blue (5 bits)
	andne	r6,r4,#0x0000c000	// upper green (2 bits)
	orrne	r5,r5,r6,lsl#3		// now we have |0|bbbbbgg0|0|0|

	// Since we don't have access to a store half-word instruction on this
	// architecture then we will have to shift this value in order to
	// write a byte
	movne	r5,r5,lsr#17
	strneb	r5,[a4,#1]

	// Lower byte (lower green and red):
	andne	r6,r4,#0x00003800	// lower green (3 bits)
	andne	r4,r4,#0x000000f8	// red (5 bits)
	movne	r4,r4,lsr#3		// |0|0|0|000rrrrr|
	orrne	r4,r4,r6,lsr#6		// |0|0|0|gggrrrrr|

	// Again, the lack of a half-word store instruction means that instead
	// of doing this:
//	andne	r4,r4,r5,lsr#8		// |0|0|0bbbbbgg|gggrrrrr|
	// and writing the following as a half-word, we have to store a byte

	strneb	r4,[a4],#-2	// store word at ptr2
	subeq	a4,a4,#2	// update ptr2

	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_rgba_span_m2	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,pc}^



// -----	Masked rgba pixel span (dithered, even) -----

	.globl	asm_write_rgba_span_masked_dither_even2

//	Inputs:			a1 = n		(r0)
//				a2 = &mask	(r1)
//				a3 = &rgba	(r2)
//				a4 = &ptr2	(r3)

asm_write_rgba_span_masked_dither_even2:

	stmfd	sp!,{r4,r5,r6,r7,lr}

	subs	a1,a1,#1	// Start at (n-1)th pixel

	addgt	a4,a4,a1,lsl#1	// Modify ptr2 to start at the end of the span
				// ptr2 = ptr2 + (n * 2)
	addgt	a3,a3,a1,lsl#2	// Modify rgba to end of the array
				// rgba = rgba + (n * 4)

	mov	r7,#0		// even start

loop_start_rgba_span_mde2:

	ldrb	r4,[a2,a1]	// load value from the mask array
	cmp	r4,#0

	subeq	a3,a3,#4		// otherwise update rgba
	subeq	a4,a4,#2		// update ptr2
	eoreq	r7,r7,#1		// flip value of r7 between 1 and 0
	beq     rgba_span_mde2_next

	ldr	r4,[a3],#-4		// load word from rgba if mask is non-zero

	eors	r7,r7,#1		// if was zero then write as usual

	// Upper byte (blue and upper green):
	andne	r5,r4,#0x00f80000	// blue (5 bits)
	andne	r6,r4,#0x0000c000	// upper green (2 bits)
	orrne	r5,r5,r6,lsl#3		// now we have |0|bbbbbgg0|0|0|

	movne	r5,r5,lsr#17
	strneb	r5,[a4,#1]

	// Lower byte (lower green and red):
	andne	r6,r4,#0x00003800	// lower green (3 bits)
	andne	r4,r4,#0x000000f8	// red (5 bits)
	movne	r4,r4,lsr#3		// |0|0|0|000rrrrr|
	orrne	r4,r4,r6,lsr#6		// |0|0|0|gggrrrrr|

	strneb	r4,[a4],#-2	// store word at ptr2

	// for an odd pixel then we take into account the sixth bit of each component

	// Upper byte (blue and upper green):
	andeq	r5,r4,#0x00f80000	// blue (5 bits)
	andeq	r6,r4,#0x00040000	// blue (sixth bit)
	orreq	r5,r5,r6,lsl#1
	andeq	r6,r4,#0x0000c000	// upper green (2 bits)
	orreq	r5,r5,r6,lsl#3		// now we have |0|bbbbbgg0|0|0|

	moveq	r5,r5,lsr#17
	streqb	r5,[a4,#1]

	// Lower byte (lower green and red):
	andeq	r6,r4,#0x00003800	// lower green (3 bits)
	andeq	r5,r4,#0x00000400	// lower green (sixth bit)
	orreq	r6,r6,r5,lsl#1
	andeq	r5,r4,#0x00000004	// red (sixth bit)
	andeq	r4,r4,#0x000000f8	// red (5 bits)
	orreq	r4,r4,r5,lsl#1
	moveq	r4,r4,lsr#3		// |0|0|0|000rrrrr|
	orreq	r4,r4,r6,lsr#6		// |0|0|0|gggrrrrr|

	streqb	r4,[a4],#-2	// store word at ptr2

rgba_span_mde2_next:

	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_rgba_span_mde2	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,r7,pc}^



// -----	Masked rgba pixel span (dithered, odd) -----

	.globl	asm_write_rgba_span_masked_dither_odd2

//	Inputs:			a1 = n		(r0)
//				a2 = &mask	(r1)
//				a3 = &rgba	(r2)
//				a4 = &ptr2	(r3)

asm_write_rgba_span_masked_dither_odd2:

	stmfd	sp!,{r4,r5,r6,r7,lr}

	subs	a1,a1,#1	// Start at (n-1)th pixel

	addgt	a4,a4,a1,lsl#1	// Modify ptr2 to start at the end of the span
				// ptr2 = ptr2 + (n * 2)
	addgt	a3,a3,a1,lsl#2	// Modify rgba to end of the array
				// rgba = rgba + (n * 4)

	mov	r7,#1		// odd start

loop_start_rgba_span_mdo2:

	ldrb	r4,[a2,a1]	// load value from the mask array
	cmp	r4,#0

	subeq	a3,a3,#4		// otherwise update rgba
	subeq	a4,a4,#2		// update ptr2
	eoreq	r7,r7,#1		// flip value of r7 between 1 and 0
	beq     rgba_span_mdo2_next

	ldr	r4,[a3],#-4		// load word from rgba if mask is non-zero

	eors	r7,r7,#1		// if was zero then write as usual

	// Upper byte (blue and upper green):
	andne	r5,r4,#0x00f80000	// blue (5 bits)
	andne	r6,r4,#0x0000c000	// upper green (2 bits)
	orrne	r5,r5,r6,lsl#3		// now we have |0|bbbbbgg0|0|0|

	movne	r5,r5,lsr#17
	strneb	r5,[a4,#1]

	// Lower byte (lower green and red):
	andne	r6,r4,#0x00003800	// lower green (3 bits)
	andne	r4,r4,#0x000000f8	// red (5 bits)
	movne	r4,r4,lsr#3		// |0|0|0|000rrrrr|
	orrne	r4,r4,r6,lsr#6		// |0|0|0|gggrrrrr|

	strneb	r4,[a4],#-2	// store word at ptr2

	// for an odd pixel then we take into account the sixth bit of each component

	// Upper byte (blue and upper green):
	andeq	r5,r4,#0x00f80000	// blue (5 bits)
	andeq	r6,r4,#0x00040000	// blue (sixth bit)
	orreq	r5,r5,r6,lsl#1
	andeq	r6,r4,#0x0000c000	// upper green (2 bits)
	orreq	r5,r5,r6,lsl#3		// now we have |0|bbbbbgg0|0|0|

	moveq	r5,r5,lsr#17
	streqb	r5,[a4,#1]

	// Lower byte (lower green and red):
	andeq	r6,r4,#0x00003800	// lower green (3 bits)
	andeq	r5,r4,#0x00000400	// lower green (sixth bit)
	orreq	r6,r6,r5,lsl#1
	andeq	r5,r4,#0x00000004	// red (sixth bit)
	andeq	r4,r4,#0x000000f8	// red (5 bits)
	orreq	r4,r4,r5,lsl#1
	moveq	r4,r4,lsr#3		// |0|0|0|000rrrrr|
	orreq	r4,r4,r6,lsr#6		// |0|0|0|gggrrrrr|

	streqb	r4,[a4],#-2	// store word at ptr2

rgba_span_mdo2_next:

	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_rgba_span_mdo2	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,r7,pc}^



// -----	Unmasked rgba pixel span -----

	.globl	asm_write_rgba_span_unmasked2

//	Inputs:			a1 = n		(r0)
//				a2 = &rgba	(r1)
//				a3 = &ptr2	(r2)

asm_write_rgba_span_unmasked2:

	stmfd	sp!,{r4,r5,r6,lr}

	subs	a1,a1,#1	// Start at (n-1)th pixel
	addgt	a3,a3,a1,lsl#1	// Modify ptr2 to start at the end of the span
				// ptr2 = ptr2 + (n * 2)
	addgt	a2,a2,a1,lsl#2	// Modify rgba to end of the span
				// rgba = rgba + (n * 4)

loop_start_rgba_span_um2:

	ldr	r4,[a2],#-4	// load word from rgba

	// Convert rgb values to 16 bpp format
	// (Note that Mesa internally stores rgba as R(0) G(1) B(2) A(3))

	// Upper byte (blue and upper green):
	and	r5,r4,#0x00f80000	// blue (5 bits)
	and	r6,r4,#0x0000c000	// upper green (3 bits)
	orr	r5,r5,r6,lsl#3		// now we have |0|bbbbbggg|0|0|

	// Since we don't have access to a store half-word instruction on this
	// architecture then we will have to shift this value in order to
	// write a byte
	mov	r5,r5,lsr#17
	strb	r5,[a3,#1]

	// Lower byte (lower green and red):
	and	r6,r4,#0x00003800	// lower green (3 bits)
	and	r4,r4,#0x000000f8	// red (5 bits)
	mov	r4,r4,lsr#3		// |0|0|0|000rrrrr|
	orr	r4,r4,r6,lsr#6		// |0|0|0|gggrrrrr|

	// Again, the lack of a half-word store instruction means that instead
	// of doing this:
//	and	r4,r4,r5,lsr#8		// |0|0|bbbbbggg|gggrrrrr|
	// and writing the following as a half-word, we have to store a byte

	strb	r4,[a3],#-2	// store byte at ptr2

	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_rgba_span_um2	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,pc}^



// -----	Unmasked rgba pixel span (dithered, even) -----

	.globl	asm_write_rgba_span_unmasked_dither_even2

//	Inputs:			a1 = n		(r0)
//				a2 = &rgba	(r2)
//				a3 = &ptr2	(r3)

asm_write_rgba_span_unmasked_dither_even2:

	stmfd	sp!,{r4,r5,r6,lr}

	subs	a1,a1,#1	// Start at (n-1)th pixel

	addgt	a3,a3,a1,lsl#1	// Modify ptr2 to start at the end of the span
				// ptr2 = ptr2 + (n * 2)
	addgt	a2,a2,a1,lsl#2	// Modify rgba to end of the array
				// rgba = rgba + (n * 4)

	mov	a4,#0		// even start

loop_start_rgba_span_ude2:

	ldr	r4,[a2],#-4		// load word from rgba if mask is non-zero

	eors	a4,a4,#1		// if was zero then write as usual

	// Upper byte (blue and upper green):
	andne	r5,r4,#0x00f80000	// blue (5 bits)
	andne	r6,r4,#0x0000c000	// upper green (2 bits)
	orrne	r5,r5,r6,lsl#3		// now we have |0|bbbbbgg0|0|0|

	movne	r5,r5,lsr#17
	strneb	r5,[a3,#1]

	// Lower byte (lower green and red):
	andne	r6,r4,#0x00003800	// lower green (3 bits)
	andne	r4,r4,#0x000000f8	// red (5 bits)
	movne	r4,r4,lsr#3		// |0|0|0|000rrrrr|
	orrne	r4,r4,r6,lsr#6		// |0|0|0|gggrrrrr|

	strneb	r4,[a3],#-2	// store word at ptr2

	// for an odd pixel then we take into account the sixth bit of each component

	// Upper byte (blue and upper green):
	andeq	r5,r4,#0x00f80000	// blue (5 bits)
	andeq	r6,r4,#0x00040000	// blue (sixth bit)
	orreq	r5,r5,r6,lsl#1
	andeq	r6,r4,#0x0000c000	// upper green (2 bits)
	orreq	r5,r5,r6,lsl#3		// now we have |0|bbbbbgg0|0|0|

	moveq	r5,r5,lsr#17
	streqb	r5,[a3,#1]

	// Lower byte (lower green and red):
	andeq	r6,r4,#0x00003800	// lower green (3 bits)
	andeq	r5,r4,#0x00000400	// lower green (sixth bit)
	orreq	r6,r6,r5,lsl#1
	andeq	r5,r4,#0x00000004	// red (sixth bit)
	andeq	r4,r4,#0x000000f8	// red (5 bits)
	orreq	r4,r4,r5,lsl#1
	moveq	r4,r4,lsr#3		// |0|0|0|000rrrrr|
	orreq	r4,r4,r6,lsr#6		// |0|0|0|gggrrrrr|

	streqb	r4,[a3],#-2		// store word at ptr2

	subs	a1,a1,#1			// subtract 1 from the counter
	bge	loop_start_rgba_span_ude2	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,pc}^



// -----	Unmasked rgba pixel span (dithered, odd) -----

	.globl	asm_write_rgba_span_unmasked_dither_odd2

//	Inputs:			a1 = n		(r0)
//				a2 = &rgba	(r2)
//				a3 = &ptr2	(r3)

asm_write_rgba_span_unmasked_dither_odd2:

	stmfd	sp!,{r4,r5,r6,lr}

	subs	a1,a1,#1	// Start at (n-1)th pixel

	addgt	a3,a3,a1,lsl#1	// Modify ptr2 to start at the end of the span
				// ptr2 = ptr2 + (n * 2)
	addgt	a2,a2,a1,lsl#2	// Modify rgba to end of the array
				// rgba = rgba + (n * 4)

	mov	a4,#1		// odd start

loop_start_rgba_span_udo2:

	ldr	r4,[a2],#-4		// load word from rgba if mask is non-zero

	eors	a4,a4,#1		// if was zero then write as usual

	// Upper byte (blue and upper green):
	andne	r5,r4,#0x00f80000	// blue (5 bits)
	andne	r6,r4,#0x0000c000	// upper green (2 bits)
	orrne	r5,r5,r6,lsl#3		// now we have |0|bbbbbgg0|0|0|

	movne	r5,r5,lsr#17
	strneb	r5,[a3,#1]

	// Lower byte (lower green and red):
	andne	r6,r4,#0x00003800	// lower green (3 bits)
	andne	r4,r4,#0x000000f8	// red (5 bits)
	movne	r4,r4,lsr#3		// |0|0|0|000rrrrr|
	orrne	r4,r4,r6,lsr#6		// |0|0|0|gggrrrrr|

	strneb	r4,[a3],#-2	// store word at ptr2

	// for an odd pixel then we take into account the sixth bit of each component

	// Upper byte (blue and upper green):
	andeq	r5,r4,#0x00f80000	// blue (5 bits)
	andeq	r6,r4,#0x00040000	// blue (sixth bit)
	orreq	r5,r5,r6,lsl#1
	andeq	r6,r4,#0x0000c000	// upper green (2 bits)
	orreq	r5,r5,r6,lsl#3		// now we have |0|bbbbbgg0|0|0|

	moveq	r5,r5,lsr#17
	streqb	r5,[a3,#1]

	// Lower byte (lower green and red):
	andeq	r6,r4,#0x00003800	// lower green (3 bits)
	andeq	r5,r4,#0x00000400	// lower green (sixth bit)
	orreq	r6,r6,r5,lsl#1
	andeq	r5,r4,#0x00000004	// red (sixth bit)
	andeq	r4,r4,#0x000000f8	// red (5 bits)
	orreq	r4,r4,r5,lsl#1
	moveq	r4,r4,lsr#3		// |0|0|0|000rrrrr|
	orreq	r4,r4,r6,lsr#6		// |0|0|0|gggrrrrr|

	streqb	r4,[a3],#-2		// store word at ptr2

	subs	a1,a1,#1			// subtract 1 from the counter
	bge	loop_start_rgba_span_udo2	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,pc}^



// -----	Masked rgb pixel span -----

	.globl	asm_write_rgb_span_masked2

//	Inputs:			a1 = n		(r0)
//				a2 = &mask	(r1)
//				a3 = &rgba	(r2)
//				a4 = &ptr2	(r3)

asm_write_rgb_span_masked2:

	stmfd	sp!,{r4,r5,r6,lr}

	sub	a1,a1,#1	// Start at (n-1)th pixel
	add	a4,a4,a1,lsl#1	// Modify ptr2 to start at the end of the span
	add	a3,a3,a1,lsl#1	// Modify rgba to end of the array
	add	a3,a3,a1	// rgba = rgba + (n * 3)

loop_start_rgb_span_m2:

	ldrb	r4,[a2,a1]	// load value from the mask array
	cmp	r4,#0		// if it is zero then
	subeq	a4,a4,#2	// update ptr2
//	beq	next_pixel_rgb_span_m	// jump to next pixel

	ldrneb	r4,[a3,#2]	// load blue byte
	andne	r4,r4,#0xf8	// r4 = bbbbb000
	ldrneb	r5,[a3,#1]	// load green byte (r5)
	movne	r5,r5,lsr#3	// r5 = 000ggggg (top five bits)
	movne	r6,r5,lsr#3	// r6 = 000000gg (top two bits)
	orrne	r4,r6,r4,lsr#1	// r4 = 0bbbbbgg
	strneb	r4,[a4,#1]

	ldrneb	r4,[a3],#-3	// load red byte (decreasing rgba at the same time)
	movne	r4,r4,lsr#5	// r4 = 000rrrrr
	andne	r5,r5,#7	// r5 = 00000ggg (lower five bytes from the top of green)
	orrne	r4,r4,r5,lsl#5	// gggrrrrr
	strneb	r4,[a4],#-2

//next_pixel_rgb_span_m2
	subeq	a3,a3,#3	// update rgba
	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_rgb_span_m2	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,pc}^



// -----	Masked rgb pixel span (dither, even) -----

	.globl	asm_write_rgb_span_masked_dither_even2

//	Inputs:			a1 = n		(r0)
//				a2 = &mask	(r1)
//				a3 = &rgba	(r2)
//				a4 = &ptr2	(r3)

asm_write_rgb_span_masked_dither_even2:

	stmfd	sp!,{r4,r5,r6,r7,lr}

	sub	a1,a1,#1	// Start at (n-1)th pixel
	add	a4,a4,a1,lsl#1	// Modify ptr2 to start at the end of the span
	add	a3,a3,a1,lsl#1	// Modify rgba to end of the array
	add	a3,a3,a1	// rgba = rgba + (n * 3)

	mov	r7,#0		// even start

loop_start_rgb_span_mde2:

	ldrb	r4,[a2,a1]	// load value from the mask array
	cmp	r4,#0		// if it is zero then
	subeq	a4,a4,#2	// update ptr2
	subeq	a3,a3,#3	// update rgba
	eoreq	r7,r7,#1	// toggle the even-odd counter
	beq	next_pixel_rgb_span_mde2	// jump to next pixel

	eors	r7,r7,#1	// test for even pixels

	ldrneb	r4,[a3,#2]	// load blue byte
	andne	r4,r4,#0xf8	// r4 = bbbbb000
	ldrneb	r5,[a3,#1]	// load green byte (r5)
	movne	r5,r5,lsr#3	// r5 = 000ggggg (top five bits)
	movne	r6,r5,lsr#3	// r6 = 000000gg (top two bits)
	orrne	r4,r6,r4,lsr#1	// r4 = 0bbbbbgg
	strneb	r4,[a4,#1]

	ldrneb	r4,[a3],#-3	// load red byte (decreasing rgba at the same time)
	movne	r4,r4,lsr#5	// r4 = 000rrrrr
	andne	r5,r5,#7	// r5 = 00000ggg (lower five bytes from the top of green)
	orrne	r4,r4,r5,lsl#5	// gggrrrrr
	strneb	r4,[a4],#-2

	// for an odd pixel we take into account the sixth bit of each component
	ldreqb	r4,[a3,#2]	// load blue byte
	andeq	r5,r4,#0x04	// r5 = 00000b00
	andeq	r4,r4,#0xf8	// r4 = bbbbb000
	orreq	r4,r4,r5,lsl#1
	ldreqb	r5,[a3,#1]	// load green byte (r5)
	moveq	r5,r5,lsr#2	// r5 = 000ggggg (top six bits)
	moveq	r6,r5,lsr#4	// r6 = 000000gg (top two bits)
	orreq	r4,r6,r4,lsr#1	// r4 = 0bbbbbgg
	streqb	r4,[a4,#1]

	ldreqb	r4,[a3],#-3	// load red byte (decreasing rgba at the same time)
	andeq	r6,r4,#0x04	// r6 = 0000r000
	moveq	r4,r4,lsr#5	// r4 = 000rrrrr
	orreq	r4,r4,r6,lsl#1
	andeq	r6,r5,#0x01	// sixth bit of green
	andeq	r5,r5,#0x0e	// r5 = 0000ggg0 (three bytes from the top of green)
	orreq	r5,r5,r6,lsl#1
	orreq	r4,r4,r5,lsl#4	// gggrrrrr
	streqb	r4,[a4],#-2


next_pixel_rgb_span_mde2:
	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_rgb_span_mde2	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,r7,pc}^



// -----	Masked rgb pixel span (dither, odd) -----

	.globl	asm_write_rgb_span_masked_dither_odd2

//	Inputs:			a1 = n		(r0)
//				a2 = &mask	(r1)
//				a3 = &rgba	(r2)
//				a4 = &ptr2	(r3)

asm_write_rgb_span_masked_dither_odd2:

	stmfd	sp!,{r4,r5,r6,r7,lr}

	sub	a1,a1,#1	// Start at (n-1)th pixel
	add	a4,a4,a1,lsl#1	// Modify ptr2 to start at the end of the span
	add	a3,a3,a1,lsl#1	// Modify rgba to end of the array
	add	a3,a3,a1	// rgba = rgba + (n * 3)

	mov	r7,#1		// odd start

loop_start_rgb_span_mdo2:

	ldrb	r4,[a2,a1]	// load value from the mask array
	cmp	r4,#0		// if it is zero then
	subeq	a4,a4,#2	// update ptr2
	subeq	a3,a3,#3	// update rgba
	eoreq	r7,r7,#1	// toggle the even-odd counter
	beq	next_pixel_rgb_span_mdo2	// jump to next pixel

	eors	r7,r7,#1	// test for even pixels

	ldrneb	r4,[a3,#2]	// load blue byte
	andne	r4,r4,#0xf8	// r4 = bbbbb000
	ldrneb	r5,[a3,#1]	// load green byte (r5)
	movne	r5,r5,lsr#3	// r5 = 000ggggg (top five bits)
	movne	r6,r5,lsr#3	// r6 = 000000gg (top two bits)
	orrne	r4,r6,r4,lsr#1	// r4 = 0bbbbbgg
	strneb	r4,[a4,#1]

	ldrneb	r4,[a3],#-3	// load red byte (decreasing rgba at the same time)
	movne	r4,r4,lsr#5	// r4 = 000rrrrr
	andne	r5,r5,#7	// r5 = 00000ggg (lower five bytes from the top of green)
	orrne	r4,r4,r5,lsl#5	// gggrrrrr
	strneb	r4,[a4],#-2

	// for an odd pixel we take into account the sixth bit of each component
	ldreqb	r4,[a3,#2]	// load blue byte
	andeq	r5,r4,#0x04	// r5 = 00000b00
	andeq	r4,r4,#0xf8	// r4 = bbbbb000
	orreq	r4,r4,r5,lsl#1
	ldreqb	r5,[a3,#1]	// load green byte (r5)
	moveq	r5,r5,lsr#2	// r5 = 000ggggg (top six bits)
	moveq	r6,r5,lsr#4	// r6 = 000000gg (top two bits)
	orreq	r4,r6,r4,lsr#1	// r4 = 0bbbbbgg
	streqb	r4,[a4,#1]

	ldreqb	r4,[a3],#-3	// load red byte (decreasing rgba at the same time)
	andeq	r6,r4,#0x04	// r6 = 0000r000
	moveq	r4,r4,lsr#5	// r4 = 000rrrrr
	orreq	r4,r4,r6,lsl#1
	andeq	r6,r5,#0x01	// sixth bit of green
	andeq	r5,r5,#0x0e	// r5 = 0000ggg0 (three bytes from the top of green)
	orreq	r5,r5,r6,lsl#1
	orreq	r4,r4,r5,lsl#4	// gggrrrrr
	streqb	r4,[a4],#-2


next_pixel_rgb_span_mdo2:
	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_rgb_span_mdo2	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,r7,pc}^



// -----	Unmasked rgb pixel span -----

	.globl	asm_write_rgb_span_unmasked2

//	Inputs:			a1 = n		(r0)
//				a2 = &rgba	(r1)
//				a3 = &ptr2	(r2)

asm_write_rgb_span_unmasked2:

	stmfd	sp!,{r4,r5,r6,lr}

	sub	a1,a1,#1	// Start at (n-1)th pixel
	add	a3,a3,a1,lsl#1	// Modify ptr2 to start at the end of the span
	add	a2,a2,a1,lsl#1	// Modify rgba to end of the span
	add	a2,a2,a1	// rgba = rgba + (n * 3)

loop_start_rgb_span_um2:

	ldrb	r4,[a2,#2]	// load blue byte
	and	r4,r4,#0xf8	// r4 = bbbbb000
	ldrb	r5,[a2,#1]	// load green byte (r5)
	mov	r5,r5,lsr#3	// r5 = 000ggggg (top five bits)
	mov	r6,r5,lsr#3	// r6 = 000000gg (top two bits)
	orr	r4,r6,r4,lsr#1	// r4 = 0bbbbbgg
	strb	r4,[a3,#1]

	ldrb	r4,[a2],#-3	// load red byte (decreasing rgba at the same time)
	mov	r4,r4,lsr#5	// r4 = 000rrrrr
	and	r5,r5,#7	// r5 = 00000ggg (lower five bytes from the top of green)
	orr	r4,r4,r5,lsl#5	// gggrrrrr
	strb	r4,[a3],#-2

	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_rgb_span_um2	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,pc}^



// -----	Unmasked rgb pixel span (dither, even) -----

	.globl	asm_write_rgb_span_unmasked_dither_even2

//	Inputs:			a1 = n		(r0)
//				a2 = &rgba	(r2)
//				a3 = &ptr2	(r3)

asm_write_rgb_span_unmasked_dither_even2:

	stmfd	sp!,{r4,r5,r6,lr}

	sub	a1,a1,#1	// Start at (n-1)th pixel
	add	a4,a4,a1,lsl#1	// Modify ptr2 to start at the end of the span
	add	a3,a3,a1,lsl#1	// Modify rgba to end of the array
	add	a3,a3,a1	// rgba = rgba + (n * 3)

	mov	a4,#0		// even start

loop_start_rgb_span_ude2:

	eors	a4,a4,#1	// test for even pixels

	ldrneb	r4,[a2,#2]	// load blue byte
	andne	r4,r4,#0xf8	// r4 = bbbbb000
	ldrneb	r5,[a2,#1]	// load green byte (r5)
	movne	r5,r5,lsr#3	// r5 = 000ggggg (top five bits)
	movne	r6,r5,lsr#3	// r6 = 000000gg (top two bits)
	orrne	r4,r6,r4,lsr#1	// r4 = 0bbbbbgg
	strneb	r4,[a3,#1]

	ldrneb	r4,[a2],#-3	// load red byte (decreasing rgba at the same time)
	movne	r4,r4,lsr#5	// r4 = 000rrrrr
	andne	r5,r5,#7	// r5 = 00000ggg (lower five bytes from the top of green)
	orrne	r4,r4,r5,lsl#5	// gggrrrrr
	strneb	r4,[a3],#-2

	// for an odd pixel we take into account the sixth bit of each component
	ldreqb	r4,[a2,#2]	// load blue byte
	andeq	r5,r4,#0x04	// r5 = 00000b00
	andeq	r4,r4,#0xf8	// r4 = bbbbb000
	orreq	r4,r4,r5,lsl#1
	ldreqb	r5,[a2,#1]	// load green byte (r5)
	moveq	r5,r5,lsr#2	// r5 = 000ggggg (top six bits)
	moveq	r6,r5,lsr#4	// r6 = 000000gg (top two bits)
	orreq	r4,r6,r4,lsr#1	// r4 = 0bbbbbgg
	streqb	r4,[a3,#1]

	ldreqb	r4,[a2],#-3	// load red byte (decreasing rgba at the same time)
	andeq	r6,r4,#0x04	// r6 = 0000r000
	moveq	r4,r4,lsr#5	// r4 = 000rrrrr
	orreq	r4,r4,r6,lsl#1
	andeq	r6,r5,#0x01	// sixth bit of green
	andeq	r5,r5,#0x0e	// r5 = 0000ggg0 (three bytes from the top of green)
	orreq	r5,r5,r6,lsl#1
	orreq	r4,r4,r5,lsl#4	// gggrrrrr
	streqb	r4,[a3],#-2

	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_rgb_span_ude2	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,pc}^



// -----	Unmasked rgb pixel span (dither, odd) -----

	.globl	asm_write_rgb_span_unmasked_dither_odd2

//	Inputs:			a1 = n		(r0)
//				a2 = &rgba	(r2)
//				a3 = &ptr2	(r3)

asm_write_rgb_span_unmasked_dither_odd2:

	stmfd	sp!,{r4,r5,r6,lr}

	sub	a1,a1,#1	// Start at (n-1)th pixel
	add	a4,a4,a1,lsl#1	// Modify ptr2 to start at the end of the span
	add	a3,a3,a1,lsl#1	// Modify rgba to end of the array
	add	a3,a3,a1	// rgba = rgba + (n * 3)

	mov	a4,#1		// odd start

loop_start_rgb_span_udo2:

	eors	a4,a4,#1	// test for even pixels

	ldrneb	r4,[a2,#2]	// load blue byte
	andne	r4,r4,#0xf8	// r4 = bbbbb000
	ldrneb	r5,[a2,#1]	// load green byte (r5)
	movne	r5,r5,lsr#3	// r5 = 000ggggg (top five bits)
	movne	r6,r5,lsr#3	// r6 = 000000gg (top two bits)
	orrne	r4,r6,r4,lsr#1	// r4 = 0bbbbbgg
	strneb	r4,[a3,#1]

	ldrneb	r4,[a2],#-3	// load red byte (decreasing rgba at the same time)
	movne	r4,r4,lsr#5	// r4 = 000rrrrr
	andne	r5,r5,#7	// r5 = 00000ggg (lower five bytes from the top of green)
	orrne	r4,r4,r5,lsl#5	// gggrrrrr
	strneb	r4,[a3],#-2

	// for an odd pixel we take into account the sixth bit of each component
	ldreqb	r4,[a2,#2]	// load blue byte
	andeq	r5,r4,#0x04	// r5 = 00000b00
	andeq	r4,r4,#0xf8	// r4 = bbbbb000
	orreq	r4,r4,r5,lsl#1
	ldreqb	r5,[a2,#1]	// load green byte (r5)
	moveq	r5,r5,lsr#2	// r5 = 000ggggg (top six bits)
	moveq	r6,r5,lsr#4	// r6 = 000000gg (top two bits)
	orreq	r4,r6,r4,lsr#1	// r4 = 0bbbbbgg
	streqb	r4,[a3,#1]

	ldreqb	r4,[a2],#-3	// load red byte (decreasing rgba at the same time)
	andeq	r6,r4,#0x04	// r6 = 0000r000
	moveq	r4,r4,lsr#5	// r4 = 000rrrrr
	orreq	r4,r4,r6,lsl#1
	andeq	r6,r5,#0x01	// sixth bit of green
	andeq	r5,r5,#0x0e	// r5 = 0000ggg0 (three bytes from the top of green)
	orreq	r5,r5,r6,lsl#1
	orreq	r4,r4,r5,lsl#4	// gggrrrrr
	streqb	r4,[a3],#-2

	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_rgb_span_udo2	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,pc}^



// -----	Write rgba pixels -----

	.globl	asm_write_rgba_pixels2

//	Inputs:			a1 = n
//				a2 = x[]
//				a3 = y[]
//				a4 = parameters[] (rgba[], mask[], buffer end, width)

asm_write_rgba_pixels2:

	stmfd	sp!,{r4,r5,r6,r7,r8,r9,r10,lr}

	sub	a1,a1,#1	// Start at (n-1)th pixel
	ldr	r5,[a4]		// r5 = rgba[]
	ldr	r6,[a4,#4]	// r6 = mask[]
	ldr	r7,[a4,#8]	// r7 = buffer end
	ldr	a4,[a4,#12]	// a4 = width
	mov	a4,a4,lsl#1	// a4 = width * pixel depth

loop_start_write_rgba_pixels2:

	ldrb	r8,[r6,a1]	// load value from the mask array
	cmp	r8,#0
//	beq	next_pixel_write_rgba_pixels2

	ldrne	r4,[a2,a1,lsl#2]// load x position
	movne	r4,r4,lsl#1	// pixel = x * pixel size
	ldrne	r8,[a3,a1,lsl#2]// load y position
	mulne	r8,a4,r8	// y * width
	subne	r4,r4,r8	// x - (y * width)
	addne	r4,r4,r7	// buffer end + x - (y * width)

	ldrne	r8,[r5,a1,lsl#2]// load word from rgba if mask is non-zero

	// Convert rgb values to 16 bpp format
	// (Note that Mesa internally stores rgba as R(0) G(1) B(2) A(3)
	// so that reading a word containing these bytes gives
	// aaaaaaaabbbbbbbbggggggggrrrrrrrr

	// Upper byte (blue and upper green):
	andne	r9,r8,#0x00f80000	// blue (5 bits)
	andne	r10,r8,#0x0000c000	// upper green (2 bits)
	orrne	r9,r9,r10,lsl#3		// now we have |0|bbbbbgg0|0|0|

	// Since we don't have access to a store half-word instruction on this
	// architecture then we will have to shift this value in order to
	// write a byte
	movne	r9,r9,lsr#17
	strneb	r9,[r4,#1]

	// Lower byte (lower green and red):
	andne	r10,r8,#0x00003800	// lower green (3 bits)
	andne	r8,r8,#0x000000f8	// red (5 bits)
	movne	r8,r8,lsr#3		// |0|0|0|000rrrrr|
	orrne	r8,r8,r10,lsr#6		// |0|0|0|gggrrrrr|

	// Again, the lack of a half-word store instruction means that instead
	// of doing this:
//	andne	r4,r4,r5,lsr#8		// |0|0|0bbbbbgg|gggrrrrr|
	// and writing the following as a half-word, we have to store a byte

	strneb	r8,[r4]		// store word at buffer + x - (y * width)

//next_pixel_write_rgba_pixels2

	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_write_rgba_pixels2	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,r7,r8,r9,r10,pc}^



// -----	Read rgba span -----

	.globl	asm_read_rgba_span2

//	Inputs:			a1 = n		(r0)
//				a2 = &rgba	(r1)
//				a3 = &ptr2	(r2)

asm_read_rgba_span2:

	stmfd	sp!,{r4,r5,r6,lr}

	sub	a1,a1,#1	// Start at (n-1)th pixel
	add	a3,a3,a1,lsl#1	// Modify ptr2 to start at the end of the span
	add	a2,a2,a1,lsl#2	// Modify rgba to end of the array

loop_start_rgba_read_span2:

	// Convert 16 bpp values to rgba format
	// (Note that Mesa internally stores rgba as R(0) G(1) B(2) A(3))

	// Upper byte (blue and upper green):
	ldrb	r4,[a3,#1]	// load value from ptr2 + 1 (0bbbbbgg)
	and	r5,r4,#0x7c	// extract the blue component (0bbbbb00)
	mov	r5,r5,lsl#17	// r5 = bbbbb000 00000000 00000000
	and	r4,r4,#0x3	// r4 = 000000gg
	orr	r5,r5,r4,lsl#14	// r5 = bbbbb000 gg000000 00000000
	// Lower byte (lower green and red):
	ldrb	r6,[a3],#-2	// load value from ptr2 (gggrrrrr)
	and	r4,r6,#0xe0	// r4 = ggg00000
	orr	r5,r5,r4,lsl#6	// r5 = bbbbb000 ggggg000 00000000
	and	r6,r6,#0x1f	// r6 = 000rrrrr
	orr	r5,r5,r6,lsl#3	// r5 = bbbbb000 ggggg000 rrrrr000

	str	r5,[a2],#-4	// store byte at ptr2

	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_rgba_read_span2	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,pc}^



// -----	Read rgba pixels -----

	.globl	asm_read_rgba_pixels2

//	Inputs:			a1 = n
//				a2 = x[]
//				a3 = y[]
//				a4 = parameters[] (rgba[], mask[], buffer end, width)

asm_read_rgba_pixels2:

	stmfd	sp!,{r4,r5,r6,r7,r8,r9,r10,lr}

	sub	a1,a1,#1	// Start at (n-1)th pixel
	ldr	r5,[a4]		// r5 = rgba[]
	ldr	r6,[a4,#4]	// r6 = mask[]
	ldr	r7,[a4,#8]	// r7 = buffer end
	ldr	a4,[a4,#12]	// a4 = width
	mov	a4,a4,lsl#1	// a4 = width * pixel depth

loop_start_read_rgba_pixels2:

	ldrb	r8,[r6,a1]	// load value from the mask array
	cmp	r8,#0
//	beq	next_pixel_read_rgba_pixels2

	ldrne	r4,[a2,a1,lsl#2]// load x position
	movne	r4,r4,lsl#1	// pixel = x * pixel size
	ldrne	r8,[a3,a1,lsl#2]// load y position
	mulne	r8,a4,r8	// y * width
	subne	r4,r4,r8	// x - (y * width)
	addne	r4,r4,r7	// buffer end + x - (y * width)

	ldrneb	r8,[r4,#1]	// load upper byte from buffer + x - (y * width)

	andne	r9,r8,#0x7c	// extract the blue component (0bbbbb00)
	movne	r9,r9,lsl#17	// r9 = bbbbb000 00000000 00000000
	andne	r8,r8,#0x3	// r8 = 000000gg
	orrne	r9,r9,r8,lsl#14	// r9 = bbbbb000 gg000000 00000000
	// Lower byte (lower green and red):
	ldrneb	r10,[a3],#-2	// load value from ptr2 (gggrrrrr)
	andne	r8,r10,#0xe0	// r8 = ggg00000
	orrne	r9,r9,r8,lsl#6	// r9 = bbbbb000 ggggg000 00000000
	andne	r10,r10,#0x1f	// r10 = 000rrrrr
	orrne	r9,r9,r10,lsl#3	// r9 = bbbbb000 ggggg000 rrrrr000

	strne	r9,[r5,a1,lsl#2]// store word in rgba if mask is non-zero

//next_pixel_read_rgba_pixels2

	subs	a1,a1,#1	// subtract 1 from the counter
	bge	loop_start_read_rgba_pixels2	// repeat if r0 >= 0

	ldmfd	sp!,{r4,r5,r6,r7,r8,r9,r10,pc}^
