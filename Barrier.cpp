//
// Created by eyall on 1/21/2020.
//

#include "Barrier.h"

Barrier::Barrier(unsigned int num_of_threads) : N(num_of_threads),
                                                waiting_on_barrier(0), sem(),
                                                sem2(),mutex(){
    sem_init(&sem, 0, 0);
    sem_init(&sem2, 0, 1);
    pthread_mutex_init(&mutex, nullptr);
}

void Barrier::wait() {
    pthread_mutex_lock(&mutex);
    waiting_on_barrier++;
    if (waiting_on_barrier==N){
        sem_wait(&sem2); // lock the second
        sem_post(&sem); // unlock the first
    }
    pthread_mutex_unlock(&mutex);
    sem_wait(&sem);
    sem_post(&sem);

    pthread_mutex_lock(&mutex);
    waiting_on_barrier--;
    if (waiting_on_barrier==0){
        sem_wait(&sem); // lock the first
        sem_post(&sem2); // unlock the second
    }
    pthread_mutex_unlock(&mutex);

    sem_wait(&sem2);
    sem_post(&sem2);
}

unsigned int Barrier::waitingThreads() {
    pthread_mutex_lock(&mutex);
    unsigned int ret= waiting_on_barrier;
    pthread_mutex_unlock(&mutex);
    return ret;
}

Barrier::~Barrier() {
    sem_destroy(&sem);
    sem_destroy(&sem2);
    pthread_mutex_destroy(&mutex);
}
