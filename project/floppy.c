#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include "floppy.h"

/* Initializes the MS-DOS device indicated by name and returns information about the device as a Disk.
*/
int fd; /*global var set by open()*/
int bps; /*bytes per sector from disk */
/*
typedef struct {
    unsigned int bytesPerSector;
    unsigned int sectorsPerTrack;
    unsigned int heads;
    unsigned int cylinders;
} geometry_t;
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
    lseek(fd,13,SEEK_SET);
    // read sectors per track
    read(fd,&buf,2);
    // assign value
    floppy->geometry.sectorsPerTrack = buf;
    printf("\nSectors per track: %d",floppy->geometry.sectorsPerTrack);


    // lseek(fd,0,SEEK_SET); // moves cursor in fd
    // adresses in boot sector, lseek to those addresses and read info
    // populate all structs and return
    return 0;
}

/* Reads the given logical sector from the Disk and enters the data, byte-by-byte, from that sector into the given buffer. 
Buffer must be large enough to hold a sector's worth of data.

Allocate the buffer dynamically to match the size of a sector on the given disk.

Returns 1 if successful, 0 otherwise.
*/
int sector_read(Disk theDisk, unsigned int logicalSectorNumber, unsigned char* buffer) {
    bps = 512;
    read(fd, buffer, bps);
    printf("raejwalr: %s",buffer);
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

}
