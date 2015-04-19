CC=gcc
CFLAGS=-c -O2 -Wall -g

all: main

main: floppy.o main.o
    $(CC) floppy.o main.o -o main 

floppy.o: floppy.c
    $(CC) $(CFLAGS) floppy.c

main.o: main.c
    $(CC) $(CFLAGS) main.c

clean:
    /bin/rm -f main *.o *.gz

run:
    ./main floppy.img

tarball:
    tar -cvzf project.tar.gz *