/*         ______   ___    ___
 *        /\  _  \ /\_ \  /\_ \
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      Sprite drawing functions.
 *
 *      By Michael Bukin.
 *
 *      See readme.txt for copyright information.
 */

#ifndef __bma_cspr_h
#define __bma_cspr_h

/* _linear_draw_sprite:
 *  Draws a sprite onto a linear bitmap at the specified x, y position,
 *  using a masked drawing mode where zero pixels are not output.
 */
void FUNC_LINEAR_DRAW_SPRITE(BITMAP *dst, BITMAP *src, int dx, int dy)
{
   int x, y, w, h;
   int dxbeg, dybeg;
   int sxbeg, sybeg;

   if (dst->clip) {
      int tmp;

      tmp = dst->cl - dx;
      sxbeg = ((tmp < 0) ? 0 : tmp);
      dxbeg = sxbeg + dx;

      tmp = dst->cr - dx;
      w = ((tmp > src->w) ? src->w : tmp) - sxbeg;
      if (w <= 0)
	 return;

      tmp = dst->ct - dy;
      sybeg = ((tmp < 0) ? 0 : tmp);
      dybeg = sybeg + dy;

      tmp = dst->cb - dy;
      h = ((tmp > src->h) ? src->h : tmp) - sybeg;
      if (h <= 0)
	 return;
   }
   else {
      w = src->w;
      h = src->h;
      sxbeg = 0;
      sybeg = 0;
      dxbeg = dx;
      dybeg = dy;
   }

   if (dst->id & (BMP_ID_VIDEO | BMP_ID_SYSTEM)) {
      bmp_select(dst);

      for (y = 0; y < h; y++) {
	 PIXEL_PTR s = OFFSET_PIXEL_PTR(src->line[sybeg + y], sxbeg);
	 PIXEL_PTR d = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);

	 for (x = w - 1; x >= 0; INC_PIXEL_PTR(s), INC_PIXEL_PTR(d), x--) {
	    unsigned long c = GET_MEMORY_PIXEL(s);
	    if (!IS_SPRITE_MASK(src, c)) {
	       PUT_PIXEL(d, c);
	    }
	 }
      }

      bmp_unwrite_line(dst);
   }
   else {
      for (y = 0; y < h; y++) {
	 PIXEL_PTR s = OFFSET_PIXEL_PTR(src->line[sybeg + y], sxbeg);
	 PIXEL_PTR d = OFFSET_PIXEL_PTR(dst->line[dybeg + y], dxbeg);

	 for (x = w - 1; x >= 0; INC_PIXEL_PTR(s), INC_PIXEL_PTR(d), x--) {
	    unsigned long c = GET_MEMORY_PIXEL(s);
	    if (!IS_SPRITE_MASK(src, c)) {
	       PUT_MEMORY_PIXEL(d, c);
	    }
	 }
      }
   }
}

void FUNC_LINEAR_DRAW_SPRITE_END(void) { }



/* _linear_draw_256_sprite:
 *  Draws a 256 coor sprite onto a linear bitmap at the specified x, y
 *  position, using a masked drawing mode where zero pixels are not output.
 */
void FUNC_LINEAR_DRAW_256_SPRITE(BITMAP *dst, BITMAP *src, int dx, int dy)
{
   int x, y, w, h;
   int dxbeg, dybeg;
   int sxbeg, sybeg;
   int *table;

   if (dst->clip) {
      int tmp;

      tmp = dst->cl - dx;
      sxbeg = ((tmp < 0) ? 0 : tmp);
      dxbeg = sxbeg + dx;

      tmp = dst->cr - dx;
      w = ((tmp > src->w) ? src->w : tmp) - sxbeg;
      if (w <= 0)
	 return;

      tmp = dst->ct - dy;
      sybeg = ((tmp < 0) ? 0 : tmp);
      dybeg = sybeg + dy;

      tmp = dst->cb - dy;
      h = ((tmp > src->h) ? src->h : tmp) - sybeg;
      if (h <= 0)
	 return;
   }
   else {
      w = src->w;
      h = src->h;
      sxbeg = 0;
      sybeg = 0;
      dxbeg = dx;
      dybeg = dy;
   }

   table = _palette_expansion_table(bitmap_color_depth(dst));

   if (dst->id & (BMP_ID_VIDEO | BMP_ID_SYSTEM)) {
      bmp_select(dst);

      for (y = 0; y < h; y++) {
	 unsigned char *s = src->line[sybeg + y] + sxbeg;
	 PIXEL_PTR d = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);

	 for (x = w - 1; x >= 0; INC_PIXEL_PTR(s), INC_PIXEL_PTR(d), x--) {
	    unsigned long c = *s;
	    if (c != 0) {
	       c = table[c];
	       PUT_PIXEL(d, c);
	    }
	 }
      }

      bmp_unwrite_line(dst);
   }
   else {
      for (y = 0; y < h; y++) {
	 unsigned char *s = src->line[sybeg + y] + sxbeg;
	 PIXEL_PTR d = OFFSET_PIXEL_PTR(dst->line[dybeg + y], dxbeg);

	 for (x = w - 1; x >= 0; INC_PIXEL_PTR(s), INC_PIXEL_PTR(d), x--) {
	    unsigned long c = *s;
	    if (c != 0) {
	       c = table[c];
	       PUT_MEMORY_PIXEL(d, c);
	    }
	 }
      }
   }
}



/* _linear_draw_sprite_v_flip:
 *  Draws a sprite to a linear bitmap, flipping vertically.
 */
