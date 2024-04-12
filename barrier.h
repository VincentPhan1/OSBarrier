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

    public:
        int count = 0;
        int N = 0;
        sem_t mutex;
        sem_t barr;
        //sem_t gate1;
        //sem_t gate2;

        // Constructor
        barrier( int numberOfThreads ){
            int sem1 = sem_init(&mutex, 1, 1);
            int sem2 = sem_init(&barr, 1, 0);
            //int sem2 = sem_init(&gate1, 1, 0);
            //int sem3 = sem_init(&gate2, 1, 1);
            N = numberOfThreads;
        }


        // Destructor
        ~barrier( ){
            sem_destroy(&mutex);
            sem_destroy(&barr);
            //sem_destroy(&gate1);
            //sem_destroy(&gate2);

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
            }

            // if last thread, then signals to release the last thread
            // continually go on until all threads are released
            sem_post(&barr);

            // makes sure count gets updated safely and is able to be reused
            sem_wait(&mutex);
            count--;
            sem_post(&mutex);


            // vincent pseudocode
//            sem_wait(&mutex);
//            count++;
//            // you are the last thread to leave Gate1
//            if (count == N){
//                sem_wait(&gate1);
//                for (int i=0; i<N; i++){
//                    // start the chain of releasing other threads
//                    sem_post(&gate1);
//                }
//            }
//            sem_post(&mutex);
//
//            // based off another example in class
//            sem_wait(&gate1);
//            count--;
//            if (count == 0){
//                sem_wait(&gate1);
//                sem_post(&gate2);
//            }
//            sem_post(&mutex);
//
//            sem_wait(&gate2);
//            sem_post(&gate2);
        }
    };

}

#endif
