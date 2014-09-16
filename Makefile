# $Id$

CC = gcc

#CFLAGS = -g -O2 -fPIC -DDEBUGZZ
CFLAGS = -g -O2 -fPIC
#CFLAGS = -g -O0 -fPIC

LIBS = 

library: libmetar.a

all: dmetar library

libmetar.a: src/antoi.o src/charcmp.o src/dcdmetar.o src/dcdmtrmk.o src/fracpart.o src/prtdmetr.o src/stspack2.o src/stspack3.o
	ar ruv libmetar.a src/antoi.o src/charcmp.o src/dcdmetar.o src/dcdmtrmk.o src/fracpart.o src/prtdmetr.o src/stspack2.o src/stspack3.o
	ranlib libmetar.a

dmetar: src/drvmetar.o libmetar.a
	$(CC) $(CFLAGS) -o dmetar src/drvmetar.o libmetar.a $(LIBS)

src/antoi.o: src/antoi.c src/local.h
	$(CC) $(CFLAGS) -c src/antoi.c -o src/antoi.o

src/charcmp.o: src/charcmp.c src/local.h
	$(CC) $(CFLAGS) -c src/charcmp.c -o src/charcmp.o

src/dcdmetar.o: src/dcdmetar.c src/metar_structs.h
	$(CC) $(CFLAGS) -c src/dcdmetar.c -o src/dcdmetar.o

src/dcdmtrmk.o: src/dcdmtrmk.c src/metar_structs.h
	$(CC) $(CFLAGS) -c src/dcdmtrmk.c -o src/dcdmtrmk.o

src/drvmetar.o: src/drvmetar.c src/metar_structs.h
	$(CC) $(CFLAGS) -c src/drvmetar.c -o src/drvmetar.o

src/fracpart.o: src/fracpart.c src/local.h
	$(CC) $(CFLAGS) -c src/fracpart.c -o src/fracpart.o

src/prtdmetr.o: src/prtdmetr.c src/metar_structs.h
	$(CC) $(CFLAGS) -c src/prtdmetr.c -o src/prtdmetr.o

src/stspack2.o: src/stspack2.c src/local.h
	$(CC) $(CFLAGS) -c src/stspack2.c -o src/stspack2.o

src/stspack3.o: src/stspack3.c src/local.h
	$(CC) $(CFLAGS) -c src/stspack3.c -o src/stspack3.o

clean:
	rm -f src/*.o dmetar libmetar.a

install: library
	cp include/metar.h /usr/local/include/
	chmod 0644 /usr/local/include/metar.h
	cp libmetar.a /usr/local/lib/
	chmod 0644 /usr/local/lib/libmetar.a
	ranlib /usr/local/lib/libmetar.a

