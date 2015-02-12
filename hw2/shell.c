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


void parse( char* line, command_t* p_cmd ) {
	int numArgs, i, n, j, tokenLength,stringPos,tokenStart;
	// find . -name hw2.c -print
	// name = name of command
	// argc = number of arguments = 5
	// argv = pointer to array of strings. = "find" "." "-name" "hw2.c" "-print" "NULL" <- important for last spot
	// find . -name hw2.c -print

	//have to malloc for name
	//have to create array for argv
	// loop arc times, each time mallocing space for parameters

	// make this a countParams function?
	numArgs=0;
	i=0;
	while(line[i] != '\n') {
		printf("%c", line[i]);
		if(line[i] == ' ') {
			numArgs++;
		}
		i++;
	}

	numArgs += 1;
	p_cmd -> argc = numArgs;
	
	// initial argv malloc
	p_cmd -> argv = (char**) malloc((numArgs+1)*sizeof(char*)); //malloc num things * size of each thing and cast it to be pointer to a pointer
	
	stringPos=0; 
	for(n=0;n<numArgs;n++) {
		tokenLength=0;
		// want to continue on for here, need a piece to skip whitespace and return to top
		while(line[stringPos] != ' ' && line[stringPos] != '\0') {
			stringPos++; // controlling where we are in the string
			tokenLength++; // keeping length of string
		}

		// make sure length of first command > 0. ie it exists
		if(tokenLength > 0) {
			// malloc space for each argument within argv
			p_cmd -> argv[n] = malloc((tokenLength+1)*sizeof(char));
			tokenStart=stringPos-tokenLength;
			j=0;
			printf("val of j: %i",j);
			while(j < tokenLength) {
				p_cmd -> argv[n][j] = line[tokenStart]; // get jth char of argv[n]
				j++;
				tokenStart++;
			}
			// need to add null char at the end
			p_cmd -> argv[n][j] = '\0';
		}

		stringPos++; //to move to the next section - skip whitespace
	}
	p_cmd -> argv[n+1] = NULL;
	
	// printf("argv : %s",p_cmd[0].argv[0]);
	// printf("argv 2: %s", p_cmd[0].argv[1]);
	// printf("argv 3: %s",p_cmd[0].argv[2]);
	// printf("argv 4: %s", p_cmd[0].argv[3]);

	//malloc name 
	p_cmd -> name = (char*) malloc((numArgs+1)*sizeof(char));
	p_cmd -> name = p_cmd[0].argv[0];

}

/* ------------------------------------------------------------------------------

This function will only execute external commands (cmd for short), such as ls 
or cp, using the execv system function. In the execute function, you must fork 
a child process. 

 HINT(s): 
 (1) An example is provided that demonstrates how the fork and wait operations 
 	are coded (see "Executing a Command" section in assignment PDF).
 (2) The execv is assuming the full path name for the cmd is provided (i.e. the 
 	exact file location, starting from / or the root folder). For instance, simply 
 	providing the "pwd" cmd is equivalent to "./pwd" (i.e. pwd cmd is located in 
 	the same folder as parent process). Instead, the full path (/bin/pwd) must be 
 	given. To achieve this, use the find_fullpath function implemented in this 
 	header file. An incomplete code segement is provided below.

			int fnd = FALSE;
			char* fullpath;

			fnd = find_fullpath( fullpath, p_cmd );
		
			if ( fnd ) {
				if ( fork() == 0 ) {
					execv( fullpath, p_cmd->argv );
					...
				}
				//child returns a PID of 0 - !0 parent. only child does exec ^
				wait( ... );  //wait until execv is finished - pass in NULL?
				// int stat;  wait(&stat);
			} else {
				// display to user cmd not found
			}


 
 function:
	- parameter(s): pointer to a command_t structure
    - return: status code from wait function.

*/

int execute( command_t* p_cmd ) {
	int found = FALSE;
	int childStatus;
	char fullpath[200]; 

	found = find_fullpath(fullpath,p_cmd);

	printf("\nfullpath=== %s\n",fullpath);

	if (found) {
		if( fork() == 0 ) { 
			//child process
			execv(fullpath, p_cmd->argv);
			perror("Execute terminated with an error condition!\n");
			exit(1);
		}

		wait(&childStatus);
	}
	else {
		printf("Command not found.");
	}

	return 0;

}

int find_fullpath( char* fullpath, command_t* p_cmd ) {
	int i, numPaths, n, position, length, start, j, namePos, exists;
	struct stat buffer;

	char* cmd = p_cmd -> name;
	int pathFound = FALSE;
	char* env_path = getenv("PATH");

	// printf("PATH = %s\n", env_path );

	numPaths=0;
	i=0;
	while(env_path[i] != '\0') {
		printf("%c", env_path[i]);
		if(env_path[i] == ':') {
			numPaths++;
		}
		i++;
	}
	numPaths++;
	// printf("num paths: %i", numPaths);

	position = 0;
	
	for(n=0; n < numPaths; n++) {
		length = 0;
		while(env_path[position] != ':' && env_path[position] != '\0') {
			position++;
			length++;
		}

		start=position-length;
		j=0;
		while(j < length) {
			fullpath[j] = env_path[start];
			j++;
			start++;
		}

		fullpath[j] = '/';

		j++;
		namePos=0;
		while(cmd[namePos] != '\0') {
			fullpath[j] = cmd[namePos];
			j++;
			namePos++;
		}
		fullpath[j] = '\0';
		// printf("\nPath to test: %s",fullpath);		

		position++; //move to next path variable

		exists = stat( fullpath, &buffer );
		if ( exists == 0 && ( S_IFDIR & buffer.st_mode ) ) {
			printf("\nDirectory Exists.");
		} 
		else if ( exists == 0 && ( S_IFREG & buffer.st_mode ) ) {
			pathFound = TRUE;
			// set fullpath = fullpath;

			// fullpath[0] = fullpath[0];
			printf("\nFile Exists.");
			break;
		} 
		else {
			printf("\nNot a valid file or directory.");
		}

	}

	printf("\nfinal fullpath: %s",fullpath);
	return pathFound;
}

int is_builtin( command_t* p_cmd ) {
	//command -v yap
	//exit code 0 if there, exit 1 if not
	//use instead of which for more os compatibility! 
	return 0;

}

int do_builtin( command_t* p_cmd ) {
	return 0;

}

void cleanup( command_t* p_cmd ) {
	//free until NULL 
	// free up anything that's been malloc'd
	// free();

	//where to free fullpath
}

