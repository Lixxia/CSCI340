#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include "floppy.h"

/* Initializes the MS-DOS device indicated by name and returns information about the device as a Disk.
*/
Disk physical_disk(char* name) {
    disk_t *floppy = (disk_t*) malloc(sizeof(disk_t));
    unsigned int buf;
    handle_t fd;

    // open file
    if((fd = open(name, O_RDONLY)) <0) {
        perror(name);
        exit(1);
    }
    // seek bytes per sector
    lseek(fd,11,SEEK_SET);
    // read bytes per sector
    read(fd,&buf,2);
    // assign value
    floppy->geometry.bytesPerSector = buf;
    printf("\nBytes per Sector: %d",floppy->geometry.bytesPerSector);

    // seek sectors per track
    lseek(fd,24,SEEK_SET);
    // read sectors per track
    read(fd,&buf,2);
    // assign value
    floppy->geometry.sectorsPerTrack = buf;
    printf("\nSectors per track: %d",floppy->geometry.sectorsPerTrack);

    // seek heads
    lseek(fd,26,SEEK_SET);
    // read heads
    read(fd,&buf,2);
    // assign value
    floppy->geometry.heads = buf;
    printf("\nHeads: %d",floppy->geometry.heads);

    // seek num sectors
    lseek(fd,19,SEEK_SET);
    // read num sectors
    read(fd,&buf,2);

    // seek cylinders = num tracks = num sectors / sectors per track
    floppy->geometry.cylinders = buf / floppy->geometry.sectorsPerTrack;
    printf("\nCylinders: %d",floppy->geometry.cylinders);

    floppy->floppyDisk = fd;
    return floppy;
}

/* Reads the given logical sector from the Disk and enters the data, byte-by-byte, from that sector into the given buffer. 
Buffer must be large enough to hold a sector's worth of data.

Allocate the buffer dynamically to match the size of a sector on the given disk.

Returns 1 if successful, 0 otherwise.
*/
int sector_read(Disk theDisk, unsigned int logicalSectorNumber, unsigned char* buffer) {
    unsigned int bps = theDisk->geometry.bytesPerSector;
    unsigned int offset = logicalSectorNumber*bps;
    if(lseek(theDisk->floppyDisk, offset, SEEK_SET) < 0) {
        perror("Error seeking.");
        return 0;
    }
    if(read(theDisk->floppyDisk, buffer, bps) < 0) {
        perror("Error reading.");
        return 0;
    }

    return 1;
}

/* Calls sector_read and then prints the contents of buffer to stdout. This may be one of 'c','x' or 'o' to produce output as char, hex or octal. 
Output is to be formatted 16 bytes to a line why the physical byte number of the first byte given in hex.

Ex:
Logical sector starts with byte 1234 (4d2 in hex) and buffer contains 32 bytes of numbers 1-32, printout would be:
0x04d2 001 002 003 004 005 006 007 010 011 012 013 014 015 016 017 020
0x04e2 021 022 023 024 025 026 027 030 031 032 033 034 035 036 037 040
*/
void sector_dump(Disk theDisk, int logicalSectorNumber, char type) {
    int bps = theDisk->geometry.bytesPerSector;
    unsigned char *buffer = (unsigned char*) malloc(bps);
    int offset = logicalSectorNumber*bps;
    char *format;
    int i,j;

    sector_read(theDisk,logicalSectorNumber,buffer);

    if(type == 'c') {
        // output as char
        format = "%c";
        printf("Char\n");
    }
    else if(type == 'x') {
        // output as hex
        format = "%01x\t";
        printf("Hex\n");
    }
    else if(type == 'o') {
        // output as octal
        format = "%o\t";
        printf("Oct\n");
    }

    for(i=0;i<32;i++) {
        printf("%04x\t",offset+(i*16));
        for (j = 0; j < 16; j++){
            printf(format, buffer[(i*16) + j]);
        }
        printf("\n");
    }
}
