#include "md/md.h"

#define ARRLEN(x) (sizeof(x)/sizeof(*x))

#define NOINLINE __attribute__((noinline))

/* Declared in game.S */
extern vdp_tile_t letter_h;
extern vdp_tile_t letter_e;
extern vdp_tile_t letter_l;
extern vdp_tile_t letter_o;
extern vdp_tile_t letter_w;
extern vdp_tile_t letter_r;
extern vdp_tile_t letter_d;
extern vdp_tile_t letter_exclamation_point;

void VDP_load_tile(WORD tileno, LONG *tile)
{
	BYTE i = 0;

	VDP_write_ctrl(SET_VRAM_ADDR(tileno << 5));
	while(i < VDP_TILE_SIZE) {
		VDP_write_l_data(tile[i++]);
	}
}

WORD letter_index[] = {
	0x1, 0x2, 0x3, 0x3, 0x4, 	/* hello */
	0x0, 				
	0x5, 0x4, 0x6, 0x3, 0x7,	/* world */
	0x8				/* ! */
};

int main(void)
{
	BYTE i;

	/* load these tiles into vram */
	VDP_load_tile(1, letter_h);
	VDP_load_tile(2, letter_e);
	VDP_load_tile(3, letter_l);
	VDP_load_tile(4, letter_o);
	VDP_load_tile(5, letter_w);
	VDP_load_tile(6, letter_r);
	VDP_load_tile(7, letter_d);
	VDP_load_tile(8, letter_exclamation_point);
	
	/* now we're writing into plane A */
	VDP_write_ctrl(SET_VRAM_ADDR(0xc000));

	for(i = 0; i < ARRLEN(letter_index); i++) {
		VDP_write_w_data(letter_index[i]);
	}

	return 0;
}
