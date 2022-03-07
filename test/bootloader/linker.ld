SECTIONS
{
	. = 0x7c00;
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.*)
		SHORT(0x6942)
	}
}
