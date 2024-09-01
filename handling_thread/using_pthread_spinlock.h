#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <cstdarg>
#define NUM_THREADS 100


class using_spinlock{
    public:
    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    int cnt_thread;
   

    pthread_spinlock_t lock;
    int numThreads;

    using_spinlock* returning_pthread(){
        return this;
    }

    using_spinlock(){
        cnt_thread=0;
        pthread_spin_init(&lock,PTHREAD_PROCESS_PRIVATE);
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(
            &attr,PTHREAD_CREATE_JOINABLE
        );

    }
    ~using_spinlock(){
        for (int i = 1; i < cnt_thread; i++)
        {
            pthread_join(thread[i],0);
        }
        pthread_attr_destroy(&attr);
        pthread_spin_destroy(&lock);
    }

    void Start_spinlock(){
        pthread_spin_lock(&lock);
    }

    void Finish_spinlock(){
        pthread_spin_unlock(&lock);
    }

};
