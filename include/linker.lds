OUTPUT_FORMAT(binary)
SECTIONS
{
	.text 0x00000000: {
		*(.text)
		*(.data)
		*(.rodata)
	}
	__checksum = 0x0;
	__length = .;
}

MEMORY
{
	rom (rx) : ORIGIN = 0x000000, LENGTH = 0x400000
	ram (rwa) : ORIGIN = 0xff0000, LENGTH = 0x100000
}
