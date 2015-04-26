#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include "floppy.h"

#define BYTES_TO_LINE 16
#define BUFFER_DATA 32

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
    // printf("\nBytes per Sector: %d",floppy->geometry.bytesPerSector);

    // seek sectors per track
    lseek(fd,24,SEEK_SET);
    // read sectors per track
    read(fd,&buf,2);
    // assign value
    floppy->geometry.sectorsPerTrack = buf;
    // printf("\nSectors per track: %d",floppy->geometry.sectorsPerTrack);

    // seek heads
    lseek(fd,26,SEEK_SET);
    // read heads
    read(fd,&buf,2);
    // assign value
    floppy->geometry.heads = buf;
    // printf("\nHeads: %d",floppy->geometry.heads);

    // seek num sectors
    lseek(fd,19,SEEK_SET);
    // read num sectors
    read(fd,&buf,2);

    // seek cylinders = num tracks = num sectors / sectors per track
    floppy->geometry.cylinders = buf / floppy->geometry.sectorsPerTrack;
    // printf("\nCylinders: %d",floppy->geometry.cylinders);

    floppy->floppyDisk = fd;
    return floppy;
}

int sector_read(Disk theDisk, unsigned int logicalSectorNumber, unsigned char* buffer) {
    unsigned int bps = theDisk->geometry.bytesPerSector;
    unsigned int offset = logicalSectorNumber*bps;
    // seek to sector
    if(lseek(theDisk->floppyDisk, offset, SEEK_SET) < 0) {
        perror("Error seeking.");
        return 0;
    }
    // read sector
    if(read(theDisk->floppyDisk, buffer, bps) < 0) {
        perror("Error reading.");
        return 0;
    }

    return 1;
}

void sector_dump(Disk theDisk, int logicalSectorNumber, char type) {
    int bps = theDisk->geometry.bytesPerSector;
    unsigned char *buffer = (unsigned char*) malloc(bps);
    int offset = logicalSectorNumber*bps;
    char *format;
    int logical_sector,blocks;

    sector_read(theDisk,logicalSectorNumber,buffer);

    if(type == 'c') {
        // output as char
        format = "%c";
        // printf("Char\n");
    }
    else if(type == 'x') {
        // output as hex
        format = "%02x\t";
        // printf("Hex\n");
    }
    else if(type == 'o') {
        // output as octal
        format = "%03o\t";
        // printf("Oct\n");
    }

    for(logical_sector=0;logical_sector<BUFFER_DATA;logical_sector++) {
        // print out beginning of logical sector
        printf("%#04x\t", offset+(logical_sector*BYTES_TO_LINE));

        // print out remaining blocks
        for (blocks=0; blocks<BYTES_TO_LINE; blocks++){
            printf(format, buffer[(logical_sector*BYTES_TO_LINE)+blocks]);
        }
        printf("\n"); // end of line
    }
}
