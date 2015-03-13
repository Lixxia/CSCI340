#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "shell.h"

// Signal Handler Functions
static void sig_int_handler(int sig) {
   exit(0);
}

static void sig_child_handler(int sig) {
   int status;
   pid_t pid;
   while (( pid = waitpid(-1, &status, WNOHANG )) > 0 ) {
      // Child process has terminated
   }
}

/* -----------------------------------
Main function 
Arguments:  argc = number of arguments suppled by user
            argv = array of argument values
*/

int main( int argc, char** argv ) {
	int done;
	done = FALSE;

   // Signal handling
   if (signal(SIGCHLD, sig_child_handler) == SIG_ERR) {
       perror("unable to create new SIGCHLD signal handler");
       exit(-1);
   }

   if (signal(SIGINT, sig_int_handler) == SIG_ERR) {
       perror("Unable to create new SIGINT signal handler");
       exit(-1);
   }

	while (!done) {
		char line[CMD_LENGTH];
		char fullpath[PATH_LENGTH]; 
		command_t cmd;

		printf("\n▶ "); // prompt
		fgets(line, CMD_LENGTH, stdin);

   		parse(line,&cmd);

   		if (is_builtin(&cmd) && cmd.name[0] == 'e') {
   			done = TRUE;
   		}
   		else if (is_builtin(&cmd)) {
   			do_builtin(&cmd);
   		}
   		else if (find_fullpath(fullpath, &cmd)) {
   			execute(&cmd);
   		}
   		else {
   			perror("Error in main:");
   		}
   		cleanup(&cmd);
	}

	return 0;

} 
