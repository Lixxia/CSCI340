#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "shell.h"

// -----------------------------------
// Main function 
// Arguments:	argc = number of arguments suppled by user
//				argv = array of argument values
//

int main( int argc, char** argv ) {
	int done;
	done = FALSE;

	while (!done) {
		char line[CMD_LENGTH];
		char fullpath[PATH_LENGTH]; 
		command_t cmd;

		printf("\nInput a command ");
		fgets(line, CMD_LENGTH, stdin);

   		parse(line,&cmd);

   		// if (is_builtin(&cmd) && cmd.name[0] == 'e') {
   		// 	done = TRUE;
   		// }
   		// else if (is_builtin(&cmd)) {
   		// 	do_builtin(&cmd);
   		// }
   		// else if (find_fullpath(fullpath, &cmd)) {
   			execute(&cmd);
   		// }
   		// else {
   		// 	perror("Error in main:");
   		// }
   		cleanup(&cmd);
	}

	return 0;

} // end main function
