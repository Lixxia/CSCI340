#include "dpsim.h"


static const unsigned int NUM_PHILOSPHERS = 5;
static const unsigned int NUM_CHOPSTICKS = 5;

static int chopsticks[5];
static pthread_mutex_t mutex[5];
static pthread_t philosophers[5];

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
        // check if all philosophers have 1 chopstick = deadlock
        if(chopsticks[0]==0 && chopsticks[1]==1 && chopsticks[2]==2 && chopsticks[3]==3 && chopsticks[4]==4) {
            printf("Deadlock condition (0,1,2,3,4) - Terminating.");
            break;
        }

        else {
            //see who is eating
            printf("Philosopher(s) ");
            for(k=0; k < NUM_PHILOSPHERS; k++) {
                // printf("\nphil = %i, chopsticks = %i",k,chopsticks[k]);
                if(k==4) {
                    //special case
                    if(chopsticks[0]==k && chopsticks[4]==k){
                        printf("4 ");
                    }
                }
                else if(chopsticks[k] == k && chopsticks[k+1] ==k) {
                    printf("%i, ",k);
                }
            }
            printf("are eating.\n");
            delay(1000000000);
        }

    }

    //kill all philosophers muahaha
    for(i=0; i< NUM_PHILOSPHERS; i++) {
        pthread_kill(&philosophers[i],9);
    }
    exit(0);
} // end th_main function

void* th_phil( void* th_phil_args ) {
    int phil_id = (int)th_phil_args;
    // printf("\nPHIL ID %i",phil_id);
    while(1) {
        delay(3000000000); //philosophizizing 
        eat(phil_id);
    }

} // end th_phil function

void delay( long nanosec ) {

    struct timespec t_spec;

    t_spec.tv_sec = 0;
    t_spec.tv_nsec = nanosec;

    nanosleep( &t_spec, NULL );

} // end think function

void eat( int phil_id ) {
    // printf("\n in eat");
    // printf("\n %i", phil_id); 

    // pick up right chopstick
    // pthread_mutex_lock(&mutex[phil_id]);
    chopsticks[phil_id] = phil_id;

    delay(15000);

    // pick up left chopstick
    if(phil_id==4) {
        pthread_mutex_lock(&mutex[0]);
        chopsticks[0] = phil_id;
    }
    else{
        pthread_mutex_lock(&mutex[phil_id+1]);
        chopsticks[phil_id+1] = phil_id;
    }

    //wait
    delay(20000);

    //done, return chopsticks in reverse order
    if(phil_id==4) {
        pthread_mutex_unlock(&mutex[0]);
    }
    else {
        pthread_mutex_unlock(&mutex[phil_id+1]);
    }
    pthread_mutex_unlock(&mutex[phil_id]);
} // end eat function