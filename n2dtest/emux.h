
#ifndef __EMUX_H__
#define __EMUX_H__
#pragma once

#define UINT8 unsigned char
#define INT8 signed char
#define UINT16 unsigned short
#define INT16 signed short
#define UINT32 unsigned int
#define INT32 int
#define INLINE __inline
#define TRUE 1
#define FALSE 0

typedef union PAIR_u {
	struct { UINT8 l,h,h2,h3; } b;
	struct { UINT16 l,h; } w;
	struct { INT8 l,h,h2,h3; } sb;
	struct { INT16 l,h; } sw;
	UINT32 d;
	INT32 sd;
} PAIR;

UINT8 program_read_byte(UINT32 addr);
io_read_byte(port);
io_write_byte(port, value);
program_write_byte(UINT32 addr, UINT8 value);
direct_read_decrypted_byte(pc);
direct_read_raw_byte(pc);

#endif /* __EMUX_H__ */
