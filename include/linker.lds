OUTPUT_FORMAT(binary)
SECTIONS
{
	. = 0x00000000;
	.text : {
		*(.text)
	}
	__checksum = 0x0;
	__length = .;
}
