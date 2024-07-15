#ifndef MD_H_
#error "Never include this file directly. Do it through md.h"
#endif

.macro set_xram_addr addr, cmd
	move.l #(((\addr) & 0x3fff)<<16) | (((\addr) & 0xc000)>>14)|(\cmd), (VDP_CTRL)
.endm

.macro set_vram_addr addr
	set_xram_addr \addr, VDP_vram_addr
.endm

.macro set_cram_addr addr
	set_xram_addr \addr, VDP_cram_addr
.endm

.macro set_vsram_addr addr
	set_xram_addr \addr, VDP_vsram_addr
.endm

.macro set_xram_addr_reg reg, cmd
	and.l #0xFFFF, \reg
	lsl.l #2, \reg
	lsr.w #2, \reg
	swap \reg
	or.l #\cmd, \reg
	move.l \reg, (VDP_CTRL)
.endm

.macro set_vram_addr_reg reg
	set_xram_addr_reg \reg, VDP_VRAM_ADDR
.endm

.macro set_cram_addr_reg reg
	set_xram_addr_reg \reg, VDP_CRAM_ADDR
.endm

.macro set_vsram_addr_reg reg
	set_xram_addr_reg \reg, VDP_VSRAM_ADDR
.endm
