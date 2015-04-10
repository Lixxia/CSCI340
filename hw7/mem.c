#include <stdlib.h>   // for malloc() and free()
#include "mem.h"

/*
  Physical memory array. This is a static global array for all functions in this file.  
  An element in the array with a value of zero represents a free unit of memory.
*/
static unsigned int* memory;

/*
 The size (i.e. number of units) of the physical memory array. This is a static global 
 variable used by functions in this file.  

 */
static unsigned int mem_size;


/*
 The last_placement_position variable contains the end position of the last 
 allocated unit used by the next fit placement algorithm.  

 at sometime needs to be reset to 0 so do a complete pass over the array
 */
static unsigned int last_placement_position;

/*
  Using the memory placement algorithm, strategy, allocate size
  units of memory that will reside in memory for duration time units.

  If successful, this function returns the number of contiguous blocks
  (a block is a contiguous "chuck" of units) of free memory probed while 
  searching for a suitable block of memory according to the placement 
  strategy specified.  If unsuccessful, return -1.

  If a suitable contiguous block of memory is found, the first size
  units of this block must be set to the value, duration.
 */
int mem_allocate(mem_strategy_t strategy, unsigned int size, unsigned int duration) {
  // FIRST, NEXT, BEST
  // need start count, and how much you've counted until you've reached end of block
  int count,i;
  if(strategy == FIRST) {
    while(memory[i] != NULL) {
      if(memory[i] == 0) {
        count++;
      }
      if(memory[i+1] != 0) {
        // will segfault at end ?
        //compare because have reached end of section
        if(count <= size ) {
          // put thing
        }
        count=0;
      }
    } 
  }
  else if(strategy == NEXT) {
    //last_placement_position

  }
  else if(strategy == BEST) {

  }
  else {
    return -1;
  }

  return count;
}

/*
  Go through all of memory and decrement all positive-valued entries.
  This simulates one unit of time having transpired.
 */
int mem_single_time_unit_transpired() {
  //don't decrement 0
  int i=0;
  while(memory[i] != NULL) {
    if(memory[i]>0) {
      memory[i]--;
    }
    i++;
  }
  return 0;
}

/*
  Return the number of fragments in memory.  A fragment is a
  contiguous free block of memory of size less than or equal to
  frag_size.
 */
int mem_fragment_count(int frag_size) {
  int i;
  int num_frags = 0;
  int num_free = 0;
  int min_allocation_size = 3;
  while(memory[i] != NULL) {
    if(memory[i] == 0) {
      num_free++;
    }
    if(memory[i+1] != 0) {
      //compare because have reached end of section
      if(num_free < frag_size ) {
        num_frags++;
      }
      num_free=0;
    }
  }
  return num_frags;
}

/*
  Set the value of zero to all entries of memory.
 */
void mem_clear() {
  int i=0;
  while(memory[i] != NULL) {
    memory[i]=0;
    i++;
  }
}

/*
 Allocate physical memory to size. This function should 
 only be called once near the beginning of your main function.
 */
void mem_init( unsigned int size )
{
	memory = malloc( sizeof(unsigned int)*size );
	mem_size = size;
}

/*
 Deallocate physical memory. This function should 
 only be called once near the end of your main function.
 */
void mem_free()
{
	free( memory );
}
