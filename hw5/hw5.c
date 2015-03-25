#include <stdio.h>         // for printf()
#include <pthread.h>       // for pthread_xxx() routines
#include "mcsim.h"

double gcount;    // global counter -- a double to handle large sums (billions+)
long numits;      // global variable for number of iterations (see step 3 below)

int main( int argc, char** argv ) {

	gcount = 0;

	long i, j, num_threads, max_threads;
	double pi;
	struct rlimit *rlim;
	rlim = (struct rlimit *) malloc(sizeof(rlim_t) * 2);
	struct timespec *ts;
	ts = (struct timespec *) malloc(sizeof(time_t) * 2);
	struct timespec *tse;
	tse = (struct timespec *) malloc(sizeof(time_t) * 2);

	num_threads = strtol(argv[1],NULL,10);
	numits = strtol(argv[2],NULL,10);
	getrlimit(RLIMIT_NPROC, rlim);
	max_threads = rlim->rlim_cur;
	
	if(num_threads < max_threads) {
		pthread_t threads[num_threads];

		// get start time
		clock_gettime(CLOCK_REALTIME, ts);
		for(i=0; i<num_threads; i++) {
			if(pthread_create(&threads[i], NULL, th_routine, (void *)i)) {
				 perror("Error in create.");
      			 exit(-1);
			}
		}
		for(j=0; j<num_threads; j++) {
			if(pthread_join(threads[j], NULL)) {
				perror("Error in join.");
				exit(-1);
			}
		}
			pi= (gcount/num_threads)/numits*4;
			printf("\nMonte Carlo value of pi: %f", pi);
			printf("\nValue of count: %.0f",gcount);
			// get end time
			clock_gettime(CLOCK_REALTIME, tse);
			printf("\nTime in seconds: %f", (mydifftime(ts,tse)/1000000000));
			
			pthread_exit(NULL);
	}
	else {
		printf("Too many threads.");
	}

	free(rlim);
	free(ts);
	free(tse);
	return 0;

} // end main function