void FUNC_LINEAR_DRAW_SPRITE_V_FLIP(BITMAP *dst, BITMAP *src, int dx, int dy)
{
   int x, y, w, h;
   int dxbeg, dybeg;
   int sxbeg, sybeg;

   if (dst->clip) {
      int tmp;

      tmp = dst->cl - dx;
      sxbeg = ((tmp < 0) ? 0 : tmp);
      dxbeg = sxbeg + dx;

      tmp = dst->cr - dx;
      w = ((tmp > src->w) ? src->w : tmp) - sxbeg;
      if (w <= 0)
	 return;

      tmp = dst->ct - dy;
      sybeg = ((tmp < 0) ? 0 : tmp);
      dybeg = sybeg + dy;

      tmp = dst->cb - dy;
      h = ((tmp > src->h) ? src->h : tmp) - sybeg;
      if (h <= 0)
	 return;

      dybeg += h - 1;
   }
   else {
      w = src->w;
      h = src->h;
      sxbeg = 0;
      sybeg = 0;
      dxbeg = dx;
      dybeg = dy + h - 1;
   }

   if (dst->id & (BMP_ID_VIDEO | BMP_ID_SYSTEM)) {
      bmp_select(dst);

      for (y = 0; y < h; y++) {
	 PIXEL_PTR s = OFFSET_PIXEL_PTR(src->line[sybeg + y], sxbeg);
	 PIXEL_PTR d = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg - y), dxbeg);

	 for (x = w - 1; x >= 0; INC_PIXEL_PTR(s), INC_PIXEL_PTR(d), x--) {
	    unsigned long c = GET_MEMORY_PIXEL(s);
	    if (!IS_SPRITE_MASK(src, c)) {
	       PUT_PIXEL(d, c);
	    }
	 }
      }

      bmp_unwrite_line(dst);
   }
   else {
      for (y = 0; y < h; y++) {
	 PIXEL_PTR s = OFFSET_PIXEL_PTR(src->line[sybeg + y], sxbeg);
	 PIXEL_PTR d = OFFSET_PIXEL_PTR(dst->line[dybeg - y], dxbeg);

	 for (x = w - 1; x >= 0; INC_PIXEL_PTR(s), INC_PIXEL_PTR(d), x--) {
	    unsigned long c = GET_MEMORY_PIXEL(s);
	    if (!IS_SPRITE_MASK(src, c)) {
	       PUT_MEMORY_PIXEL(d, c);
	    }
	 }
      }
   }
}



/* _linear_draw_sprite_h_flip:
 *  Draws a sprite to a linear bitmap, flipping horizontally.
 */
void FUNC_LINEAR_DRAW_SPRITE_H_FLIP(BITMAP *dst, BITMAP *src, int dx, int dy)
{
   int x, y, w, h;
   int dxbeg, dybeg;
   int sxbeg, sybeg;

   if (dst->clip) {
      int tmp;

      tmp = dst->cl - dx;
      sxbeg = ((tmp < 0) ? 0 : tmp);
      dxbeg = sxbeg + dx;

      tmp = dst->cr - dx;
      w = ((tmp > src->w) ? src->w : tmp) - sxbeg;
      if (w <= 0)
	 return;

      dxbeg += w - 1;

      tmp = dst->ct - dy;
      sybeg = ((tmp < 0) ? 0 : tmp);
      dybeg = sybeg + dy;

      tmp = dst->cb - dy;
      h = ((tmp > src->h) ? src->h : tmp) - sybeg;
      if (h <= 0)
	 return;
   }
   else {
      w = src->w;
      h = src->h;
      sxbeg = 0;
      sybeg = 0;
      dxbeg = dx + w - 1;
      dybeg = dy;
   }

   if (dst->id & (BMP_ID_VIDEO | BMP_ID_SYSTEM)) {
      bmp_select(dst);

      for (y = 0; y < h; y++) {
	 PIXEL_PTR s = OFFSET_PIXEL_PTR(src->line[sybeg + y], sxbeg);
	 PIXEL_PTR d = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);

	 for (x = w - 1; x >= 0; INC_PIXEL_PTR(s), DEC_PIXEL_PTR(d), x--) {
	    unsigned long c = GET_MEMORY_PIXEL(s);
	    if (!IS_SPRITE_MASK(src, c)) {
	       PUT_PIXEL(d, c);
	    }
	 }
      }

      bmp_unwrite_line(dst);
   }
   else {
      for (y = 0; y < h; y++) {
	 PIXEL_PTR s = OFFSET_PIXEL_PTR(src->line[sybeg + y], sxbeg);
	 PIXEL_PTR d = OFFSET_PIXEL_PTR(dst->line[dybeg + y], dxbeg);

	 for (x = w - 1; x >= 0; INC_PIXEL_PTR(s), DEC_PIXEL_PTR(d), x--) {
	    unsigned long c = GET_MEMORY_PIXEL(s);
	    if (!IS_SPRITE_MASK(src, c)) {
	       PUT_MEMORY_PIXEL(d, c);
	    }
	 }
      }
   }
}



/* _linear_draw_sprite_vh_flip:
 *  Draws a sprite to a linear bitmap, flipping both vertically and horizontally.
 */
