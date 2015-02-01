// -----------------------------------
// CSCI 340 - Operating Systems I 
// Spring 2015 (Tony Leclerc, Brent Munsell)
// utils.c implementation file
// Homework Assignment 1
//
// -----------------------------------

// gcc -Wall utils.c hw1.c -lm
// -lm links with math library, -Wall tells to print out warnings
// use -g flag for debugging
// gdb a.out (default thingydo)
// >> run
// will tell you where it got the errors
// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included

// tar czf barber.tgz folder
//or tar.gz

#include <stdio.h>	// IO functions
#include <stdlib.h> 	// atof function
#include <math.h>	// math functions
#include "utils.h"	// your own functions


//definitions
#define MAX_STRING_LEN 100 //length of the string


int read( char* file_name, v_struct* p_vec_array ) {
	FILE *fp;
	fp = fopen(file_name, "r");

	char buffer[MAX_STRING_LEN];
	int i;
	int lineCount = 0;

	while (fgets(buffer, sizeof buffer, fp) != NULL)
	{
		i = 0;
		while(buffer[i] != ',' && buffer[i] != '\0') {
			i++;
		}
		buffer[i] = '\0';

		//store values
		p_vec_array[lineCount].r = atof(buffer);
		p_vec_array[lineCount].theta = atof(&buffer[i+1]);

		printf("R: %f\n",p_vec_array[lineCount].r);
		printf("Theta: %f\n",p_vec_array[lineCount].theta);
		lineCount++;
	}
	printf("%d Lines",lineCount);

	fclose(fp);
	return lineCount;
}


// Function prototype that uses the magnitude and 
// direction values in vector structure
// to compute the x component, i.e. r*cos(theta)
//
// Note(s): 
//	- theta must be converted from degrees to radians 
//	  prior to calculation.
//
// Arguments: 	p_vector = point to v_struct 
// Return: 	double that represents x component value
//

double x_component( v_struct* p_vector ) {
	printf("%d",p_vector.r*(cos(p_vector.theta)));
	return 0;
}

double y_component( v_struct* p_vector ) {
	return 0;
}
