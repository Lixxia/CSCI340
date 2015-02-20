#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "shell.h"

// builtin commands
const char* valid_builtin_commands[] = {"cd", "exit", NULL};

// implementation of all functions
void parse( char* line, command_t* p_cmd ) {
	int numArgs, n, j, tokenLength,stringPos,tokenStart;

	numArgs = count_params(line,' ');
	p_cmd -> argc = numArgs;
	
	// initial argv malloc
	p_cmd -> argv = (char**) malloc((numArgs+1)*sizeof(char*));
	
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
	p_cmd -> argv[n] = NULL;

	// malloc name and get value from argv
	p_cmd -> name = (char*) malloc(100);
	for (n = 0; p_cmd->argv[0][n] != '\0'; n++) {
		p_cmd->name[n] = p_cmd->argv[0][n];	
	}
	p_cmd->name[n] = '\0';
}

int execute( command_t* p_cmd ) {
	// modify this for use with pipe!
	int fds[2]; // input = fds[0], output = fds[1]
	pid_t cpid1, cpid2;

	char** pargs;
	char** cargs;

	int found = FALSE;
	int childStatus, pid, i;
	char fullpath[PATH_LENGTH]; 

	// pargs = (char**) malloc((p_cmd -> argc)*sizeof(char*));
	// cargs = (char**) malloc((p_cmd -> argc)*sizeof(char*));
	// for (i=0;i<p_cmd -> argc;i++) {
	// 	pargs[i] = malloc((100)*sizeof(char));
	// 	cargs[i] = malloc((100)*sizeof(char));
	// }
	/*
	char* save_pipe;
	char** argv2;

	argv2 = p_cmd -> argv + i + 1;
	save_pipe = p_cmd -> argv[i];
	p_cmd -> argv[i] = NULL;

	//before parent leaves execute
	p_cmd -> argv[i] = save_pipe;

	==============================
	char fullpath[];
	cmd_t cmd2;
	cmd2.name = argv2[0];
	find_fullpath(fullpath,&cmd2);
	*/

	found = find_fullpath(fullpath,p_cmd);

	// split_args(p_cmd,pargs,cargs);
	printf("\n pipe location= %i",find_pipe(p_cmd));

	// printf("\n pargs[0]=%s",pargs[0]);
	// printf("\n pargs[1]=%s",pargs[1]);
	// printf("\n cargs[0]=%s",cargs[0]);
	// printf("\n cargs[1]=%s",cargs[1]);

	// pipe(fds);

	// if ( cpid1 = fork() == 0 ) {
	// 	close(1);
	// 	dup(fds[1]);
	// 	close(fds[0]);
	// 	execv(fullpath,pargs);
	// }

	// if (cpid2 = fork() == 0 ) {
	// 	close(0);
	// 	dup(fds[0]);
	// 	close(fds[1]);
	// 	execv(fullpath,cargs);
	// }
	// // also wait on cpid1
	// close(fds[0]);
	// close(fds[1]);
	// waitpid(cpid1, &childStatus, 0);
	// waitpid(cpid2, &childStatus, 0);
	// return 0;
	

	if(found == 1) {
		pid = fork();
		if( pid == 0 ) { 
			//child process
			execv(fullpath, p_cmd->argv);
			perror("Execute terminated with an error condition!\n");
		}
		wait(&childStatus);
	}
	else {
		printf("Command not found.");
	}

	// for (i=0;i<p_cmd -> argc;i++) {
	// 	free(pargs[i]);
	// 	free(cargs[i]);
	// }
	// free(pargs);
	// free(cargs);

	return childStatus;
}

int find_fullpath( char* fullpath, command_t* p_cmd ) {
	int numPaths, n, position, length, start, j, namePos, exists;
	struct stat buffer;

	int pathFound = FALSE;
	char* env_path = getenv("PATH");

	numPaths = count_params(env_path,':');

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
		while(p_cmd -> name[namePos] != '\0') {
			fullpath[j] = p_cmd -> name[namePos];
			j++;
			namePos++;
		}
		fullpath[j] = '\0';	

		position++; //move to next path variable

		exists = stat( fullpath, &buffer );
		if ( exists == 0 && ( S_IFREG & buffer.st_mode ) ) {
			pathFound = TRUE;
			// printf("\nFile Exists.");
			break;
		} 
		else {
			// printf("\nNot a valid file.");
		}
	}

	return pathFound;
}

