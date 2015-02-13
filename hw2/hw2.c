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

// -----------------------------------
// Main function 
// Arguments:	argc = number of arguments suppled by user
//				argv = array of argument values
//
//
int main( int argc, char** argv ) {
	//write a prompt, read in a command print it back

	// ----------------------------------
	// Pseudocode
	// ----------------------------------
	// while ( !done )
	//		print the prompt (i.e. >, #, or $ ... your choice)
	//		read command line
	//		parse the command line
    //      if (parsed command is "exit")
    //      	done = true
	//  	else if ( is _builtin( ... ) )
	//			do_builtin( ... )
	//		else if ( find_fullpath( ... ) )
	//  		execute( ... )
	//		else
	//			error()
	// 		cleanup( ... )
	//

	char line[CMD_LENGTH];
	command_t cmd;
	int done;

	done = FALSE;

	while (!done) {
		printf("\nInput a command ");
		fgets(line, CMD_LENGTH, stdin);
   		printf("Entered: %s", line);

   		parse(line,&cmd);

   		if (is_builtin(&cmd) && cmd.name[0] == 'e') {
   			done = TRUE;
   		}
   		else if (is_builtin(&cmd)) {
   			do_builtin(&cmd);
   		}
   		else {
   			execute(&cmd);
   		}
   		// else {
   		// 	// execute(&cmd);
   		// 	// printf("executed");
   		// 	perror();
   		// }
   		cleanup(&cmd);
	}

	return 0;

} // end main function
