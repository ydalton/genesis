#include "md/md.h"

#define ARRLEN(x) (sizeof(x)/sizeof(*x))

static vdp_tile_t letter_h = {
	0x11000110,
	0x11000110,
	0x11000110,
	0x11111110,
	0x11000110,
	0x11000110,
	0x11000110,
	0x00000000,
};

static vdp_tile_t letter_e = {
	0x11111110,
	0x11000000,
	0x11000000,
	0x11111000,
	0x11000000,
	0x11000000,
	0x11111110,
	0x00000000
};

static vdp_tile_t letter_l = {
	0x11000000,
	0x11000000,
	0x11000000,
	0x11000000,
	0x11000000,
	0x11000000,
	0x11111110,
	0x00000000
};

static vdp_tile_t letter_o = {
	0x01111100,
	0x11000110,
	0x11000110,
	0x11000110,
	0x11000110,
	0x11000110,
	0x01111100,
	0x00000000
};

static vdp_tile_t letter_w = {
	0x11000110,
	0x11000110,
	0x11010110,
	0x11010110,
	0x11010110,
	0x11010110,
	0x01111100,
	0x00000000
};

static vdp_tile_t letter_r = {
	0x11111100,
	0x11000110,
	0x11000110,
	0x11111100,
	0x11110000,
	0x22022000,
	0x22000220,
	0x00000000
};

static vdp_tile_t letter_d = {
	0x11111000,
	0x11001100,
	0x11000110,
	0x11000110,
	0x11000110,
	0x11001100,
	0x11111000,
	0x00000000
};

static vdp_tile_t letter_exclamation_point = {
	0x00111000,
	0x00111000,
	0x00111000,
	0x00010000,
	0x00000000,
	0x00010000,
	0x00111000,
	0x00010000,
};

static vdp_palette_t palette = {
	0x000,
	0xeee,
	0xe00,
	0x000,
	0x000,
	0x000,
	0x000,
	0x000,
	0x000,
	0x000,
	0x000,
	0x000,
	0x000,
	0x000,
	0x000,
	0x000,
};

static WORD vdp_registers[] = {
	VDP_REG_MODE1 | 0X04,
	VDP_REG_MODE2 | 0X74,
	VDP_REG_PLANE_A | 0X30,
	VDP_REG_WINDOW | 0X34,
	VDP_REG_PLANE_B | 0X07,
	VDP_REG_SPRITE | 0X78,
	VDP_REG_BGCOL | 0X00,
	VDP_REG_HRATE | 0XFF,
	VDP_REG_MODE3 | 0X00,
	VDP_REG_MODE4 | 0X81,
	VDP_REG_HSCROLL | 0X3D,
	VDP_REG_INCR | 0X02,
	VDP_REG_SIZE | 0X01,
	VDP_REG_WIN_X | 0X00,
	VDP_REG_WIN_Y | 0X00,
};

void load_vdp_registers()
{
	for(BYTE i = 0; i < ARRLEN(vdp_registers); i++) {
		VDP_set_reg(vdp_registers[i]);
	}
}

static inline void clear_vram(void)
{
	/* clear the VRAM */
	VDP_write_ctrl(VDP_VRAM_ADDR);
	for(WORD i = 0; i < VDP_VRAM_SIZE/4; i++) {
		VDP_write_l_data(0);
	}

	/* clear the CRAM */
	VDP_write_ctrl(VDP_CRAM_ADDR);
	for(WORD i = 0; i < VDP_CRAM_SIZE/4; i++) {
		VDP_write_l_data(0);
	}

	/* clear the VSRAM */
	VDP_write_ctrl(VDP_VSRAM_ADDR);
	for(WORD i = 0; i < VDP_VSRAM_SIZE/4; i++) {
		VDP_write_l_data(0);
	}
}

void VDP_load_tile(WORD tileno, LONG *tile)
{
	BYTE i = 0;

	VDP_write_ctrl(SET_VRAM_ADDR(tileno << 5));
	while(i < VDP_TILE_SIZE) {
		VDP_write_l_data(tile[i++]);
	}
}

void VDP_load_palette(WORD palette_no, WORD *palette)
{
	BYTE i = 0;

	VDP_write_ctrl(SET_CRAM_ADDR(palette_no << 5));
	while(i < VDP_TILE_SIZE) {
		VDP_write_w_data(palette[i++]);
	}
}

static WORD letter_index[] = {
	0x1, 0x2, 0x3, 0x3, 0x4, 	/* hello */
	0x0, 				
	0x5, 0x4, 0x6, 0x3, 0x7,	/* world */
	0x8				/* ! */
};

vdp_tile_t gen;

void _hblank_callback()
{
}

void _vblank_callback()
{
	static LONG i = 0;
	if(i % 2 == 0) {
		gen[0] = 0x11000000;
	} else {
		gen[0] = 0x00011000;
	}
	i++;
}

void _main(void)
{
	BYTE i;

	load_vdp_registers();
	clear_vram();

	VDP_load_palette(0, palette);

	VDP_load_tile(0, gen);

	VDP_write_ctrl(SET_VRAM_ADDR(0xc000));
	for(i = 0; i < 4; i++) {
		VDP_write_w_data(1);
	}
}
