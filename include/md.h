#ifndef MD_H_
#define MD_H_

/* Specific to disable the TMSS, is "SEGA" in ASCII. */
#define SEGA_MAGIC 		0x53454741

/* VDP */
#define VDP_ctrl 		0xc00004
#define VDP_data 		0xc00000
#define	VDP_hvcounter		0xc80008	

/* Write to the VDP control port to so we can write to VDP's memory locations */
#define	VDP_vram_addr		0x40000000
#define	VDP_cram_addr		0xc0000000
#define	VDP_vsram_addr		0x40000010

#define	VDP_vram_size		0x10000
#define	VDP_cram_size		0x80
#define	VDP_vsram_size		0x50

/* VDP registers. To configure the behavior of the VDP */
#define	VDP_reg_mode1		0x8000
#define	VDP_reg_mode2		0x8100
#define	VDP_reg_mode3		0x8b00
#define	VDP_reg_mode4		0x8c00

#define	VDP_reg_plane_a		0x8200
#define	VDP_reg_plane_b		0x8400
#define	VDP_reg_window		0x8300
#define	VDP_reg_sprite		0x8500
#define	VDP_reg_hscroll		0x8d00

#define	VDP_reg_bgcol		0x8700
#define	VDP_reg_hrate		0x8a00
#define	VDP_reg_incr		0x8f00
#define	VDP_reg_size		0x9000
#define	VDP_reg_win_x		0x9100
#define	VDP_reg_win_y		0x9200

#define	VDP_reg_dma_len_l	0x9300
#define	VDP_reg_dma_len_h	0x9400
#define	VDP_reg_dma_src_l	0x9500
#define	VDP_reg_dma_src_m	0x9600
#define	VDP_reg_dma_src_h	0x9700

/* Z80 registers */
#define	Z80_request		0xa11100
#define	Z80_granted		0xa11101
#define	Z80_reset		0xa11200
#define	Z80_ram_base		0xa00000

#define SET_XRAM_ADDR(addr, cmd) \
	((((addr) & 0x3fff) << 16) | (((addr) & 0xc000) >> 14) | (cmd))

#define SET_VRAM_ADDR(addr) 	SET_XRAM_ADDR(addr, VDP_vram_addr)
#define SET_CRAM_ADDR(addr) 	SET_XRAM_ADDR(addr, VDP_cram_addr)
#define SET_VSRAM_ADDR(addr) 	SET_XRAM_ADDR(addr, VDP_vsram_addr)

/* VDP tiles are 8 longs */
#define TILE_LENGTH 	8

#ifdef __ASSEMBLER__
#include "md_macro.h"
#else
typedef	unsigned char 	BYTE;
typedef unsigned int 	WORD;
typedef unsigned long 	LONG;
#endif /* __ASSEMBLER__ */

#endif /* MD_H_ */
