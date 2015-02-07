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
				- argc = 2
				- argv = {"/mnt/cdrom"} 

        EXAMPLE#2, if the user enters

				ls -la

		at the shell prompt, line would contain "ls -la". Using the 
		space character as the delimiter, the fields in the command 
		struct would be:

				- name = "ls"
				- argc = 2
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
	// need to write strcopy function

	int numArg;
	// find . -name hw2.c -print
	// name = name of command
	// argc = number of arguments = 5
	// argv = pointer to array of strings. = "find" "." "-name" "hw2.c" "-print" "NULL"

	//have to malloc for name
	//have to create array for argv
	// loop arc times, each time mallocing space for parameters
	numArg=0;
	while(line[numArg] != NULL) {
		// count, keep track of how many args there are --> argc
		// count spaces, add 1?
		if(line[numArg] == ' ') {
			numArg++;
		}
	}
	p_cmd.argc = numArg;

	//malloc name 
	p_cmd --> name = (char*) malloc((count+1)*sizeof(char));

	//malloc argv name
	p_cmd --> argv = (char**) malloc((p_cmd --> argc+1)*sizeof(char*)); //malloc num things * size of each thing and cast it to be pointer to a pointer

}

int execute( command_t* p_cmd ) {
	return 0;

}

int find_fullpath( char* fullpath, command_t* p_cmd ) {
	return 0;

}

int is_builtin( command_t* p_cmd ) {
	return 0;

}

int do_builtin( command_t* p_cmd ) {
	return 0;

}

void cleanup( command_t* p_cmd ) {
	//free until NULL 
	// free up anything that's been malloc'd
}

