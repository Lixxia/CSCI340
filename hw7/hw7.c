#include "mem.h"

/* minimum and maximum duration of use for an allocated block of memory */
#define MIN_DURATION      3
#define MAX_DURATION     25

/* minimum and maximum allocation request size */
#define MIN_REQUEST_SIZE    3
#define MAX_REQUEST_SIZE  100

/*
  The main program will accept four paramemters on the command line.
  The first parameter is the memory size.  The second parameter is the
  number of times to repeat each experiment (ie. number of runs).  The
  third parameter is the duration of the each simulation run. The
  forth parameter is a random number seed. Here is an example command
  line:

  ./hw7 10000 100 2000 1234

  This means that your program should initialize physical memory to
  10,000 units, perform 100 runs with each run taking 2000 units of
  time, and the random number generator should be seeded (one time)
  with the value 1234.
*/

int main(int argc, char** argv) {
  // FIRST, NEXT, BEST
  int r, t, size, duration, units, runs, seed, times;
  units = (int) strtol(argv[1],0,10);
  seed = (int) strtol(argv[4],0,10);
  mem_init(units);

  srand(seed);
  size = rand() % MAX_REQUEST_SIZE + MIN_REQUEST_SIZE;
  duration = rand() % MAX_DURATION + MIN_DURATION;

  runs = (int) strtol(argv[2],0,10);
  times = (int) strtol(argv[3],0,10);
  // printf("runs=%i",runs);
  printf("\nSize=%i",size);
  printf("\nDuration=%i",duration);

  for(r=0;r<runs;r++) {
    // -1 is allocation failure
    // otherwise value is num probes

    // 
    // allocation request, gather statistics, single time unit transpired 
    // for each placement for each run for each time unit
    //mem clear outside 1k loop
    for(t=0;t<times;t++) {
      printf("\nResult of mema, %i",mem_allocate(FIRST,size,duration));
      mem_single_time_unit_transpired();
      
    }
    // mem_clear();

    // for(t=0;t<times;t++) {
    //   mem_allocate(NEXT,size,duration);
    //   mem_single_time_unit_transpired();
    //   // printf("next");
    // }
    // mem_clear();

    // for(t=0;t<times;t++) {
    //   mem_allocate(BEST,size,duration);
    //   mem_single_time_unit_transpired();
    //   // printf("best");
    // }
    // mem_clear();
  }
  

  //initialize physical memory = argv[1]
  //run argv[2] times
  //take argv[3] units of time
  //rng seeded with argv[4]
  mem_fragment_count(MIN_REQUEST_SIZE-1);
  mem_free();
  return 0;
}
