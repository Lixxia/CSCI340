// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included
#include <stdio.h>		// IO functions
#include <stdlib.h> 	// atof function
#include <math.h>		// math functions
#include "utils.h"		// your own functions

// ----------------------------------------------
// Define constants
#define TOTAL_VECS 10	// constant that defines the total number of vectors

// -----------------------------------
// Main function 
// Arguments:	argc = number of arguments suppled by user
//				argv = array of argument values

int main( int argc, char** argv ) {
	int i;
	v_struct vec_array[TOTAL_VECS];

	if(argv[1] == NULL) {
		printf("Program expects filename.");
		exit(EXIT_FAILURE);
	}

	read(argv[1],vec_array);

	for(i=0; i<TOTAL_VECS; i++) {
		printf("r = %.2f, ",vec_array[i].r);
		printf("theta = %.2f degrees, ", vec_array[i].theta);
		printf("theta = %.2f radians, ",vec_array[i].theta*(M_PI/180));
		printf("x_comp = %.2f, ",x_component(&vec_array[i]));
		printf("y_comp = %.2f\n",y_component(&vec_array[i]));
	}
	
	// --------------------------------
	// Steps to be performed
	//	1. Define variables (including arrays)
	//	2. Using read() function in utils.h, read vectors defined in text file 
	//		- the location of the text file (i.e. vectors.txt) must be defined in argv[1]
	//  3. Loop through array of vector structs and print to console:
	//		- vector magnitude (using only 2 decimal places)
	//		- vector direction (in radians using only 2 decimal places)
	//		- vector direction (in degrees using only 2 decimal places)
	//		- Using compute_x() function in utils.h, x component value (using only 2 decimal places)
	//		- Using compute_y() function in utils.h, y component value (using only 2 decimal places)
	//	    in the following format
	//		r = <value>, theta = <value> degrees, <value> radians, x_comp = <value>, y_comp = <value>
	//	    For example:
	//		r = 10.00, theta = 180.00 degrees, theta = 3.14 radians, x_comp = -10.00, y_comp = 0.00 
	
	return 0;

}
