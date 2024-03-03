#include "md.h"

LONG letter_h[TILE_LENGTH] = {
	0x11000110,
	0x11000110,
	0x11000110,
	0x11111110,
	0x11000110,
	0x11000110,
	0x11000110,
	0x00000000,
};

void load_tile(WORD tileno, LONG *tile)
{
	LONG *data = (LONG *) VDP_data;
	WORD *ctrl = (WORD *) VDP_ctrl;
	BYTE i;

	*ctrl = SET_VRAM_ADDR(tileno << 5);
	for(i = 0; i < TILE_LENGTH; i++)
		*data = tile[i];
}

void main(void)
{
	load_tile(1, letter_h);
}
