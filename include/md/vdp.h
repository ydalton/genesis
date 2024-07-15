#ifndef MD_H_
#error "Never include this file directly. Do it through md.h"
#endif

/* VDP */
#define VDP_CTRL 		0xc00004
#define VDP_DATA 		0xc00000
#define	VDP_HVCOUNTER		0xc80008	

/* Write TO THE VDP CONTROL PORT to so we can write to VDP's memory locations */
#define	VDP_VRAM_ADDR		0x40000000
#define	VDP_CRAM_ADDR		0xc0000000
#define	VDP_VSRAM_ADDR		0x40000010

#define	VDP_VRAM_SIZE		0x10000
#define	VDP_CRAM_SIZE		0x80
#define	VDP_VSRAM_SIZE		0x50

/* VDP registers. To configure the behavior of the VDP */
#define	VDP_REG_MODE1		0x8000
#define	VDP_REG_MODE2		0x8100
#define	VDP_REG_MODE3		0x8b00
#define	VDP_REG_MODE4		0x8c00

#define	VDP_REG_PLANE_A		0x8200
#define	VDP_REG_PLANE_B		0x8400
#define	VDP_REG_WINDOW		0x8300
#define	VDP_REG_SPRITE		0x8500
#define	VDP_REG_HSCROLL		0x8d00

#define	VDP_REG_BGCOL		0x8700
#define	VDP_REG_HRATE		0x8a00
#define	VDP_REG_INCR		0x8f00
#define	VDP_REG_SIZE		0x9000
#define	VDP_REG_WIN_X		0x9100
#define	VDP_REG_WIN_Y		0x9200

#define	VDP_REG_DMA_LEN_L	0x9300
#define	VDP_REG_DMA_LEN_H	0x9400
#define	VDP_REG_DMA_SRC_L	0x9500
#define	VDP_REG_DMA_SRC_M	0x9600
#define	VDP_REG_DMA_SRC_H	0x9700

/* VDP tiles are 8 longs */
#define VDP_TILE_SIZE		8
/* VDP palettes are 16 words */
#define VDP_PALETTE_SIZE	16

#ifndef __ASSEMBLER__
#define str(s)		#s
#define xstr(s)		str(s)

#define SET_XRAM_ADDR(addr, cmd) \
	((((addr) & 0x3fff) << 16) | (((addr) & 0xc000) >> 14) | (cmd))

#define SET_VRAM_ADDR(addr) 	SET_XRAM_ADDR(addr, VDP_VRAM_ADDR)
#define SET_CRAM_ADDR(addr) 	SET_XRAM_ADDR(addr, VDP_CRAM_ADDR)
#define SET_VSRAM_ADDR(addr) 	SET_XRAM_ADDR(addr, VDP_VSRAM_ADDR)

#define SPRITE_SIZE(w, h) (((w-1) & 0x3) << 2 || ((h-1) & 0x3))

static inline void VDP_write_ctrl(LONG val)
{
	asm volatile ("move.l %0, (" xstr(VDP_CTRL) ")"
			:
			: "d" (val));
}

static inline void VDP_set_reg(WORD val)
{
	asm volatile ("move.w %0, (" xstr(VDP_CTRL) ")"
			:
			: "d" (val));
}

static inline void VDP_write_l_data(LONG val)
{
	//*((LONG *) data) = val;
	asm volatile ("move.l %0, " xstr(VDP_DATA)
			:
			: "d" (val));
}

static inline void VDP_write_w_data(WORD val)
{
	asm volatile ("move.w %0, " xstr(VDP_DATA)
			:
			: "r" (val));
}

static inline void VDP_write_b_data(BYTE val)
{
	asm volatile ("move.b %0, " xstr(VDP_DATA)
			:
			: "r" (val));
}

#undef xstr
#undef str

typedef LONG vdp_tile_t[VDP_TILE_SIZE];
typedef WORD vdp_palette_t[VDP_PALETTE_SIZE];

#endif /* __ASSEMBER__ */
