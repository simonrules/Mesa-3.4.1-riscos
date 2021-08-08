/* Definitions of assembler functions */

/* 32 bpp functions */

void	asm_write_rgba_span_masked(GLuint, GLuint, GLuint, GLuint);
void	asm_write_rgba_span_unmasked(GLuint, GLuint, GLuint);
void	asm_write_rgb_span_masked(GLuint, GLuint, GLuint, GLuint);
void	asm_write_rgb_span_unmasked(GLuint, GLuint, GLuint);
void	asm_write_monocolor_span(GLuint, GLuint, GLuint, GLuint);
void	asm_write_rgba_pixels(GLuint, GLuint, GLuint, GLuint);
void	asm_write_monocolor_pixels(GLuint, GLuint, GLuint, GLuint);
void	asm_read_rgba_span(GLuint, GLuint, GLuint);
void	asm_read_rgba_pixels(GLuint, GLuint, GLuint, GLuint);

/* 16 bpp functions */

void	asm_write_rgba_span_masked2(GLuint, GLuint, GLuint, GLuint);
void	asm_write_rgba_span_unmasked2(GLuint, GLuint, GLuint);
void	asm_write_rgb_span_masked2(GLuint, GLuint, GLuint, GLuint);
void	asm_write_rgb_span_unmasked2(GLuint, GLuint, GLuint);
void	asm_write_rgba_pixels2(GLuint, GLuint, GLuint, GLuint);
void	asm_read_rgba_span2(GLuint, GLuint, GLuint);
void	asm_read_rgba_pixels2(GLuint, GLuint, GLuint, GLuint);

void	asm_write_rgba_span_masked_dither_even2(GLuint, GLuint, GLuint, GLuint);
void	asm_write_rgba_span_masked_dither_odd2(GLuint, GLuint, GLuint, GLuint);
void	asm_write_rgba_span_unmasked_dither_even2(GLuint, GLuint, GLuint);
void	asm_write_rgba_span_unmasked_dither_odd2(GLuint, GLuint, GLuint);
void	asm_write_rgb_span_masked_dither_even2(GLuint, GLuint, GLuint, GLuint);
void	asm_write_rgb_span_masked_dither_odd2(GLuint, GLuint, GLuint, GLuint);
void	asm_write_rgb_span_unmasked_dither_even2(GLuint, GLuint, GLuint);
void	asm_write_rgb_span_unmasked_dither_odd2(GLuint, GLuint, GLuint);