void FUNC_LINEAR_DRAW_SPRITE_VH_FLIP(BITMAP *dst, BITMAP *src, int dx, int dy)
{
   int x, y, w, h;
   int dxbeg, dybeg;
   int sxbeg, sybeg;

   if (dst->clip) {
      int tmp;

      tmp = dst->cl - dx;
      sxbeg = ((tmp < 0) ? 0 : tmp);
      dxbeg = sxbeg + dx;

      tmp = dst->cr - dx;
      w = ((tmp > src->w) ? src->w : tmp) - sxbeg;
      if (w <= 0)
	 return;

      dxbeg += w - 1;

      tmp = dst->ct - dy;
      sybeg = ((tmp < 0) ? 0 : tmp);
      dybeg = sybeg + dy;

      tmp = dst->cb - dy;
      h = ((tmp > src->h) ? src->h : tmp) - sybeg;
      if (h <= 0)
	 return;

      dybeg += h - 1;
   }
   else {
      w = src->w;
      h = src->h;
      sxbeg = 0;
      sybeg = 0;
      dxbeg = dx + w - 1;
      dybeg = dy + h - 1;
   }

   if (dst->id & (BMP_ID_VIDEO | BMP_ID_SYSTEM)) {
      bmp_select(dst);

      for (y = 0; y < h; y++) {
	 PIXEL_PTR s = OFFSET_PIXEL_PTR(src->line[sybeg + y], sxbeg);
	 PIXEL_PTR d = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg - y), dxbeg);

	 for (x = w - 1; x >= 0; INC_PIXEL_PTR(s), DEC_PIXEL_PTR(d), x--) {
	    unsigned long c = GET_MEMORY_PIXEL(s);
	    if (!IS_SPRITE_MASK(src, c)) {
	       PUT_PIXEL(d, c);
	    }
	 }
      }

      bmp_unwrite_line(dst);
   }
   else {
      for (y = 0; y < h; y++) {
	 PIXEL_PTR s = OFFSET_PIXEL_PTR(src->line[sybeg + y], sxbeg);
	 PIXEL_PTR d = OFFSET_PIXEL_PTR(dst->line[dybeg - y], dxbeg);

	 for (x = w - 1; x >= 0; INC_PIXEL_PTR(s), DEC_PIXEL_PTR(d), x--) {
	    unsigned long c = GET_MEMORY_PIXEL(s);
	    if (!IS_SPRITE_MASK(src, c)) {
	       PUT_MEMORY_PIXEL(d, c);
	    }
	 }
      }
   }
}



/* _linear_draw_trans_sprite:
 *  Draws a translucent sprite onto a linear bitmap.
 */
void FUNC_LINEAR_DRAW_TRANS_SPRITE(BITMAP *dst, BITMAP *src, int dx, int dy)
{
   int x, y, w, h;
   int dxbeg, dybeg;
   int sxbeg, sybeg;
   DTS_BLENDER blender;

   if (dst->clip) {
      int tmp;

      tmp = dst->cl - dx;
      sxbeg = ((tmp < 0) ? 0 : tmp);
      dxbeg = sxbeg + dx;

      tmp = dst->cr - dx;
      w = ((tmp > src->w) ? src->w : tmp) - sxbeg;
      if (w <= 0)
	 return;

      tmp = dst->ct - dy;
      sybeg = ((tmp < 0) ? 0 : tmp);
      dybeg = sybeg + dy;

      tmp = dst->cb - dy;
      h = ((tmp > src->h) ? src->h : tmp) - sybeg;
      if (h <= 0)
	 return;
   }
   else {
      w = src->w;
      h = src->h;
      sxbeg = 0;
      sybeg = 0;
      dxbeg = dx;
      dybeg = dy;
   }

   blender = MAKE_DTS_BLENDER();

   if ((src->vtable->color_depth == 8) && (dst->vtable->color_depth != 9)) {
      bmp_select(dst);

      for (y = 0; y < h; y++) {
	 unsigned char *s = src->line[sybeg + y] + sxbeg;
	 PIXEL_PTR ds = OFFSET_PIXEL_PTR(bmp_read_line(dst, dybeg + y), dxbeg);
	 PIXEL_PTR dd = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);

	 for (x = w - 1; x >= 0; s++, INC_PIXEL_PTR(ds), INC_PIXEL_PTR(dd), x--) {
	    unsigned long c = *s;
	    c = DTS_BLEND(blender, GET_PIXEL(ds), c);
	    PUT_PIXEL(dd, c);
	 }
      }

      bmp_unwrite_line(dst);
   }
   else if (dst->id & (BMP_ID_VIDEO | BMP_ID_SYSTEM)) {
      bmp_select(dst);

      for (y = 0; y < h; y++) {
	 PIXEL_PTR s = OFFSET_PIXEL_PTR(src->line[sybeg + y], sxbeg);
	 PIXEL_PTR ds = OFFSET_PIXEL_PTR(bmp_read_line(dst, dybeg + y), dxbeg);
	 PIXEL_PTR dd = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);

	 for (x = w - 1; x >= 0; INC_PIXEL_PTR(s), INC_PIXEL_PTR(ds), INC_PIXEL_PTR(dd), x--) {
	    unsigned long c = GET_MEMORY_PIXEL(s);
	    c = DTS_BLEND(blender, GET_PIXEL(ds), c);
	    PUT_PIXEL(dd, c);
	 }
      }

      bmp_unwrite_line(dst);
   }
   else {
      for (y = 0; y < h; y++) {
	 PIXEL_PTR s = OFFSET_PIXEL_PTR(src->line[sybeg + y], sxbeg);
	 PIXEL_PTR d = OFFSET_PIXEL_PTR(dst->line[dybeg + y], dxbeg);

	 for (x = w - 1; x >= 0; INC_PIXEL_PTR(s), INC_PIXEL_PTR(d), x--) {
	    unsigned long c = GET_MEMORY_PIXEL(s);
	    c = DTS_BLEND(blender, GET_MEMORY_PIXEL(d), c);
	    PUT_MEMORY_PIXEL(d, c);
	 }
      }
   }
}



#if (PP_DEPTH != 8) && (PP_DEPTH != 32)

