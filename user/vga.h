#ifndef __VGA_H
#define __VGA_H

#include "stm32f10x.h"
#include "str_dealing.h"

#define	NULL			0x0

#define	VID_HSIZE		50		// Horizontal resolution (in bytes)
#define	VID_VSIZE		200		// Vertical resolution (in lines)

#define	VID_PIXELS_X	(VID_HSIZE * 8)
#define	VID_PIXELS_Y	VID_VSIZE
#define	VID_PIXELS_XR	(VID_PIXELS_X + 16)
#define	VID_HSIZE_R		(VID_HSIZE + 2)

// System font
#define	GDI_SYSFONT_WIDTH		6		// Width in pixels
#define	GDI_SYSFONT_HEIGHT		9		// Height in pixels
#define	GDI_SYSFONT_BYTEWIDTH	1		// Width in bytes
#define	GDI_SYSFONT_OFFSET		0x20

//	RASTER OPERATION
#define	GDI_ROP_COPY			0
#define	GDI_ROP_XOR				1
#define	GDI_ROP_AND				2
#define	GDI_ROP_OR				3

// RASTER OPERATION
#define	GDI_WINCAPTION			0x0001
#define	GDI_WINBORDER			0x0002
#define	GDI_WINCLOSEICON		0x0003

// Text align mode
#define	GDI_WINCAPTION_LEFT		0x0000
#define	GDI_WINCAPTION_CENTER	0x0010
#define	GDI_WINCAPTION_RIGHT	0x0020
#define	GDI_WINCAPTION_MASK		0x0030

typedef __packed struct
{
	int16_t	w;							// Width in bits
	int16_t	h;							// Height in bits
	int16_t	wb;							// width in bytes
	int16_t	wh;							// Height in bytes
	pu8	*bm;						    // Pointer to bitmap bits

} GDI_BITMAP, PGDI_BITMAP;

typedef __packed  struct
{
	int16_t	x;							// X position
	int16_t	y;							// Y position
	int16_t	w;							// Width
	int16_t	h;							// Height
    
} GDI_RECT, *PGDI_RECT;

typedef __packed struct
{
	uint16_t style;				        // Mode, see GDI_WINxxx defines
	GDI_RECT rc;					    // Absolute rectangle
	pu8	caption;			            // Caption text

} GDI_WINDOW, *PGDI_WINDOW;

void timer_config(void);
void vga_clear_screen(void);
void vga_init(void);
void vga_bitblt(PGDI_RECT prc, int16_t x, int16_t y, int16_t w, int16_t h, pu8 bm, uint16_t rop);
void vga_draw_text(int16_t x, int16_t y, pu8 ptext, u16 rop);
void vga_draw_nwords(int16_t x, int16_t y, pu8 ptext, u16 rop, u16 l);
void vga_draw_textrec(PGDI_RECT prc, pu8 ptext, uint16_t style, uint16_t rop);
void vga_draw_point(PGDI_RECT rc, u16 x, u16 y, u16 rop);
void vga_draw_line(PGDI_RECT prc, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t rop);
void vga_draw_rec(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t rop);

#endif /* __VGA_H */



