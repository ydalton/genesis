#ifndef MD_H_
#define MD_H_

/* Specific to disable the TMSS, is "SEGA" in ASCII. */
#define SEGA_MAGIC 		0x53454741

#ifndef __ASSEMBLER__
typedef	unsigned char 	BYTE;
typedef unsigned short 	WORD;
typedef unsigned long 	LONG;
#endif /* __ASSEMBLER__ */

#include "vdp.h"

/* Z80 registers */
#define	Z80_REQUEST		0xa11100
#define	Z80_GRANTED		0xa11101
#define	Z80_RESET		0xa11200
#define	Z80_RAM_BASE		0xa00000

#endif /* MD_H_ */