/* _linear_draw_trans_rgba_sprite:
 *  Draws a translucent RGBA sprite onto a linear bitmap.
 */
void FUNC_LINEAR_DRAW_TRANS_RGBA_SPRITE(BITMAP *dst, BITMAP *src, int dx, int dy)
{
   int x, y, w, h;
   int dxbeg, dybeg;
   int sxbeg, sybeg;
   RGBA_BLENDER blender;

   if (dst->clip) {
      int tmp;

      tmp = dst->cl - dx;
      sxbeg = ((tmp < 0) ? 0 : tmp);
      dxbeg = sxbeg + dx;

      tmp = dst->cr - dx;
      w = ((tmp > src->w) ? src->w : tmp) - sxbeg;
      if (w <= 0)
	 return;

      tmp = dst->ct - dy;
      sybeg = ((tmp < 0) ? 0 : tmp);
      dybeg = sybeg + dy;

      tmp = dst->cb - dy;
      h = ((tmp > src->h) ? src->h : tmp) - sybeg;
      if (h <= 0)
	 return;
   }
   else {
      w = src->w;
      h = src->h;
      sxbeg = 0;
      sybeg = 0;
      dxbeg = dx;
      dybeg = dy;
   }

   blender = MAKE_RGBA_BLENDER();

   bmp_select(dst);

   for (y = 0; y < h; y++) {
      unsigned long *s = (unsigned long *)src->line[sybeg + y] + sxbeg;
      PIXEL_PTR ds = OFFSET_PIXEL_PTR(bmp_read_line(dst, dybeg + y), dxbeg);
      PIXEL_PTR dd = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);

      for (x = w - 1; x >= 0; s++, INC_PIXEL_PTR(ds), INC_PIXEL_PTR(dd), x--) {
	 unsigned long c = *s;

	 c = RGBA_BLEND(blender, GET_PIXEL(ds), c);
	 PUT_PIXEL(dd, c);
      }
   }

   bmp_unwrite_line(dst);
}

#endif



/* _linear_draw_lit_sprite:
 *  Draws a lit sprite onto a linear bitmap.
 */
void FUNC_LINEAR_DRAW_LIT_SPRITE(BITMAP *dst, BITMAP *src, int dx, int dy, int color)
{
   int x, y, w, h;
   int dxbeg, dybeg;
   int sxbeg, sybeg;
   DLS_BLENDER blender;

   if (dst->clip) {
      int tmp;

      tmp = dst->cl - dx;
      sxbeg = ((tmp < 0) ? 0 : tmp);
      dxbeg = sxbeg + dx;

      tmp = dst->cr - dx;
      w = ((tmp > src->w) ? src->w : tmp) - sxbeg;
      if (w <= 0)
	 return;

      tmp = dst->ct - dy;
      sybeg = ((tmp < 0) ? 0 : tmp);
      dybeg = sybeg + dy;

      tmp = dst->cb - dy;
      h = ((tmp > src->h) ? src->h : tmp) - sybeg;
      if (h <= 0)
	 return;
   }
   else {
      w = src->w;
      h = src->h;
      sxbeg = 0;
      sybeg = 0;
      dxbeg = dx;
      dybeg = dy;
   }

   blender = MAKE_DLS_BLENDER(color);

   if (dst->id & (BMP_ID_VIDEO | BMP_ID_SYSTEM)) {
      bmp_select(dst);

      for (y = 0; y < h; y++) {
	 PIXEL_PTR s = OFFSET_PIXEL_PTR(src->line[sybeg + y], sxbeg);
	 PIXEL_PTR d = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);

	 for (x = w - 1; x >= 0; INC_PIXEL_PTR(s), INC_PIXEL_PTR(d), x--) {
	    unsigned long c = GET_MEMORY_PIXEL(s);

	    if (!IS_MASK(c)) {
	       c = DLS_BLEND(blender, color, c);
	       PUT_PIXEL(d, c);
	    }
	 }
      }

      bmp_unwrite_line(dst);
   }
   else {
      for (y = 0; y < h; y++) {
	 PIXEL_PTR s = OFFSET_PIXEL_PTR(src->line[sybeg + y], sxbeg);
	 PIXEL_PTR d = OFFSET_PIXEL_PTR(dst->line[dybeg + y], dxbeg);

	 for (x = w - 1; x >= 0; INC_PIXEL_PTR(s), INC_PIXEL_PTR(d), x--) {
	    unsigned long c = GET_MEMORY_PIXEL(s);

	    if (!IS_MASK(c)) {
	       c = DLS_BLEND(blender, color, c);
	       PUT_MEMORY_PIXEL(d, c);
	    }
	 }
      }
   }
}



/* _linear_draw_character:
 *  For proportional font output onto a linear bitmap: uses the sprite as
 *  a mask, replacing all set pixels with the specified color.
 */
