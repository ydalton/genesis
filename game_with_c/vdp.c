#include "md.h"

#define ARRLEN(x) (sizeof(x)/sizeof(*x))

#define NOINLINE __attribute__((noinline))

/* Declared in game.S */
extern LONG letter_h[TILE_LENGTH];
extern LONG letter_e[TILE_LENGTH];
extern LONG letter_l[TILE_LENGTH];
extern LONG letter_o[TILE_LENGTH];
extern LONG letter_w[TILE_LENGTH];
extern LONG letter_r[TILE_LENGTH];
extern LONG letter_d[TILE_LENGTH];
extern LONG letter_exclamation_point[TILE_LENGTH];

#define str(s) #s
#define xstr(s) str(s)

static inline void VDP_write_ctrl(LONG val)
{
	asm volatile ("move.l %0, " xstr(VDP_ctrl)
			:
			: "d" (val));
}

static inline void VDP_write_l_data(LONG val)
{
	//*((LONG *) data) = val;
	asm volatile ("move.l %0, " xstr(VDP_data)
			:
			: "d" (val));
}

static inline void VDP_write_w_data(WORD val)
{
	asm volatile ("move.w %0, " xstr(VDP_data)
			:
			: "r" (val));
}

static inline void VDP_write_b_data(BYTE val)
{
	asm volatile ("move.b %0, " xstr(VDP_data)
			:
			: "r" (val));
}

#undef xstr
#undef str

void VDP_load_tile(WORD tileno, LONG *tile)
{
	BYTE i = 0;

	VDP_write_ctrl(SET_VRAM_ADDR(tileno << 5));
	while(i < TILE_LENGTH) {
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
