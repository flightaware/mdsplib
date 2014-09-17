# $Id$

CC = gcc

#CFLAGS = -g -O2 -fPIC -DDEBUGZZ
#CFLAGS = -g -O0 -fPIC -DDEBUGZZ
CFLAGS = -g -O2 -fPIC
#CFLAGS = -g -O0 -fPIC

LIBS = 

library: libmetar.a

all: metar_test library

libmetar.a: src/antoi.o src/charcmp.o src/decode_metar.o src/decode_metar_remark.o src/fracpart.o src/print_decoded_metar.o src/stspack2.o src/stspack3.o
	ar ruv libmetar.a src/antoi.o src/charcmp.o src/decode_metar.o src/decode_metar_remark.o src/fracpart.o src/print_decoded_metar.o src/stspack2.o src/stspack3.o
	ranlib libmetar.a

metar_test: src/metar_test.o libmetar.a
	$(CC) $(CFLAGS) -o metar_test src/metar_test.o libmetar.a $(LIBS)

src/antoi.o: src/antoi.c src/local.h
	$(CC) $(CFLAGS) -c src/antoi.c -o src/antoi.o

src/charcmp.o: src/charcmp.c src/local.h
	$(CC) $(CFLAGS) -c src/charcmp.c -o src/charcmp.o

src/decode_metar.o: src/decode_metar.c src/metar_structs.h
	$(CC) $(CFLAGS) -c src/decode_metar.c -o src/decode_metar.o

src/decode_metar_remark.o: src/decode_metar_remark.c src/metar_structs.h
	$(CC) $(CFLAGS) -c src/decode_metar_remark.c -o src/decode_metar_remark.o

src/metar_test.o: src/metar_test.c src/metar_structs.h
	$(CC) $(CFLAGS) -c src/metar_test.c -o src/metar_test.o

src/fracpart.o: src/fracpart.c src/local.h
	$(CC) $(CFLAGS) -c src/fracpart.c -o src/fracpart.o

src/print_decoded_metar.o: src/print_decoded_metar.c src/metar_structs.h
	$(CC) $(CFLAGS) -c src/print_decoded_metar.c -o src/print_decoded_metar.o

src/stspack2.o: src/stspack2.c src/local.h
	$(CC) $(CFLAGS) -c src/stspack2.c -o src/stspack2.o

src/stspack3.o: src/stspack3.c src/local.h
	$(CC) $(CFLAGS) -c src/stspack3.c -o src/stspack3.o

clean:
	rm -f src/*.o metar_test libmetar.a

install: library
	cp include/metar.h /usr/local/include/
	chmod 0644 /usr/local/include/metar.h
	cp libmetar.a /usr/local/lib/
	chmod 0644 /usr/local/lib/libmetar.a
	ranlib /usr/local/lib/libmetar.a