void FUNC_LINEAR_DRAW_CHARACTER(BITMAP *dst, BITMAP *src, int dx, int dy, int color)
{
   int x, y, w, h;
   int dxbeg, dybeg;
   int sxbeg, sybeg;

   if (dst->clip) {
      int tmp;

      tmp = dst->cl - dx;
      sxbeg = ((tmp < 0) ? 0 : tmp);
      dxbeg = sxbeg + dx;

      tmp = dst->cr - dx;
      w = ((tmp > src->w) ? src->w : tmp) - sxbeg;
      if (w <= 0)
	 return;

      tmp = dst->ct - dy;
      sybeg = ((tmp < 0) ? 0 : tmp);
      dybeg = sybeg + dy;

      tmp = dst->cb - dy;
      h = ((tmp > src->h) ? src->h : tmp) - sybeg;
      if (h <= 0)
	 return;
   }
   else {
      w = src->w;
      h = src->h;
      sxbeg = 0;
      sybeg = 0;
      dxbeg = dx;
      dybeg = dy;
   }

   bmp_select(dst);

   if (_textmode < 0) {
      /* Masked character.  */
      for (y = 0; y < h; y++) {
	 unsigned char *s = src->line[sybeg + y] + sxbeg;
	 PIXEL_PTR d = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);

	 for (x = w - 1; x >= 0; s++, INC_PIXEL_PTR(d), x--) {
	    unsigned long c = *s;

	    if (c != 0) {
	       PUT_PIXEL(d, color);
	    }
	 }
      }
   }
   else {
      /* Opaque character.  */
      for (y = 0; y < h; y++) {
	 unsigned char *s = src->line[sybeg + y] + sxbeg;
	 PIXEL_PTR d = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);

	 for (x = w - 1; x >= 0; s++, INC_PIXEL_PTR(d), x--) {
	    unsigned long c = *s;

	    if (c != 0) {
	       PUT_PIXEL(d, color);
	    }
	    else {
	       PUT_PIXEL(d, _textmode);
	    }
	 }
      }
   }

   bmp_unwrite_line(dst);
}



/* _linear_draw_rle_sprite:
 *  Draws an RLE sprite onto a linear bitmap at the specified position.
 */
void FUNC_LINEAR_DRAW_RLE_SPRITE(BITMAP *dst, AL_CONST RLE_SPRITE *src, int dx, int dy)
{
   int x, y, w, h;
   int dxbeg, dybeg;
   int sxbeg, sybeg;
   RLE_PTR s;

   if (dst->clip) {
      int tmp;

      tmp = dst->cl - dx;
      sxbeg = ((tmp < 0) ? 0 : tmp);
      dxbeg = sxbeg + dx;

      tmp = dst->cr - dx;
      w = ((tmp > src->w) ? src->w : tmp) - sxbeg;
      if (w <= 0)
	 return;

      tmp = dst->ct - dy;
      sybeg = ((tmp < 0) ? 0 : tmp);
      dybeg = sybeg + dy;

      tmp = dst->cb - dy;
      h = ((tmp > src->h) ? src->h : tmp) - sybeg;
      if (h <= 0)
	 return;
   }
   else {
      w = src->w;
      h = src->h;
      sxbeg = 0;
      sybeg = 0;
      dxbeg = dx;
      dybeg = dy;
   }

   s = (RLE_PTR) (src->dat);

   /* Clip top.  */
   for (y = sybeg - 1; y >= 0; y--) {
      long c = *s++;

      while (!RLE_IS_EOL(c)) {
	 if (c > 0)
	    s += c;
	 c = *s++;
      }
   }

   bmp_select(dst);

   /* Visible part.  */
   if (sxbeg || dx+src->w >= dst->cr) {
      for (y = 0; y < h; y++) {
         PIXEL_PTR d = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);
         long c = *s++;

         /* Clip left.  */
         for (x = sxbeg; x > 0; ) {
	    if (RLE_IS_EOL(c))
	       goto next_line;
	    else if (c > 0) {
	       /* Run of solid pixels.  */
	       if ((x - c) >= 0) {
	          /* Fully clipped.  */
	          x -= c;
	          s += c;
	       }
	       else {
	          /* Visible on the right.  */
	          c -= x;
	          s += x;
	          break;
	       }
	    }
	    else {
	       /* Run of transparent pixels.  */
	       if ((x + c) >= 0) {
	          /* Fully clipped.  */
	          x += c;
	       }
	       else {
	          /* Visible on the right.  */
	          c += x;
	          break;
	       }
	    }

	    c = *s++;
         }

         /* Visible part.  */
         for (x = w; x > 0; ) {
	    if (RLE_IS_EOL(c))
	       goto next_line;
	    else if (c > 0) {
	       /* Run of solid pixels.  */
	       if ((x - c) >= 0) {
	          /* Fully visible.  */
	          x -= c;
	          for (c--; c >= 0; s++, INC_PIXEL_PTR(d), c--) {
		     unsigned long col = *s;
		     PUT_PIXEL(d, col);
	          }
	       }
	       else {
	          /* Clipped on the right.  */
	          c -= x;
	          for (x--; x >= 0; s++, INC_PIXEL_PTR(d), x--) {
		     unsigned long col = *s;
		     PUT_PIXEL(d, col);
	          }
	          break;
	       }
	    }
	    else {
	       /* Run of transparent pixels.  */
	       x += c;
	       d = OFFSET_PIXEL_PTR(d, -c);
	    }

	    c = *s++;
         }

         /* Clip right.  */
         while (!RLE_IS_EOL(c)) {
	    if (c > 0)
	       s += c;
	    c = *s++;
         }

      next_line:
      }
   }
   else {
      for (y = 0; y < h; y++) {
         PIXEL_PTR d = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);
         long c = *s++;

         /* Visible part.  */
         for (x = w; x > 0; ) {
	    if (RLE_IS_EOL(c))
	       goto next_line2;
	    else if (c > 0) {
	       /* Run of solid pixels.  */
	       if ((x - c) >= 0) {
	          /* Fully visible.  */
	          x -= c;
	          for (c--; c >= 0; s++, INC_PIXEL_PTR(d), c--) {
		     unsigned long col = *s;
		     PUT_PIXEL(d, col);
	          }
	       }
	       else {
	          /* Clipped on the right.  */
	          c -= x;
	          for (x--; x >= 0; s++, INC_PIXEL_PTR(d), x--) {
		     unsigned long col = *s;
		     PUT_PIXEL(d, col);
	          }
	          break;
	       }
	    }
	    else {
	       /* Run of transparent pixels.  */
	       x += c;
	       d = OFFSET_PIXEL_PTR(d, -c);
	    }

	    c = *s++;
         }

      next_line2:
      }
   }

   bmp_unwrite_line(dst);
}



