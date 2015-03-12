#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
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

/*
HW4 Tasks:
(within execute)
fork a child process that runs in the background. This operation can be identified by determining
if an ampersand (&) is the last element in the command struct argv array. 

Redirect stdin to a text file. This operation can be identified by determining if a greater than '>'
is present in the command struct argv array. 

*/
int execute( command_t* p_cmd ) {
	int fds[2]; // input = fds[0], output = fds[1]
	pid_t bpid, fdcpid, cpid1, cpid2;
	command_t cmd_second;

	char* save_pipe;
	char* save_amp;
	char* save_angle;
	char** argv_second;

	int found, childStatus, pid, pipe_found, last, second_last, background;
	int fdchild_process_status;
	int outfile;
	char fullpath[PATH_LENGTH]; 
	char fullpath_second[PATH_LENGTH];
	char fullpath_back[PATH_LENGTH]; 
	char fullpath_redirect[PATH_LENGTH]; 

	background = FALSE;
	found = FALSE;
	last = p_cmd->argc - 1;
	second_last = last - 1;
	pipe_found = find_pipe(p_cmd);

	// check for &
	printf("we are here\n");
	if(p_cmd -> argv[last][0] == '&') {
		printf("the dpero\n %s", p_cmd -> argv[last]);
		// background process
		// need struct without &, use same method as pipe split?
		save_amp = p_cmd -> argv[last];
		p_cmd -> argv[last] = NULL;

		find_fullpath(fullpath_back,p_cmd);
		
		printf("yep\n");
		background = TRUE;

		// catching when child dies
		if (signal(SIGCHLD, sig_child_handler) == SIG_ERR) {
		    perror("unable to create new SIGCHLD signal handler");
		    exit(-1);
		}

		if (signal(SIGINT, sig_int_handler) == SIG_ERR) {
		    perror("Unable to create new SIGINT signal handler");
		    exit(-1);
		}

		if ((bpid = fork()) == 0) {
		    execv(fullpath_back, p_cmd -> argv);
		    perror("Child process terminated in error condition");
		    exit(-1);
		}

		p_cmd -> argv[last] = save_amp;
	}

	// need to check for '>'
	else if(second_last >= 1 && p_cmd -> argv[second_last][0] == '>') {
		// stdin redirect to file
		/* need location of '>', save and change to NULL
		get file name, should be second_lasttem in argv
		*/
		save_angle = p_cmd -> argv[second_last];
		p_cmd -> argv[second_last] = NULL;

		outfile = open(p_cmd -> argv[last], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP );

		find_fullpath(fullpath_redirect,p_cmd);

		if (outfile == -1) {
		    fprintf(stderr, "Failed to open file.\n");
		}
		else if ((fdcpid = fork()) == 0) {
		    dup2(outfile,1);
		    execv(fullpath_redirect,p_cmd -> argv);
		    exit(-1);
		}

		p_cmd -> argv[second_last] = save_angle;

		close(outfile);
		waitpid(fdcpid, &fdchild_process_status, 0);
	}
	
	else if(pipe_found != 0) {
		// pipe char exists
		argv_second = p_cmd -> argv + pipe_found + 1;
		save_pipe = p_cmd -> argv[pipe_found];
		p_cmd -> argv[pipe_found] = NULL;

		cmd_second.name = argv_second[0];

		find_fullpath(fullpath,p_cmd);
		find_fullpath(fullpath_second,&cmd_second);

		pipe(fds);
		
		cpid1 = fork();
		if ( cpid1 == 0 ) {
			close(1);
			dup(fds[1]);
			close(fds[0]);
			execv(fullpath,p_cmd->argv);
		}
		cpid2 = fork();
		if ( cpid2 == 0 ) {
			close(0);
			dup(fds[0]);
			close(fds[1]);
			execv(fullpath_second,argv_second);
		}

		p_cmd -> argv[pipe_found] = save_pipe;
		
		close(fds[0]);
		close(fds[1]);

		waitpid(cpid1, &childStatus, 0);
		waitpid(cpid2, &childStatus, 0);
	}

	else {
		found = find_fullpath(fullpath,p_cmd);
		// proceed normally
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
	}

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

/*
add a SIGCHLD signal handler function (prototype and implementation) in the hw2.c that is able to 
asynchronously reclaim (or reap) a forked child process that has terminated normally.
*/

static void sig_int_handler(int sig) {
	printf("In SIGINT handler\n");
	exit(0);
}

static void sig_child_handler(int sig) {
	printf("In SIGCHLD handler\n");
	int status;
	pid_t pid;
	while (( pid = waitpid(-1, &status, WNOHANG )) != -1 ) {
		printf("Child Process (%d) has Terminated\n", pid );
	}
}