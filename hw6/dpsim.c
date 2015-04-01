#include "dpsim.h"


static const unsigned int NUM_PHILOSPHERS = 5;
static const unsigned int NUM_CHOPSTICKS = 5;

static int chopsticks[5];
static pthread_mutex_t mutex[5];
static pthread_t philosophers[5];

/**************************************************
 
 Main Thread Function:
 void* th_main( void* th_main_args )
 
 Note: NO mutex locks or unlocks should be performed by main 
 thread, however the chopsticks array is being read by the 
 main_thread, and is vulnerable to race conditions. You must 
 consider the effect of these race conditions, and the 
 correctness of the check/comparsions and output of the 
 main_thread. 
 
 ------------------------------------------------
 In this function perform the following steps:
 ------------------------------------------------
 1. Initialize all element values in the chopsticks array to -1
 2. Create a thread for each philosopher (hint: use philosophers[5] array)
 - If a error condition occurs, then immediately exit this function with status value 1.
 3. Execute a infinite loop that does the following:
 If a deadlock condition is found then 
 display deadlock
 terminate the loop and goto step 4
 otherwise 
 display which philosophers are eating
 and then call the delay function (you specify nanosec sleep value)
 
 An portion of example output:
 Philopsher(s) 0, 2 are eating
 Philospher(s) 1 are eating
 .
 .
 .
 Deadlock condition (0,1,2,3,4) ... terminating
 
 4. Kill each philosopher thread
 5. Exit the main thread with status value equal to 0.
 
 */
void* th_main( void* th_main_args ) {
    int i,j,k,loop;
    loop=TRUE;
    for(i=0; i<NUM_CHOPSTICKS; i++) {
        chopsticks[i] = -1;
    }

    for(j=0; j<NUM_PHILOSPHERS; j++) {
        if(pthread_create(&philosophers[j], NULL, th_phil, (void *)j)) {
             perror("Error in create.");
             exit(1);
        }
    }

    while(loop) {
        //check for deadlock
        if(chopsticks[0]==0 && chopsticks[1]==1 && chopsticks[2]==2 && chopsticks[3]==3 && chopsticks[4]==4) {
            //display deadlock
            printf("DEADLOCK");
            loop=-1;
        }
        else {
            //see who is eating
            printf("Philosopher(s)");
            for(k=0; k < NUM_PHILOSPHERS; k++) {
                printf("\nchopsticks = %i",chopsticks[k]);
                if(k==4) {
                    //special case
                    if(chopsticks[0]==k && chopsticks[4]==k){
                        printf("4");
                    }
                }
                else if(chopsticks[k] == k && chopsticks[k+1] ==k) {
                    printf("%i, ",k);
                }

            }
            printf("are eating\n");
            delay(1000000000);
        }

    }
    //kill all philosophers
    for(i=0; i< NUM_PHILOSPHERS; i++) {
        pthread_kill(&philosophers[i],9);
        exit(0);
    }
} // end th_main function

/**************************************************
 
 Philosopher Thread Function: 
 void* th_phil( void* th_phil_args );
 
 ------------------------------------------------
 In this function perform the following steps:
 ------------------------------------------------
 1. Get the philosopher id (hint: use th_phil_args)
 2. Execute an infinite loop that does the following:
 - call the delay function for thinking (you specify nanosec sleep value)
 - call the eat function (argument is the philosopher id)
 
 */
void* th_phil( void* th_phil_args ) {
    int phil_id = (int)th_phil_args;
    printf("\nPHIL ID %i",phil_id);
    while(1) {
        delay(1000000000); //thinking
        eat(phil_id);
    }

} // end th_phil function

void delay( long nanosec ) {

    struct timespec t_spec;

    t_spec.tv_sec = 0;
    t_spec.tv_nsec = nanosec;

    nanosleep( &t_spec, NULL );

} // end think function

/**************************************************
 
 Eat Function: 
 void eat( int phil_id )
 
 ------------------------------------------------
 In this function perform the following steps:
 ------------------------------------------------
 This philosopher, phil_id, picks up chopstick phil_id
 (i.e. the right chopstick) then delays for no more than 
 20,000 nanoseconds. Next the philosopher picks up the 
 left chopstick. Note: mutual exclusion is necessary when 
 updating a shared resource. After having picked up both 
 chopsticks (as described) the philosopher will delay a 
 number of nanoseconds that is determined by you experimentally.
 After the delay completes, the philosopher will release 
 the two chopsticks in the reverse order in which they were
 picked up.
 
 */
void eat( int phil_id ) {
    int ration, i,j;
    printf("\n in eat");
    printf("\n phil id = %i", phil_id);

    pthread_mutex_lock(mutex);
    //pick up left chopstick
    if(phil_id==4) {
        chopsticks[0] = phil_id;
    }
    else{
        chopsticks[phil_id+1] = phil_id;
    }
    delay(20000);

    pthread_mutex_lock(mutex);
    //pick up right chopstick
    chopsticks[phil_id] = phil_id;
   
 
    //wait
    delay(1000000000);
 
    //done, return chopsticks
    for (j = 0; j < 2; j++) {
        pthread_mutex_unlock(mutex);
    }

} // end eat function