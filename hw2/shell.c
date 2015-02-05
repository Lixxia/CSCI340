// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "shell.h"

// --------------------------------------------
// Currently only two builtin commands for this
// assignment exist, however in future, more could 
// be added to the builtin command array.
// --------------------------------------------
const char* valid_builtin_commands[] = {"cd", "exit", NULL};

// --------------------------------------------
// Implementation for each prototype below
// --------------------------------------------


/* ------------------------------------------------------------------------------
This function will parse the command (cmd for short) and its arguments specified 
by the user.  
HINT(s): This function is "essentially" an exercise in string parsing. 

        EXAMPLE#1, if the user enters

				cd /mnt/cdrom

		at the shell prompt, line would contain "cd /mnt/cdrom". 
		Using the space character as the delimiter, the fields in 
		the command struct would be:

				- name = "cd"
				- argc = 1
				- argv = {"/mnt/cdrom"} 

        EXAMPLE#2, if the user enters

				ls -la

		at the shell prompt, line would contain "ls -la". Using the 
		space character as the delimiter, the fields in the command 
		struct would be:

				- name = "ls"
				- argc = 1
				- argv = {"-la"} 

        EXAMPLE#3, if the user enters nothing at the shell prompt (i.e. 
        simply hits the return key), line would NULL, and the fields in 
        the command struct would be:

				- name = 0
				- argc = 0
				- argv = 0 
 
 function:
	- parameter(s): char* pointer that contains the cmd and its arguments, 
					and a pointer to an existing command_t structure
	- return: N/A
*/
void parse( char* line, command_t* p_cmd ) {
	

}

int execute( command_t* p_cmd ) {

}

int find_fullpath( char* fullpath, command_t* p_cmd ) {

}

int is_builtin( command_t* p_cmd ) {

}

int do_builtin( command_t* p_cmd ) {

}

void cleanup( command_t* p_cmd ) {

}

