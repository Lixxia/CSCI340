#include <stdlib.h>
#include "floppy.h"

int main(int argc, char** argv) {
    //initialize errything
    Disk floppy = physical_disk(argv[1]);
    int sector_num = atoi(argv[2]);
    char type = argv[3][0];
    sector_dump(floppy,sector_num,type);

    close(floppy->floppyDisk); // close file descriptor
    free(floppy); // free malloc'd disk

    return 0;
}