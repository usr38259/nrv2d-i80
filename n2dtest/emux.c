
#include "emux.h"

extern unsigned char mem [65536];

UINT8 program_read_byte(UINT32 addr)
{
	return mem [addr & 0xffff];
}

io_read_byte(port)
{
	return 0;
}

io_write_byte(port, value)
{
	return 0;
}

program_write_byte(UINT32 addr, UINT8 value)
{
	mem [addr & 0xffff] = value;
}

direct_read_decrypted_byte(pc)
{
	return mem [pc & 0xffff];
}

direct_read_raw_byte(pc)
{
	return mem [pc & 0xffff];
}