/* _linear_draw_trans_rle_sprite:
 *  Draws a translucent RLE sprite onto a linear bitmap.
 */
void FUNC_LINEAR_DRAW_TRANS_RLE_SPRITE(BITMAP *dst, AL_CONST RLE_SPRITE *src, int dx, int dy)
{
   int x, y, w, h;
   int dxbeg, dybeg;
   int sxbeg, sybeg;
   RLE_PTR s;
   DTS_BLENDER blender;

   if (dst->clip) {
      int tmp;

      tmp = dst->cl - dx;
      sxbeg = ((tmp < 0) ? 0 : tmp);
      dxbeg = sxbeg + dx;

      tmp = dst->cr - dx;
      w = ((tmp > src->w) ? src->w : tmp) - sxbeg;
      if (w <= 0)
	 return;

      tmp = dst->ct - dy;
      sybeg = ((tmp < 0) ? 0 : tmp);
      dybeg = sybeg + dy;

      tmp = dst->cb - dy;
      h = ((tmp > src->h) ? src->h : tmp) - sybeg;
      if (h <= 0)
	 return;
   }
   else {
      w = src->w;
      h = src->h;
      sxbeg = 0;
      sybeg = 0;
      dxbeg = dx;
      dybeg = dy;
   }

   blender = MAKE_DTS_BLENDER();
   s = (RLE_PTR) (src->dat);

   /* Clip top.  */
   for (y = sybeg - 1; y >= 0; y--) {
      long c = *s++;

      while (!RLE_IS_EOL(c)) {
	 if (c > 0)
	    s += c;
	 c = *s++;
      }
   }

   bmp_select(dst);

   /* Visible part.  */
   if (sxbeg || dx+src->w >= dst->cr) {
      for (y = 0; y < h; y++) {
         PIXEL_PTR ds = OFFSET_PIXEL_PTR(bmp_read_line(dst, dybeg + y), dxbeg);
         PIXEL_PTR dd = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);
         long c = *s++;

         /* Clip left.  */
         for (x = sxbeg; x > 0; ) {
	    if (RLE_IS_EOL(c))
	       goto next_line;
	    else if (c > 0) {
	       /* Run of solid pixels.  */
	       if ((x - c) >= 0) {
	          /* Fully clipped.  */
	          x -= c;
	          s += c;
	       }
	       else {
	          /* Visible on the right.  */
	          c -= x;
	          s += x;
	          break;
	       }
	    }
	    else {
	       /* Run of transparent pixels.  */
	       if ((x + c) >= 0) {
	          /* Fully clipped.  */
	          x += c;
	       }
	       else {
	          /* Visible on the right.  */
	          c += x;
	          break;
	       }
	    }

	    c = *s++;
         }

         /* Visible part.  */
         for (x = w; x > 0; ) {
	    if (RLE_IS_EOL(c))
	       goto next_line;
	    else if (c > 0) {
	       /* Run of solid pixels.  */
	       if ((x - c) >= 0) {
	          /* Fully visible.  */
	          x -= c;
	          for (c--; c >= 0; s++, INC_PIXEL_PTR(ds), INC_PIXEL_PTR(dd), c--) {
		     unsigned long col = DTS_BLEND(blender, GET_PIXEL(ds), *s);
		     PUT_PIXEL(dd, col);
	          }
	       }
	       else {
	          /* Clipped on the right.  */
	          c -= x;
	          for (x--; x >= 0; s++, INC_PIXEL_PTR(ds), INC_PIXEL_PTR(dd), x--) {
		     unsigned long col = DTS_BLEND(blender, GET_PIXEL(ds), *s);
		     PUT_PIXEL(dd, col);
	          }
	          break;
	       }
	    }
	    else {
	       /* Run of transparent pixels.  */
	       x += c;
	       ds = OFFSET_PIXEL_PTR(ds, -c);
	       dd = OFFSET_PIXEL_PTR(dd, -c);
	    }

	    c = *s++;
         }

         /* Clip right.  */
         while (!RLE_IS_EOL(c)) {
	    if (c > 0)
	       s += c;
	    c = *s++;
         }

      next_line:
      }
   }
   else {
      for (y = 0; y < h; y++) {
         PIXEL_PTR ds = OFFSET_PIXEL_PTR(bmp_read_line(dst, dybeg + y), dxbeg);
         PIXEL_PTR dd = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);
         long c = *s++;

         /* Visible part.  */
         for (x = w; x > 0; ) {
	    if (RLE_IS_EOL(c))
	       goto next_line2;
	    else if (c > 0) {
	       /* Run of solid pixels.  */
	       if ((x - c) >= 0) {
	          /* Fully visible.  */
	          x -= c;
	          for (c--; c >= 0; s++, INC_PIXEL_PTR(ds), INC_PIXEL_PTR(dd), c--) {
		     unsigned long col = DTS_BLEND(blender, GET_PIXEL(ds), *s);
		     PUT_PIXEL(dd, col);
	          }
	       }
	       else {
	          /* Clipped on the right.  */
	          c -= x;
	          for (x--; x >= 0; s++, INC_PIXEL_PTR(ds), INC_PIXEL_PTR(dd), x--) {
		     unsigned long col = DTS_BLEND(blender, GET_PIXEL(ds), *s);
		     PUT_PIXEL(dd, col);
	          }
	          break;
	       }
	    }
	    else {
	       /* Run of transparent pixels.  */
	       x += c;
	       ds = OFFSET_PIXEL_PTR(ds, -c);
	       dd = OFFSET_PIXEL_PTR(dd, -c);
	    }

	    c = *s++;
         }

         /* Clip right.  */
         while (!RLE_IS_EOL(c)) {
	    if (c > 0)
	       s += c;
	    c = *s++;
         }

      next_line2:
      }
   }

   bmp_unwrite_line(dst);
}



