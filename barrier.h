#ifndef BARRIER_H
#define BARRIER_H


#include <semaphore.h>
#include <iostream>
#include <fcntl.h>

namespace synchronization
{

/**
 * @class barrier barrier.h
 * @brief Adds a reusable barrier implemented with POSIX semaphores.
 *
 * The core idea is having two barriers. One for waiting until every other thread finishes it's task, the other barrier
 * is used to ensure the first barrier is completely reset before releasing all the threads again.
 *
 * @author Vincent Phan and Justin Pham
 */
    // Provides a reusable barrier
    class barrier {
    private:
        // Declare your variables here
        int count = 0;
        int N = 0;
        sem_t mutex;
        sem_t gate1;
        sem_t gate2;

    public:
        /**
         * @brief Create the semaphores and assign the total number of threads to 'N'
         * @param numberOfThreads
         */

        // Constructor
        barrier( int numberOfThreads ){
            N = numberOfThreads;
            if (sem_init(&mutex, 0, 1) == -1 || sem_init(&gate1, 0, 0) == -1 || sem_init(&gate2, 0, 1) == -1){
                std::cerr << "Could not create semaphores.";
                exit(1);
            }
        }


        // Destructor
        ~barrier( ){
            sem_destroy(&mutex);
            sem_destroy(&gate1);
            sem_destroy(&gate2);
        }

        /**
         * @brief Thread arrives and barriers and waits. The last thread to arrive opens the barrier for all the threads to pass.
         */

        // Function to wait at the barrier until all threads have reached the barrier
        void arriveAndWait( void ){
            // Makes sure count gets updated safely
            sem_wait(&mutex);
            count++;
            sem_post(&mutex);

            // If not the last thread, then wait
            if (count < N) {
                sem_wait(&gate1);
            } else {
                // If the last thread, release all waiting threads
                sem_wait(&gate2);   //Before freeing the rest, lock next gate
                for (int i = 0; i < N - 1; ++i) {
                    sem_post(&gate1);
                }
            }

            // Makes sure count gets updated safely and is able to be reused
            sem_wait(&mutex);
            count--;
            if (count == 0) {
                sem_post(&gate2);
            }
            sem_post(&mutex);

            //Wait until count is completely reset before continuing to next layer.
            sem_wait(&gate2);
            sem_post(&gate2);
        }
    };
}

#endif
