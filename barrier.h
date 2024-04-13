#ifndef BARRIER_H
#define BARRIER_H


#include <semaphore.h>
#include <iostream>
#include <fcntl.h>

namespace synchronization
{


    // Provides a reusable barrier
    class barrier {
    private:
        // Declare your variables here
        int count = 0;
        int N = 0;
        sem_t mutex;
        sem_t barr;

    public:
        // Constructor
        barrier( int numberOfThreads ){
            if (sem_init(&mutex, 0, 1) == -1 | sem_init(&barr, 0, 0) == -1){
                std::cerr << "Could not create semaphores.";
                exit(1);
            }
        }


        // Destructor
        ~barrier( ){
            sem_destroy(&mutex);
            sem_destroy(&barr);
        }

        // Function to wait at the barrier until all threads have reached the barrier
        void arriveAndWait( void ){
            // justin pseudocode

            // makes sure count gets updated safely
            sem_wait(&mutex);
            count++;
            sem_post(&mutex);

            // if not the last thread, then wait
            if (count < N){
                sem_wait(&barr);
            } else {
                // If the last thread, release all waiting threads
                for (int i = 0; i < N - 1; ++i) {
                    sem_post(&barr);
                }
            }

            // if last thread, then signals to release the last thread
            // continually go on until all threads are released
            sem_post(&barr);

            // makes sure count gets updated safely and is able to be reused
            sem_wait(&mutex);
            count--;
            sem_post(&mutex);
        }
    };

}

#endif