#if (PP_DEPTH != 8) && (PP_DEPTH != 32)

/* _linear_draw_trans_rgba_rle_sprite:
 *  Draws a translucent RGBA RLE sprite onto a linear bitmap.
 */
void FUNC_LINEAR_DRAW_TRANS_RGBA_RLE_SPRITE(BITMAP *dst, AL_CONST RLE_SPRITE *src, int dx, int dy)
{
   int x, y, w, h;
   int dxbeg, dybeg;
   int sxbeg, sybeg;
   unsigned long *s;
   RGBA_BLENDER blender;

   if (dst->clip) {
      int tmp;

      tmp = dst->cl - dx;
      sxbeg = ((tmp < 0) ? 0 : tmp);
      dxbeg = sxbeg + dx;

      tmp = dst->cr - dx;
      w = ((tmp > src->w) ? src->w : tmp) - sxbeg;
      if (w <= 0)
	 return;

      tmp = dst->ct - dy;
      sybeg = ((tmp < 0) ? 0 : tmp);
      dybeg = sybeg + dy;

      tmp = dst->cb - dy;
      h = ((tmp > src->h) ? src->h : tmp) - sybeg;
      if (h <= 0)
	 return;
   }
   else {
      w = src->w;
      h = src->h;
      sxbeg = 0;
      sybeg = 0;
      dxbeg = dx;
      dybeg = dy;
   }

   blender = MAKE_RGBA_BLENDER();
   s = (unsigned long *) (src->dat);

   /* Clip top.  */
   for (y = sybeg - 1; y >= 0; y--) {
      long c = *s++;

      while (c != MASK_COLOR_32) {
	 if (c > 0)
	    s += c;
	 c = *s++;
      }
   }

   bmp_select(dst);

   /* Visible part.  */
   if (sxbeg || dx+src->w >= dst->cr) {
      for (y = 0; y < h; y++) {
         PIXEL_PTR ds = OFFSET_PIXEL_PTR(bmp_read_line(dst, dybeg + y), dxbeg);
         PIXEL_PTR dd = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);
         long c = *s++;

         /* Clip left.  */
         for (x = sxbeg; x > 0; ) {
	    if (c == MASK_COLOR_32)
	       goto next_line;
	    else if (c > 0) {
	       /* Run of solid pixels.  */
	       if ((x - c) >= 0) {
	          /* Fully clipped.  */
	          x -= c;
	          s += c;
	       }
	       else {
	          /* Visible on the right.  */
	          c -= x;
	          s += x;
	          break;
	       }
	    }
	    else {
	       /* Run of transparent pixels.  */
	       if ((x + c) >= 0) {
	          /* Fully clipped.  */
	          x += c;
	       }
	       else {
	          /* Visible on the right.  */
	          c += x;
	          break;
	       }
	    }

	    c = *s++;
         }

         /* Visible part.  */
         for (x = w; x > 0; ) {
	    if (c == MASK_COLOR_32)
	       goto next_line;
	    else if (c > 0) {
	       /* Run of solid pixels.  */
	       if ((x - c) >= 0) {
	          /* Fully visible.  */
	          x -= c;
	          for (c--; c >= 0; s++, INC_PIXEL_PTR(ds), INC_PIXEL_PTR(dd), c--) {
		     unsigned long col = RGBA_BLEND(blender, GET_PIXEL(ds), *s);
		     PUT_PIXEL(dd, col);
	          }
	       }
	       else {
	          /* Clipped on the right.  */
	          c -= x;
	          for (x--; x >= 0; s++, INC_PIXEL_PTR(ds), INC_PIXEL_PTR(dd), x--) {
		     unsigned long col = RGBA_BLEND(blender, GET_PIXEL(ds), *s);
		     PUT_PIXEL(dd, col);
	          }
	          break;
	       }
	    }
	    else {
	       /* Run of transparent pixels.  */
	       x += c;
	       ds = OFFSET_PIXEL_PTR(ds, -c);
	       dd = OFFSET_PIXEL_PTR(dd, -c);
	    }

	    c = *s++;
         }

         /* Clip right.  */
         while (c != MASK_COLOR_32) {
	    if (c > 0)
	       s += c;
	    c = *s++;
         }

      next_line:
      }
   }
   else {
      for (y = 0; y < h; y++) {
         PIXEL_PTR ds = OFFSET_PIXEL_PTR(bmp_read_line(dst, dybeg + y), dxbeg);
         PIXEL_PTR dd = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);
         long c = *s++;

         /* Visible part.  */
         for (x = w; x > 0; ) {
	    if (c == MASK_COLOR_32)
	       goto next_line2;
	    else if (c > 0) {
	       /* Run of solid pixels.  */
	       if ((x - c) >= 0) {
	          /* Fully visible.  */
	          x -= c;
	          for (c--; c >= 0; s++, INC_PIXEL_PTR(ds), INC_PIXEL_PTR(dd), c--) {
		     unsigned long col = RGBA_BLEND(blender, GET_PIXEL(ds), *s);
		     PUT_PIXEL(dd, col);
	          }
	       }
	       else {
	          /* Clipped on the right.  */
	          c -= x;
	          for (x--; x >= 0; s++, INC_PIXEL_PTR(ds), INC_PIXEL_PTR(dd), x--) {
		     unsigned long col = RGBA_BLEND(blender, GET_PIXEL(ds), *s);
		     PUT_PIXEL(dd, col);
	          }
	          break;
	       }
	    }
	    else {
	       /* Run of transparent pixels.  */
	       x += c;
	       ds = OFFSET_PIXEL_PTR(ds, -c);
	       dd = OFFSET_PIXEL_PTR(dd, -c);
	    }

	    c = *s++;
         }

      next_line2:
      }
   }

   bmp_unwrite_line(dst);
}

