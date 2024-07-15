#include "md/md.h"
#include "md/macro.h"

.equ 	VDP_plane_a_base,	0xc000
.equ 	VDP_plane_b_base,	0xe000

.section .header

/* 68000 vector table */
.long	0x00ffe0000
.long 	_start
.rept 	26
.long	_interrupt
.endr
.long	_vblank
.long	_interrupt
.long	_vblank
.rept 	33
.long	_interrupt
.endr

/* MegaDrive ROM header */
.ascii 	"SEGA MEGA DRIVE" 
.align	16, ' '

/* copyright */
.ascii 	"(C)DLJS 2023.JAN"
.align	16, ' '

/* domestic game name */
.ascii 	"TEST GAME"
.align	32, ' '
.ascii " "
.align	16, ' '

/* overseas game name */
.ascii 	"TEST GAME"
.align	64, ' '

/* serial number */
.ascii "GM 00001051-00"

/* checksum */
.word __checksum

/* controller support */
.ascii "J"
.align 16

/* ram range */
.long 0x0
.long __length

/* rom range */
.long 0x0
.long 0xff00000

/* extra ram & modem */
.ascii " "
.align 16, ' '
.rept 40
.byte ' '
.endr
.align 16, ' '

/* region */
.ascii "JUE"
.align 16, ' '

_interrupt:
	rte

.section .text

_start:
	jsr disable_tmss

	move.w #0x2300, %sr

	/*jsr z80_load_program*/

	/* VDP init */
	jsr load_vdp_registers

	jsr clear_vram

	move.w #0x0, %d0
	lea palette, %a0
	jsr load_palette

	jsr main


Lforever:
	trap #0
	bra.s Lforever
	/* padding */
	nop
	nop
	nop
	nop
	nop

/*
 * d0 = tile number
 * a0 = ptr to first tile
 */
load_tile:
	lsl.w #0x5, %d0
	set_vram_addr_reg %d0

	lea (VDP_DATA), %a1
	move.w #0x7, %d1
L1:
	move.l (%a0)+, (%a1)
	dbf %d1, L1
	rts
/*
 * d0 = tile number
 * d1 = number of tiles
 * a0 = ptr to first tile
 */
load_tiles:
	lsl.w #0x5, %d0
	set_vram_addr_reg %d0

	lea (VDP_DATA), %a1
	subq.w #1, %d1
L4:
	move.w #0x7, %d2
L5:
	move.l (%a0)+, (%a1)
	dbf %d2, L5
	dbf %d1, L4
	rts

/*
 * d0 = palette number
 * a0 = ptr to palette
 */
load_palette:
	lsl.w #5, %d0
	set_cram_addr_reg %d0

	move.w #8-1, %d1
	lea VDP_DATA, %a1
L2:
	move.l (%a0)+, (%a1)
	dbra %d1, L2
	rts

disable_tmss:
	move.b (0xa10001), %d0
	and.b %d0, 0x0f
	beq .L2
	move.l #SEGA_MAGIC, (0xa14000)
.L2:
	tst.w (VDP_CTRL)
	rts

clear_vram:
	clr %d0
	lea	(VDP_CTRL), %a0
	lea	(VDP_DATA), %a1

	move.l #VDP_VRAM_ADDR, (%a0)
	move.w #(VDP_VRAM_SIZE/4)-1, %d1
Lclear_vram:
	move.l %d0, (%a1)
	dbf %d1, Lclear_vram

	move.l #VDP_CRAM_ADDR, (%a0)
	move.w #(VDP_CRAM_SIZE/4)-1, %d1
Lclear_cram:
	move.l %d0, (%a1)
	dbf %d1, Lclear_cram

	move.l #VDP_VSRAM_ADDR, (%a0)
	move.w #(VDP_VSRAM_SIZE/4)-1, %d1
Lclear_vsram:
	move.l %d0, (%a1)
	dbf %d1, Lclear_vsram
	rts

