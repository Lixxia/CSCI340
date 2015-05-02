# Operating Systems
A repository containing code for assignments in my operating systems course.

## HW1
In this assignment we were tasked with writing a small program that would read the values from a user inputed text file (in this case, `vectors.txt`). Each line in this text was a vector defined by a:
    1. magnitude (r), where r ∈ R
    2. direction (θ) measured in degrees, where θ ∈ R

Once these values were read in, the following would be printed out to the console (all using only two decimal places):
- vector magnitude
- vector direction (in radians)
- vector direction (in degrees)
- Using compute_x() function in utils.h, x component value 
- Using compute_y() function in utils.h, y component value 

## HW2
This assignment called for us to write a small shell for the linux kernel. It would accept basic commands from the user, check if they were built in (cd and exit) or look for the fullpath and execute it using a child process.

## HW3
This assignment extended the previous one (HW3). Using the existing code from HW2 I added pipe functionality to my shell.

## HW4
Again building upon the previous two assignments. In addition to the system and standard library functions used in assignments 2 and 3, this assignment will also use the system and standard library functions listed below:

- Duplicate a file desciptor: `int dup2(int oldfd, int newfd)`
- Create/open a new file descriptor: `int open(const char* pathname, int flags, mode_t mode)`
- signals: `sighandler_t signal(int signum, sighandler_t handler)`

## HW5
Developed a small software application that uses thread computation with a Monte Carlo simulation to estimate Pi.

## HW6
Simulation of [dining philosophers problem](http://en.wikipedia.org/wiki/Dining_philosophers_problem).

## HW7
Assignment to compare the effectiveness (in terms of external fragmentation, number of probes, and allocation failures) of three different dynamic partitioning placement algorithms. 

Each element in the array represents a single unit of memory. Value of zero indicates a free unit, a positive value represents a unit in use. 

## Project
Designed, developed and tested a C-program that will open and read a MS-DOS FAT16 filesystem. 

Example output when run with `floppy.img`, reading sector 35 and output in hex format:

```
0x4600  54  68  69  73  20  69  73  20  61  20  73  69  6d  70  6c  65  
0x4610  20  52  45  41  44  4d  45  20  66  69  6c  65  20  66  6f  72  
0x4620  20  74  68  65  20  70  75  72  70  6f  73  65  20  6f  66  20  
0x4630  68  61  76  69  6e  67  20  61  74  20  6c  65  61  73  74  20  
0x4640  6f  6e  65  0a  66  69  6c  65  20  6f  6e  20  6f  75  72  20  
0x4650  66  6c  6f  70  70  79  20  64  72  69  76  65  2e  20  20  47  
0x4660  6f  6f  64  20  6c  75  63  6b  20  77  69  74  68  20  74  68  
0x4670  65  20  6c  61  62  2e  20  20  4f  68  68  68  2c  20  49  20  
0x4680  61  6c  6d  6f  73  74  0a  66  6f  72  67  6f  74  2c  20  68  
0x4690  61  76  65  20  61  20  67  72  65  61  74  20  53  70  72  69  
0x46a0  6e  67  20  42  72  65  61  6b  21  0a  0a  54  6f  6e  79  0a  
0x46b0  0a  00  00  00  00  00  00  00  00  00  00  00  00  00  00  00  
0x46c0  00  00  00  00  00  00  00  00  00  00  00  00  00  00  00  00  
...
0x47e0  00  00  00  00  00  00  00  00  00  00  00  00  00  00  00  00  
0x47f0  00  00  00  00  00  00  00  00  00  00  00  00  00  00  00  00  
```

