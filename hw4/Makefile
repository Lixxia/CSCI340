CC=gcc
CFLAGS=-c -g -Wall

all: hw4

hw4: shell.o hw4.o
	$(CC) -g shell.o hw4.o -o hw4

shell.o: shell.c
	$(CC) $(CFLAGS) shell.c

hw4.o:	hw4.c
	$(CC) $(CFLAGS) hw4.c

clean:
	/bin/rm -f hw4 *.o *.gz

run:
	./hw4

tarball:
	tar -cvzf barber.tar.gz *
