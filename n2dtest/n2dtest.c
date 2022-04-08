
#include <stdio.h>
#include <memory.h>
#include <io.h>
#include "i8x.h"

#define TEXT_BIN	"packed.dat"
#define UNPKDATA_FILE "unpacked.dat"

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
	long fl, pkl, ts = 0;

	memset (mem, 0, sizeof (mem));
	f = fopen (TEXT_BIN, "rb");
	if (f == NULL) {
		perror ("BIN file '" TEXT_BIN "' open error");
		return 1;
	}
	pkl = filelength (fileno (f));
	if (pkl <= 0 || pkl >= 65536) {
		puts ("File length error");
		fclose (f);
		return 1;
	}
	if (fread (mem, pkl, 1, f) != 1) {
		perror ("BIN file '" TEXT_BIN "' read error");
		fclose (f);
		return 1;
	}
	fclose (f);

	i8080_cpu_init (&i80);
	i808x_cpu_reset (&i80);
	i80.SP.w.l = 0xfffc;
	*(unsigned short*)&mem [0xfffe] = 0x7676;
	*(unsigned short*)&mem [0xfffc] = 0xfffe;
	i80.PC.w.l = 0x0000;

	while (1) {
		if (mem [i80.PC.w.l] == 0x76) {
			printf ("Halt at %04X\n", i80.PC.w.l);
			break;
		}
		if (ts < 0) {	puts ("Time exceed"); break; }
		i80.icount = 0;
		i808x_cpu_execute (&i80);
		ts -= i80.icount;
	}
	if (ts > 0) {
		printf ("%9d tcks; %.6f s (%f MHz)\n", ts, (float)ts / PFREQ, (float)PFREQ * 1e-6);
		f = fopen (UNPKDATA_FILE, "rb");
	}	else	return 1;
	if (f != NULL) {
		fl = filelength (fileno (f));
		if (fl <= 0 || fl >= 65536) {
			puts ("File '" UNPKDATA_FILE "' length error");
			fclose (f);
			return 1;
		}
		if (fread (unpkdata, fl, 1, f) != 1) {
			perror ("File '" UNPKDATA_FILE "' read error");
			fclose (f);
			return 1;
		}
		fclose (f);
		if (pkl + fl <= 65536) {
			if (memcmp (mem + pkl, unpkdata, fl) == 0)
				printf ("Unpacked data file '" UNPKDATA_FILE
					"' and memory at offset %04X are identical\n", (int)pkl);
			else	puts ("Unpacked data file '" UNPKDATA_FILE "' and memory ARE NOT IDENTICAL");
		} else {
			puts ("Size of unpacked data is out of memory");
			return 1;
		}
	} else {
		perror ("Unpacked data file '" UNPKDATA_FILE "' open error");
		return 1;
	}
	return 0;
}
