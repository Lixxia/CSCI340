#include "mem.h"

/* minimum and maximum duration of use for an allocated block of memory */
#define MIN_DURATION      3
#define MAX_DURATION     25

/* minimum and maximum allocation request size */
#define MIN_REQUEST_SIZE    3
#define MAX_REQUEST_SIZE  100

int main(int argc, char** argv) {
  // FIRST, NEXT, BEST
  int r, t, size, duration, units, runs, seed, times;
  int f_probes, f_frags, f_avg_probes, f_avg_failures, f_avg_frags, f_total_probes, f_total_failures, f_total_frags;
  int n_probes, n_frags, n_avg_probes, n_avg_failures, n_avg_frags, n_total_probes, n_total_failures, n_total_frags;
  int b_probes, b_frags, b_avg_probes, b_avg_failures, b_avg_frags, b_total_probes, b_total_failures, b_total_frags;
  units = (int) strtol(argv[1],0,10);
  seed = (int) strtol(argv[4],0,10);
  mem_init(units);

  srand(seed);

  runs = (int) strtol(argv[2],0,10);
  times = (int) strtol(argv[3],0,10);
  //Initialize all the things
  f_probes = 0;
  f_frags = 0;
  f_avg_probes = 0;
  f_avg_failures = 0;
  f_total_probes = 0;
  f_total_failures = 0;
  f_total_frags = 0;
  n_probes = 0;
  n_frags = 0;
  n_avg_probes = 0;
  n_avg_failures = 0;
  n_total_probes = 0;
  n_total_failures = 0;
  n_total_frags = 0;
  b_probes = 0;
  b_frags = 0;
  b_avg_probes = 0;
  b_avg_failures = 0;
  b_total_probes = 0;
  b_total_failures = 0;
  b_total_frags = 0;

  for(r=0;r<runs;r++) {
    // FIRST
    for(t=0;t<times;t++) {
      size = rand() % MAX_REQUEST_SIZE + MIN_REQUEST_SIZE;
      duration = rand() % MAX_DURATION + MIN_DURATION;
      f_probes = mem_allocate(FIRST,size,duration);
      if(f_probes != -1) {
        //success, get avg
        f_total_probes+=f_probes;
      }
      else if(f_probes == -1) {
        f_total_failures++;
      }
      f_frags = mem_fragment_count(MIN_REQUEST_SIZE-1);
      f_total_frags+=f_frags;

      mem_single_time_unit_transpired();
    }
    mem_clear();

    // NEXT
    for(t=0;t<times;t++) {
      size = rand() % MAX_REQUEST_SIZE + MIN_REQUEST_SIZE;
      duration = rand() % MAX_DURATION + MIN_DURATION;
      n_probes = mem_allocate(NEXT,size,duration);
      // printf("\nprobes=%i",n_probes);
      if(n_probes != -1) {
        //success, get avg
        n_total_probes+=n_probes;
      }
      else if(n_probes == -1) {
        n_total_failures++;
      }
      n_frags = mem_fragment_count(MIN_REQUEST_SIZE-1);
      n_total_frags+=n_frags;

      mem_single_time_unit_transpired();
    }
    mem_clear();

    // BEST
    for(t=0;t<times;t++) {
      size = rand() % MAX_REQUEST_SIZE + MIN_REQUEST_SIZE;
      duration = rand() % MAX_DURATION + MIN_DURATION;
      b_probes = mem_allocate(BEST,size,duration);
      // printf("\nprobes=%i",b_probes);
      if(b_probes != -1) {
        //success, get avg
        b_total_probes+=b_probes;
      }
      else if(b_probes == -1) {
        b_total_failures++;
      }
      b_frags = mem_fragment_count(MIN_REQUEST_SIZE-1);
      b_total_frags+=b_frags;

      mem_single_time_unit_transpired();
    }
    mem_clear();
  }

  f_avg_frags = f_total_frags/runs;
  f_avg_probes = f_total_probes/runs;
  f_avg_failures = f_total_failures/runs;
  printf("\n=====FIRST=====");
  printf("\nAverage Probes: %i",f_avg_probes);
  printf("\nAverage Failures %i",f_avg_failures);
  printf("\nAverage Frags: %i",f_avg_frags);

  n_avg_frags = n_total_frags/runs;
  n_avg_probes = n_total_probes/runs;
  n_avg_failures = n_total_failures/runs;
  printf("\n=====NEXT=====");
  printf("\nAverage Probes: %i",n_avg_probes);
  printf("\nAverage Failures %i",n_avg_failures);
  printf("\nAverage Frags: %i",n_avg_frags);

  b_avg_frags = b_total_frags/runs;
  b_avg_probes = b_total_probes/runs;
  b_avg_failures = b_total_failures/runs;
  printf("\n=====BEST=====");
  printf("\nAverage Probes: %i",b_avg_probes);
  printf("\nAverage Failures %i",b_avg_failures);
  printf("\nAverage Frags: %i",b_avg_frags);

  mem_free();
  return 0;
}
