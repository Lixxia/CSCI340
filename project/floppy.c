/* Initializes the MS-DOS device indicated by name and returns information about the device as a Disk.
*/
int fid; /*global var set by open()*/
int bps; /*bits per sector from disk */

Disk physical_disk(char* name) {
    fid = open(name, O_RDONLY);
}

/* Reads the given logical sector from the Disk and enters the data, byte-by-byte, from that sector into the given buffer. Buffer must be large enough to hold a sector's worth of data.

Allocate the buffer dynamically to match the size of a sector on the given disk.

Returns 1 if successful, 0 otherwise.
*/
int sector_read(Disk theDisk, unsigned int logicalSectorNumber, unsinged char* buffer) {
    read(fid,buffer, bps);
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
