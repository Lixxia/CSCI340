
#define SUCCESSFUL 0
#define ERROR -1

#define TRUE 1
#define FALSE 0

typedef struct {
	
	char* name;		// name of command (must malloc)
	int argc;		// total number of arguments
	char** argv;	// argument values (must malloc)
	
} command_t;

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
void parse( char* line, command_t* p_cmd );

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
int execute( command_t* p_cmd );

/* ------------------------------------------------------------------------------

This function is used determine if the named command (cmd for short) entered by 
the user in the shell can be found in one of the folders defined in the PATH 
environment (env or short) variable.

HINT(s): Use getenv() system function to retrieve the folders defined in the 
		PATH variable. An small code segment is shown below that demonstrates 
		how to retrieve folders defined in your PATH.

			char* path_env_variable;
			path_env_variable = getenv( "PATH" );
			printf("PATH = %s\n", path_env_variable );

		The format of the PATH is very simple, the ':' character is delimiter, 
		i.e. used to mark the beginning and end of a folder defined in the path.

		Write a loop that parses each folder defined in the path, then uses this 
		folder along with the stat function (see "File/Directory Status" section 
		in the assignment PDF).
 
 function:
	- parameter(s): char* pointer that contains the fullpath to named command found 
					by this function, and a pointer to an existing command_t structure
	- return: TRUE if cmd is in the PATH, or FALSE if not in the PATH. 

*/
int find_fullpath( char* fullpath, command_t* p_cmd );

/* ------------------------------------------------------------------------------

This function will determine if command (cmd for short) entered in the shell by 
the user is a valid builtin command.

HINT(s): Use valid_builtin_commands array defined in shell.c

function:
	- parameter(s): pointer to a command_t structure
	- return: TRUE if the cmd is in array, else FALSE if not in array. 

*/
int is_builtin( command_t* p_cmd );

/* ------------------------------------------------------------------------------

This function is used execute built-in commands such as change directory (cd)

HINT(s): See man page for more information about chdir function 

function:
	- parameter(s): pointer to a command_t structure
	- return: SUCCESSFUL if no error occurred during chdir operation, or ERROR 
			if an error condition was generated by chdir operation.

*/
int do_builtin( command_t* p_cmd );

/* ------------------------------------------------------------------------------

This function is used to free memory that may be malloc'd for the name and argv 
fields in the command_t structure.  To be safe, you should also set the pointer 
values to NULL.
 
HINT(s): See man page for more information about free function
 
function:
	- parameter(s): pointer to a command_t structure
	- return: N/A

*/
void cleanup( command_t* p_cmd );
