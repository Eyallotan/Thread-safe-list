#ifndef BARRIER_H_
#define BARRIER_H_

#include <semaphore.h>
#include <pthread.h>
#include <iostream>


class Barrier {
public:
    Barrier(unsigned int num_of_threads);
    void wait();
    unsigned int waitingThreads();
    ~Barrier();

private:
    unsigned int N;
    unsigned int waiting_on_barrier;
    sem_t sem;
    sem_t sem2;
    pthread_mutex_t mutex;
};


#endif // BARRIER_H_
