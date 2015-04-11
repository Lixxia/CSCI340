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
  int count,i,j,start_block, block;
  i=0;
  count=0;
  start_block=0;
  block=0;

  if(strategy == FIRST) {
    while(i<mem_size) {
      // printf("\nmem[%i]: %i",i,memory[i]);
      if((i!=0 && memory[i]==0 && memory[i-1] != 0) || (i==0 && memory[i] ==0)) {
        //start of block
        start_block=i;
        printf("\nstart block=%i",i);
        block++;
        count=0;
      }
      if(memory[i]==0) {
        count++;
      }
      else if(memory[i] != 0) {
        count=0;
      }
      // printf("\ncount=%i",i);
      if(size <= count) {
        printf("\nDO THE THING @ %i",i);
          while(count !=0) {
            printf("\ncount = %i",count);
            memory[start_block] = duration;
            start_block++;
            count--;
        }
        break;
      }

      // if(memory[i] == 0) {
      //   if(i>0 && memory[i-1] != 0) {
      //     start_block = i;
      //     count=0;
      //   }
      //   else if(i == mem_size-1) {
      //     block++;
      //   }
      //   count++; // num free blocks
      // }
      // // printf("\ncount = %i",count);
      // if(count == mem_size) {
      //   block=1;
      //   // array is fresh, all free
      //   for(start_block; start_block<size; start_block++) {
      //       memory[start_block] = duration;
      //   }
      // }

      // if(memory[i] == 0 && memory[i+1] != 0 || memory[i] == 0 && memory[i-1] != 0) {
      //   printf("\nFree portion= %i",i);
      //   printf("\nCount=%i",count);
      //   printf("\nSize=%i",size);
      //   block++;
      //   if(count <= size ) {
      //     for(start_block; start_block<count; start_block++) {
      //       memory[start_block] = duration;
      //     }
      //   }
      //   count=0;
      // }
      i++;
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
  printf("\nblocks! %i",block);
  return block;
}

/*
  Go through all of memory and decrement all positive-valued entries.
  This simulates one unit of time having transpired.
 */
int mem_single_time_unit_transpired() {
  //don't decrement 0
  int i=0;
  while(i<mem_size) {
    if(memory[i]>0) {
      printf("\nmem[%i]: %i",i,memory[i]);
      memory[i] = memory[i]-1;
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
  int i = 0;
  int num_frags = 0;
  int num_free = 0;
  while(i<mem_size) {
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
    i++;
  }
  return num_frags;
}

/*
  Set the value of zero to all entries of memory.
 */
void mem_clear() {
  int i=0;
  while(i<mem_size) {
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
