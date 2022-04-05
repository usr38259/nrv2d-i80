
#include <stdio.h>
#include <memory.h>
#include <io.h>
#include "i8x.h"

#define TEXT_BIN	"packed.dat"
#define UNPKDATA_FILE "unpacked.dat"
#define UNPKDATA_OFFSET 0x1EC1

#define PFREQ 2000000

#ifndef _CRTAPI1
#define _CRTAPI1 __cdecl
#endif

struct i8085_state i80;
unsigned char mem [65536];
char unpkdata [65536];

int _CRTAPI1 main ()
{
	FILE *f;
	long fl, ts = 0;

	memset (mem, 0, sizeof (mem));
	f = fopen (TEXT_BIN, "rb");
	if (f == NULL) {
		puts ("BIN file '" TEXT_BIN "' open error");
		return 1;
	}
	fl = filelength (fileno (f));
	if (fl <= 0 || fl >= 65536) {
		puts ("File length error");
		fclose (f);
		return 1;
	}
	if (fread (mem, fl, 1, f) != 1) {
		puts ("BIN file read error");
		fclose (f);
		return 1;
	}
	fclose (f);

	i8080_cpu_init (&i80);
	i808x_cpu_reset (&i80);
	i80.SP.w.l = 0x7ffc;
	*(unsigned short*)&mem [0x7ffe] = 0x7676;
	*(unsigned short*)&mem [0x7ffc] = 0x7ffe;
	i80.PC.w.l = 0x0000;

	while (1) {
		if (mem [i80.PC.w.l] == 0x76) {
			printf ("Halt at %04X\n", i80.PC.w.l);
			break;
		}
		if (ts < 0) {
			puts ("Time exceed");
			break;
		}
		i80.icount = 0;
		i808x_cpu_execute (&i80);
		ts -= i80.icount;
	}
	f = fopen (UNPKDATA_FILE, "rb");
	if (f != NULL) {
		fl = filelength (fileno (f));
		if (fl <= 0 || fl >= 65536) {
			puts ("File '" UNPKDATA_FILE "' length error");
			fclose (f);
			goto ts;
		}
		if (fread (unpkdata, fl, 1, f) != 1) {
			puts ("File '" UNPKDATA_FILE "' read  error:");
			perror (NULL);
			fclose (f);
			goto ts;
		}
		if (memcmp (mem + UNPKDATA_OFFSET, unpkdata, fl) == 0) {
			printf ("Unpacked data file '" UNPKDATA_FILE
				"' and memory at offset %04X are identical\n", UNPKDATA_OFFSET);
		} else	puts ("Unpacked data file '" UNPKDATA_FILE "' and memory ARE NOT IDENTICAL");
		fclose (f);
	} else {
		puts ("Unpacked data file '" UNPKDATA_FILE "' open error:");
		perror (NULL);
	}
ts:	if (ts > 0)
		printf ("%9d tcks; %.6f s\n", ts, (float)ts / PFREQ);
	return 0;
}