int is_builtin( command_t* p_cmd ) {
	int numBuiltin, n;

	numBuiltin = (sizeof(valid_builtin_commands)/sizeof(*valid_builtin_commands))-1;

	for(n=0;n<numBuiltin;n++) {
		if(is_same(p_cmd -> argv[0], valid_builtin_commands[n])) {
			return TRUE;
		}
	}
	return FALSE;
}

int do_builtin( command_t* p_cmd ) {
	int status = ERROR;

	if(p_cmd -> argv[0][0] == 'c') {
		status = chdir(p_cmd -> argv[1]);
		if(status == 0) {
			printf("Changed directory to: %s",p_cmd -> argv[1]);
			return SUCCESSFUL;
		}
		else if(status == -1) {
			perror("CD unsuccessful.");
		}
	}
	else {
		perror("Error.");
	}
	return status;
}

void cleanup( command_t* p_cmd ) {
	int i, numArgs;
	numArgs = p_cmd -> argc;

    for ( i = 0; i<numArgs; i++ ) {
    	free(p_cmd -> argv[i]);
    }
    free(p_cmd -> argv);
    free(p_cmd -> name); 
}

// Some helper functions

int is_same(char* first, const char* second) {
    char* i = first;
    const char* j = second;

    while(*i != '\0' || *j != '\0' ) {
        if(*i != *j) {
            return FALSE;
        }
        i++;
        j++;
    }
	return TRUE;
}

int count_params(char* string, char delimiter) {
	int position,count;

	position = 0;
	count = 0;
	while(string[position] != '\0') {

		if(string[position] == delimiter) {
			count++;
		}
		else if(string[position] == '\n' || string[position] == '\r' || string[position] == '\t') {
			string[position] = '\0';
		}
		position++;
	}
	return count+1;
}

int find_pipe(command_t* p_cmd) {
	int location;

	location = 0;
	while(p_cmd -> argv[location] != '|' && p_cmd -> argv[location] != NULL) {
		if(p_cmd -> argv[location][0] == '|') {
			return location;
		}
		location ++;
	}
	return 0;
}

int split_args(command_t* p_cmd, char **pargs, char **cargs) {
	int n, i, section, pos;

	section = 1; // before pipe and after pipe
	pos=0;
	for(n=0; n < p_cmd -> argc; n++) {
		// take all argv up to | and assign them to pargs
		i=0;

		// First set of arguments
		if(section == 1) {
			while(p_cmd -> argv[n][i] != '|' && p_cmd -> argv[n][i] != '\0') {
				pargs[n][i] = p_cmd->argv[n][i];
				i++;
			}
			pos++;
			pargs[n][i] = '\0';
		}
		// Check for pipe and advance in argv
		if(p_cmd -> argv[n][i] == '|') {
				section = 2;
				printf("pipe here");
				n++;
		}

		// Second set of arguments
		if(section == 2) {
			// use pos to move back to the beginning of array for cargs
			while(p_cmd -> argv[n][i] != '\0') {
				cargs[n-pos][i] = p_cmd->argv[n][i];
				i++;
			}
			cargs[n-pos][i] = '\0';
		}
	}
	pargs[n] = NULL;
	cargs[n-pos] = NULL;

	return 0;
}


/*
	NOTES:
	=====

	Every process has its own file descriptor table
	every file you open gets added to that table
	there are 3 initial ones: stin, stdout, sterr

	Can change any of these 3 by closing and duping 
	or opening something -> will populate into the 
	table at that spot.

	char* save_pipe;
	char** argv2;

	argv2 = p_cmd -> argv + i + 1;
	save_pipe = p_cmd -> argv[i];
	p_cmd -> argv[i] = NULL;

	//before parent leaves execute
	p_cmd -> argv[i] = save_pipe;

	==============================
	char fullpath[];
	cmd_t cmd2;
	cmd2.name = argv2[0];
	find_fullpath(fullpath,&cmd2);
*/