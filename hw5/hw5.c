#include <stdio.h>         // for printf()
#include <pthread.h>       // for pthread_xxx() routines
#include "mcsim.h"

double gcount;    // global counter -- a double to handle large sums (billions+)
long numits;      // global variable for number of iterations (see step 3 below)

int main( int argc, char** argv ) {

	gcount = 0;

	int num_threads, num_iter, max_threads, thread_id, exitstatus;
	struct rlimit *rlim;
	rlim = (struct rlimit *) malloc(sizeof(rlim_t) * 2);

	num_threads = argv[1];
	num_iter = argv[2];
	getrlimit(RLIMIT_AS, rlim);
	max_threads = rlim->rlim_cur;
	perror("shits goin down");
	// clock_gettime(CLOCK_REALTIME, struct timespec *tp)
	printf("num threads is %d", max_threads);
	// p_thread threads[num_threads];

	if(num_threads < max_threads) {
		// do normal
		pthread_t threads[num_threads];
		// get starttime
		
	}
	else {
		// exit
	}

	/*

	---------------------------------------------------------------------
	Step-by-Step procedure for Monte carlo simulator to estimate PI
	---------------------------------------------------------------------
	1. Create the following variables:
		- pointer to an array of pthread structures x
		- counting variables for loops
		- thread id x
		- exit status of joined thread x
		- number of threads to run x
		- maximum number of threads (hint: rlimit) ~
		- nanosecond start and stop time structs (hint: timespec)
	2. Get number of threads input by user from argv[1]
	3. Get number of iterations input by user from argv[2]
	4. Get the maximum number of threads the OS can create (hint: getrlimit function)
	5. If are number of threads > maximum OS threads then goto 14, else goto step 6
	6. Allocate an array of pthread structures using number of threads input by user (see step 2)
	7. Get start time of simulation and store in time struct
	8. Use a loop to create a pthread for each index position in pthread array (created in step 5), if an error occurs during thread creation - exit simulation program immediately (hint: pthread_create function)
	9. Use a loop to join each pthread in created in the pthread array, if an error occurs during thread join operation - exit simulation program immediately (hint: pthread_join function)
	10. Get stop time of simulation and store in time struct
	11. Print out the:
		- value of PI using 6 decimal places
		- value stored in global variable gcount using 0 decimal places
		- time in seconds required to run the simulation using 4 decimal places (hint: mydifftime function)
	12. Free memory allocated for pthread array
	13. Call pthread terminate function (hint: pthread_exit function)
	14. exit simulation program

	---------------------------------------------------------------------
	Example input and output of Monte carlo simulator to estimate PI
	---------------------------------------------------------------------

		# ./hw5 300 100		// i.e. run simulatior using 300 threads and 100 iterations

	  	monte carlo value of PI = 3.141200
	  	value of count = 23559
	  	time in seconds = 0.0761

	*/


	return 0;

} // end main function
