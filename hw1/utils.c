// -----------------------------------
// CSCI 340 - Operating Systems I 
// Spring 2015 (Tony Leclerc, Brent Munsell)
// utils.c implementation file
// Homework Assignment 1
//
// -----------------------------------

// NOTES:
// gcc -Wall utils.c hw1.c -lm
// -lm links with math library, -Wall tells to print out warnings
// use -g flag for debugging
// gdb a.out (default thingydo)
// >> run
// will tell you where it got the errors

// tar czf barber.tgz folder
//or tar.gz

// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included

#include <stdio.h>	// IO functions
#include <stdlib.h> 	// atof function
#include <math.h>	// math functions
#include "utils.h"	// your own functions

// Definitions
#define MAX_STRING_LEN 100 //length of the file buffer string

int read( char* file_name, v_struct* p_vec_array ) {
	FILE *fp;
	fp = fopen(file_name, "r");

	char buffer[MAX_STRING_LEN];
	int i;
	int lineCount = 0;

	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		i = 0;
		while(buffer[i] != ',' && buffer[i] != '\0') {
			i++;
		}
		buffer[i] = '\0';

		// store values
		p_vec_array[lineCount].r = atof(buffer);
		p_vec_array[lineCount].theta = atof(&buffer[i+1]);

		// printf("R: %f\n",p_vec_array[lineCount].r);
		// printf("Theta: %f\n",p_vec_array[lineCount].theta);

		// If the provided vector direction is > 360, then your code must subtract 360 from θ, i.e. θ = θ − 360, and
		// If the provided vector direction is < −360, then your code must add 360 to θ, i.e. θ = θ + 360.
		if(p_vec_array[lineCount].theta > 360) {
			p_vec_array[lineCount].theta = p_vec_array[lineCount].theta - 360;
		}
		else if(p_vec_array[lineCount].theta < -360) {
			p_vec_array[lineCount].theta = p_vec_array[lineCount].theta + 360;
		}

		// printf("new theta: %f\n",p_vec_array[lineCount].theta);
		lineCount++;
	}

	fclose(fp);
	return lineCount;
}

double x_component( v_struct* p_vector ) {
	double thetaRad = p_vector[0].theta*(PI/180); //degrees -> radians
	return p_vector[0].r*(cos(thetaRad));
}

double y_component( v_struct* p_vector ) {
	double thetaRad = p_vector[0].theta*(PI/180); //degrees -> radians
	return p_vector[0].r*(sin(thetaRad));
}
