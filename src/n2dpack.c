
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef __linux__
#include <sys/stat.h>
#else
#include <io.h>
#endif
#include "ucl/ucl.h"

#define DEFCOMPLV	10

#ifdef _WIN32
#ifndef _CRTAPI1
#define _CRTAPI1 __cdecl
#endif
#else
#define _CRTAPI1
#endif

UCL_PUBLIC(int)
ucl_nrv2d_test_overlap_8_o (const ucl_bytep src, ucl_uint src_off,
	ucl_uint  src_len, ucl_uintp dst_len, ucl_voidp wrkmem, unsigned int *poverlap);

const char *in_fname  = NULL;
const char *out_fname = NULL;

long file_length (FILE *f);
static void print_usage (void);
static void usage (void);
static unsigned int get_nrv2d_overhead (unsigned int size);

int _CRTAPI1 main (int argc, const char *argv[])
{
	int cr, in_len, out_len, olen, i;
	unsigned int ovrlp;
	char complv = DEFCOMPLV;
	FILE *f;
	char *pi = NULL, *po = NULL;
	char f_e = 0, f_to = 0;
	const char *pa, *fname;
	long flen;

	for (i = 1; i < argc; i++) {
		pa = argv [i];
		if (pa [0] == '-'
#ifdef _WIN32
		|| pa [0] == '/'
#endif
		) {	pa ++;
			if ((pa [0] == '?' || tolower (pa [0]) == 'h')
#ifndef __linux__
				&& pa [1] == '\0' || stricmp (pa, "help") == 0 ||
				pa [0] == '-' && stricmp (pa, "-help") == 0) goto usg;
#else
				&& pa [1] == '\0' || strcmp (pa, "help") == 0 ||
				pa [0] == '-' && strcmp (pa, "-help") == 0) goto usg;
#endif
			if (tolower (pa [0]) == 'o' && pa [1] == '\0') { f_to = 1; continue; }
			else if (isdigit (pa [0])) {
				cr = atoi (pa);
				if (cr > 10 || cr <= 0)
					printf ("Bad compression level: %s\n", pa-1), f_e = 1;
				else	complv = cr;
				continue;
			}
			printf ("Unknown option: %s\n", pa-1); f_e = 1;
			continue;
		}
		if (in_fname == NULL) in_fname = pa;
		else if (out_fname == NULL) out_fname = pa;
		else { f_e = 1; break; }
	}
	if (f_e) { puts ("Command line error"); usage (); return 0; }
	if (in_fname == NULL) {
usg:		print_usage (); return 0;
	}

	f = fopen (in_fname, "rb");
	if (f == NULL) {
		printf ("File '%s' open error\n", in_fname);
		goto ioerr;
	}
	fname = in_fname;
	flen = file_length (f);
	if (flen == -1) {
iomsg:		printf ("%s: file I/O error: ", fname);
ioerr:		perror (NULL); cr = 1;
		goto err1;
	}
	if (flen >= 65536 || flen < 0) { puts ("File is too large"); cr = 1; goto err; }
	if (flen == 0) { puts ("File is empty"); cr = 0; goto err; }
	if (ucl_init () != UCL_E_OK) {
		puts ("UCL initialization error");
		cr = 1; goto err;
	}
	pi = malloc (in_len = flen);
	if (pi == NULL) {
merr:		puts ("Memory allocation error");
		cr = 1; goto err1;
	}
	po = malloc (in_len + get_nrv2d_overhead (in_len));
	if (po == NULL) goto merr;
	if (fread (pi, in_len, 1, f) != 1) goto iomsg;
	fclose (f); f = NULL; fname = NULL;
	cr = ucl_nrv2d_99_compress (pi, in_len, po, &out_len, 0, complv, NULL, NULL);
	if (cr < UCL_E_OK) {
		if (cr == UCL_E_NOT_COMPRESSIBLE) {
			puts ("Can't compress (UCL)");
			cr = 1; goto err1;
		}
		printf ("Compression error (%d)\n", cr);
		cr = 1; goto err1;
	}
	free (pi); pi = NULL; cr = 0;
	if (f_to) {
		olen = in_len;
		cr = ucl_nrv2d_test_overlap_8_o (po-in_len, in_len, out_len, &olen, NULL, &ovrlp);
		if (cr < UCL_E_OK) printf ("Overlap test error (%d)\n", cr), f_to = 0;
	}
	printf ("File:\t%s\nInitial size:\t%5d B", in_fname, in_len);
	if (f_to) printf (" (overlap slop: %u B)\n", out_len - ovrlp);
#ifndef __linux__
	else	fputchar ('\n');
#else
	else	puts("");
#endif
	printf ("Packed:\t\t%5d B (%.1f %%)\n",
		out_len, (float)(out_len * 100) / in_len);
	if (out_len >= in_len) puts ("Data is not compressible");
	if (out_fname != NULL) {
		f = fopen (out_fname, "wb");
		fname = out_fname;
		if (f == NULL) goto iomsg;
		if (fwrite (po, out_len, 1, f) != 1) goto iomsg;
		printf ("Saved to:\t%s\n", out_fname);
	}
err1:	if (po) free (po);
	if (pi) free (pi);
err:	if (f) fclose (f);
	return cr;
}

static unsigned int get_nrv2d_overhead (unsigned int size)
{
	return size / 8 + 256;
}

static void print_usage (void)
{
	puts ("\nUCL NRV2d algorithm compressor (UCL v1.03) slightly modificated for\n"
	"  8-bit special decompressor implementation (with short final marker)\n");
	usage ();
}

static void usage (void)
{
	puts ("Usage:\n  n2dpack <input file> [<output file>] [-o] [-<level>]\n"
	"\t-o\t - test overlap\n"
	"\t-<level> - 1..10 (max default)"
#ifdef __linux__
	"\n"
#endif
);
}

long file_length (FILE *f)
{
#ifdef __linux__
	struct stat st;
	if (fstat (fileno (f), &st) != 0)
		return -1;
	return st.st_size;
#else
	return filelength (fileno (f));
#endif
}
