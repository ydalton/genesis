OUTPUT_FORMAT(binary)
SECTIONS
{
	. = 0x00000000;
	.text : {
		game.o(.header)
		*(.text)
	}
	.data : {
		*(.data)
	}
	__checksum = 0x0;
	__length = .;
}
