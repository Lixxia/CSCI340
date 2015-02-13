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

void parse( char* line, command_t* p_cmd ) {
	printf("\nBEGIN PARSE");
	int numArgs, i, n, j, tokenLength,stringPos,tokenStart;

	// make this a countParams function?
	numArgs=0;
	i=0;
	while(line[i] != '\0') {
		if(line[i] == ' ') {
			numArgs++;
		}
		else if(line[i] == '\n' || line[i] == '\r') {
			line[i] = '\0';
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
	p_cmd -> argv[n] = '\0';

	// malloc name and get value from argv
	p_cmd -> name = (char*) malloc(100);
	p_cmd -> name = p_cmd[0].argv[0];
	printf("\nEND PARSE");
}

int execute( command_t* p_cmd ) {
	printf("\n BEGIN EXECUTE");
	int found = FALSE;
	int childStatus;
	char fullpath[PATH_LENGTH]; 

	found = find_fullpath(fullpath,p_cmd);

	if(found == 1) {
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
	printf("\n END EXECUTE");
	return childStatus;
}

int find_fullpath( char* fullpath, command_t* p_cmd ) {
	printf("\n BEGIN FIND FP");
	int i, numPaths, n, position, length, start, j, namePos, exists;
	struct stat buffer;

	char* cmd = p_cmd -> name;
	int pathFound = FALSE;
	char* env_path = getenv("PATH");

	// printf("PATH = %s\n", env_path );

	numPaths=0;
	i=0;
	while(env_path[i] != '\0') {
		if(env_path[i] == ':') {
			numPaths++;
		}
		i++;
	}
	numPaths++;

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
		// if ( exists == 0 && ( S_IFDIR & buffer.st_mode ) ) {
		// 	printf("\nDirectory Exists.");
		// } 
		if ( exists == 0 && ( S_IFREG & buffer.st_mode ) ) {
			pathFound = TRUE;
			printf("\nFile Exists.");
			break;
		} 
		else {
			printf("\nNot a valid file.");
		}
	}

	printf("\n END FIND FP");
	return pathFound;
}

int is_builtin( command_t* p_cmd ) {
	printf("\n BEGIN ISBUILTIN");
	int numBuiltin, n, i, found;
	char* cmd = p_cmd -> name;

	numBuiltin = (sizeof(valid_builtin_commands)/sizeof(*valid_builtin_commands))-1;
	found = FALSE;
	
	for(n=0;n<numBuiltin;n++) {
		i=0;
		while(cmd[i] != '\0') {
			if(cmd[i] == valid_builtin_commands[n][i]) {
				found = TRUE;
				printf("same");
			}
			else {
				found = FALSE;
				printf("diff");
			}
			printf("\ntestchar '%c'\n",cmd[i]);
			i++;
		}
		printf("\ntestchar2 '%c'\n",valid_builtin_commands[n][i]);
		if(found == TRUE && valid_builtin_commands[n][i] == '\0') {
			break;
		}
	}

	printf("\n END ISBUILTIN");

	return found;
}

int do_builtin( command_t* p_cmd ) {
	printf("\n BEGIN DOBUILTIN");
	int status;

	if(p_cmd -> name[0] == 'c') {
		status = chdir(p_cmd -> argv[1]);
		if(status == 0) {
			printf("Changed directory to: %s",p_cmd -> argv[1]);
		}
		else if(status == -1) {
			perror("Error in builtin.");
		}
	}
	else if (p_cmd -> name[0] == 'e') {
		status=SUCCESSFUL;
		printf("Exiting process.");
	}
	else {
		status=ERROR;
		perror("Something went wrong in do_builtin.");
	}
	printf("\n END DOBUILTIN");

	return status;
}

void cleanup( command_t* p_cmd ) {
	printf("\n BEGIN CLEANUP");
	int i, numArgs;
	numArgs = p_cmd -> argc;

    for ( i = 0; i<numArgs; i++ ) {
    	free(p_cmd -> argv[i]);
    }
    free(p_cmd -> argv);
    // free(p_cmd -> name);
    printf("\n END CLEANUP");
}

