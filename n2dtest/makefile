
CFLAGS = /Oxs /Gz

n2dtest.exe:
	cl $(CFLAGS) n2dtest.c i8085.c emux.c

test: n2dtest.exe
	n2dtest.exe
