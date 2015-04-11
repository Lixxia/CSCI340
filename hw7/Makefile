CC=gcc
CFLAGS=-c -O2 -Wall -g

all: hw7

hw7: mem.o hw7.o
	$(CC) mem.o hw7.o -o hw7 

mem.o: mem.c
	$(CC) $(CFLAGS) mem.c

hw7.o: hw7.c
	$(CC) $(CFLAGS) hw7.c

clean:
	/bin/rm -f hw7 *.o *.gz

run:
	./hw7 500 20 40 1234

tarball:
	tar -cvzf barber.tar.gz *