load_vdp_registers:
	clr.l %d0
	movea.l %d0, %a0
	move.l #0x3fff, %d1

	lea (VDP_CTRL), %a0

	move.w #(VDP_REG_MODE1 | 0x04), (%a0)
	move.w #(VDP_REG_MODE2 | 0x74), (%a0)
	move.w #(VDP_REG_PLANE_A | 0x30), (%a0)
	move.w #(VDP_REG_WINDOW | 0x34), (%a0)
	move.w #(VDP_REG_PLANE_B | 0x07), (%a0)
	move.w #(VDP_REG_SPRITE | 0x78), (%a0)
	move.w #(VDP_REG_BGCOL | 0x00), (%a0)
	move.w #(VDP_REG_HRATE | 0xff), (%a0)
	move.w #(VDP_REG_MODE3 | 0x00), (%a0)
	move.w #(VDP_REG_MODE4 | 0x81), (%a0)
	move.w #(VDP_REG_HSCROLL | 0x3d), (%a0)
	move.w #(VDP_REG_INCR | 0X02), (%a0)
	move.w #(VDP_REG_SIZE | 0X01), (%a0)
	move.w #(VDP_REG_WIN_X | 0x00), (%a0)
	move.w #(VDP_REG_WIN_Y | 0x00), (%a0)

	rts

z80_load_program:
	/* reset the Z80 */
	move.w #0x0, (Z80_RESET)
	move.w #0x100, (Z80_REQUEST)
	move.w #0x100, (Z80_RESET)
L3:	
	btst.b #0, Z80_GRANTED
	bne L3 
	move.l #Z80_RAM_BASE, %a1
	
	/* copy program into Z80 ram */
	move.b #0x00, (%a1)+
	move.b #0xC3, (%a1)+
	move.b #0x00, (%a1)+
	move.b #0x00, (%a1)+

	move.w #0x100, (Z80_RESET)
	MOVE.W #0X0, (Z80_REQUEST)
	rts

_hblank:
	rte
_vblank:
#if 0
	set_vram_addr 0xc000
	move.w (VDP_DATA), %d0
	cmpi.w #0xa, %d0
	bge L_vblank_out
	addi.w #1, %d0
	move.w %d0, (VDP_DATA)
L_vblank_out: 
#endif
	rte

.section .data

letter_index:
	.word 0x1
	.word 0x2
	.word 0x3
	.word 0x3
	.word 0x4
	.word 0x0
	.word 0x5
	.word 0x4
	.word 0x6
	.word 0x3
	.word 0x7
	.word 0x8

.global letter_h
letter_h:
	.long  	0x11000110
	.long  	0x11000110
	.long  	0x11000110
	.long  	0x11111110
	.long  	0x11000110
	.long  	0x11000110
	.long  	0x11000110
	.long  	0x00000000

.global letter_e
letter_e:
	.long  	0x11111110
	.long  	0x11000000
	.long  	0x11000000
	.long  	0x11111000
	.long  	0x11000000
	.long  	0x11000000
	.long  	0x11111110
	.long  	0x00000000

.global letter_l
letter_l:
	.long  	0x11000000
	.long  	0x11000000
	.long  	0x11000000
	.long  	0x11000000
	.long  	0x11000000
	.long  	0x11000000
	.long  	0x11111110
	.long  	0x00000000

.global letter_o
letter_o:
	.long  	0x01111100
	.long  	0x11000110
	.long  	0x11000110
	.long  	0x11000110
	.long  	0x11000110
	.long  	0x11000110
	.long  	0x01111100
	.long  	0x00000000

.global letter_w
letter_w:
	.long  	0x11000110
	.long  	0x11000110
	.long  	0x11010110
	.long  	0x11010110
	.long  	0x11010110
	.long  	0x11010110
	.long  	0x01111100
	.long  	0x00000000

.global letter_r
letter_r:
	.long 	0x11111100
	.long 	0x11000110
	.long 	0x11000110
	.long 	0x11111100
	.long 	0x11110000
	.long 	0x11011000
	.long 	0x11000110
	.long 	0x00000000

.global letter_d
letter_d:
	.long 	0x11111000
	.long 	0x11001100
	.long 	0x11000110
	.long 	0x11000110
	.long 	0x11000110
	.long 	0x11001100
	.long 	0x11111000
	.long 	0x00000000

.global letter_exclamation_point
letter_exclamation_point:
	.long 	0x00111000
	.long 	0x00111000
	.long 	0x00111000
	.long 	0x00010000
	.long 	0x00000000
	.long 	0x00010000
	.long 	0x00111000
	.long 	0x00010000
	.long 	0x00000000

palette:
	.word 0x000
	.word 0xeee 
	.word 0x000
	.word 0x000
	.word 0x000
	.word 0x000
	.word 0x000
	.word 0x000
	.word 0x000
	.word 0x000
	.word 0x000
	.word 0x000
	.word 0x000
	.word 0x000
	.word 0x000
	.word 0x000
