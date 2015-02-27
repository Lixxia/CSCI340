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