#endif



/* _linear_draw_lit_rle_sprite:
 *  Draws a tinted RLE sprite onto a linear bitmap.
 */
void FUNC_LINEAR_DRAW_LIT_RLE_SPRITE(BITMAP *dst, AL_CONST RLE_SPRITE *src, int dx, int dy, int color)
{
   int x, y, w, h;
   int dxbeg, dybeg;
   int sxbeg, sybeg;
   RLE_PTR s;
   DLS_BLENDER blender;

   if (dst->clip) {
      int tmp;

      tmp = dst->cl - dx;
      sxbeg = ((tmp < 0) ? 0 : tmp);
      dxbeg = sxbeg + dx;

      tmp = dst->cr - dx;
      w = ((tmp > src->w) ? src->w : tmp) - sxbeg;
      if (w <= 0)
	 return;

      tmp = dst->ct - dy;
      sybeg = ((tmp < 0) ? 0 : tmp);
      dybeg = sybeg + dy;

      tmp = dst->cb - dy;
      h = ((tmp > src->h) ? src->h : tmp) - sybeg;
      if (h <= 0)
	 return;
   }
   else {
      w = src->w;
      h = src->h;
      sxbeg = 0;
      sybeg = 0;
      dxbeg = dx;
      dybeg = dy;
   }

   blender = MAKE_DLS_BLENDER(color);
   s = (RLE_PTR) (src->dat);

   /* Clip top.  */
   for (y = sybeg - 1; y >= 0; y--) {
      long c = *s++;

      while (!RLE_IS_EOL(c)) {
	 if (c > 0)
	    s += c;
	 c = *s++;
      }
   }

   bmp_select(dst);

   /* Visible part.  */
   if (sxbeg || dx+src->w >= dst->cr) {
      for (y = 0; y < h; y++) {
         PIXEL_PTR d = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);
         long c = *s++;

         /* Clip left.  */
         for (x = sxbeg; x > 0; ) {
	    if (RLE_IS_EOL(c))
	       goto next_line;
	    else if (c > 0) {
	       /* Run of solid pixels.  */
	       if ((x - c) >= 0) {
	          /* Fully clipped.  */
	          x -= c;
	          s += c;
	       }
	       else {
	          /* Visible on the right.  */
	          c -= x;
	          s += x;
	          break;
	       }
	    }
	    else {
	       /* Run of transparent pixels.  */
	       if ((x + c) >= 0) {
	          /* Fully clipped.  */
	          x += c;
	       }
	       else {
	          /* Visible on the right.  */
	          c += x;
	          break;
	       }
	    }

	    c = *s++;
         }

         /* Visible part.  */
         for (x = w; x > 0; ) {
	    if (RLE_IS_EOL(c))
	       goto next_line;
	    else if (c > 0) {
	       /* Run of solid pixels.  */
	       if ((x - c) >= 0) {
	          /* Fully visible.  */
	          x -= c;
	          for (c--; c >= 0; s++, INC_PIXEL_PTR(d), c--) {
		     unsigned long col = DLS_BLEND(blender, color, *s);
		     PUT_PIXEL(d, col);
	          }
	       }
	       else {
	          /* Clipped on the right.  */
	          c -= x;
	          for (x--; x >= 0; s++, INC_PIXEL_PTR(d), x--) {
		     unsigned long col = DLS_BLEND(blender, color, *s);
		     PUT_PIXEL(d, col);
	          }
	          break;
	       }
	    }
	    else {
	       /* Run of transparent pixels.  */
	       x += c;
	       d = OFFSET_PIXEL_PTR(d, -c);
	    }

	    c = *s++;
         }

         /* Clip right.  */
         while (!RLE_IS_EOL(c)) {
	    if (c > 0)
	       s += c;
	    c = *s++;
         }

      next_line:
      }
   }
   else {
      for (y = 0; y < h; y++) {
         PIXEL_PTR d = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);
         long c = *s++;

         /* Visible part.  */
         for (x = w; x > 0; ) {
	    if (RLE_IS_EOL(c))
	       goto next_line2;
	    else if (c > 0) {
	       /* Run of solid pixels.  */
	       if ((x - c) >= 0) {
	          /* Fully visible.  */
	          x -= c;
	          for (c--; c >= 0; s++, INC_PIXEL_PTR(d), c--) {
		     unsigned long col = DLS_BLEND(blender, color, *s);
		     PUT_PIXEL(d, col);
	          }
	       }
	       else {
	          /* Clipped on the right.  */
	          c -= x;
	          for (x--; x >= 0; s++, INC_PIXEL_PTR(d), x--) {
		     unsigned long col = DLS_BLEND(blender, color, *s);
		     PUT_PIXEL(d, col);
	          }
	          break;
	       }
	    }
	    else {
	       /* Run of transparent pixels.  */
	       x += c;
	       d = OFFSET_PIXEL_PTR(d, -c);
	    }

	    c = *s++;
         }

      next_line2:
      }
   }

   bmp_unwrite_line(dst);
}

#endif /* !__bma_cspr_h */

