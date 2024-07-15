#include "md/md.h"

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
.ascii 	"(C)DLJS 2024.JUL"
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

_start:
	jsr disable_tmss
	move.w #0x2300, %sr

	jsr _main

Lforever:
	trap #0
	bra.s Lforever
	nop
	nop
	nop

disable_tmss:
	move.b (0xa10001), %d0
	and.b %d0, 0x0f
	beq Lno_tmss
	move.l #SEGA_MAGIC, (0xa14000)
Lno_tmss:
	tst.w (VDP_CTRL)
	rts
	nop
	nop
	nop

_hblank:
	jsr _hblank_callback
	rte
_vblank:
	jsr _vblank_callback
	rte
