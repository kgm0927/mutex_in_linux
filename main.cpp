#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <mutex>
#include <thread>
#include <pthread.h>

#define PORT 8080
#define NUM_THREADS 100

using namespace std;


struct ThreadData {
    int thread_id;
    pthread_mutex_t* mutex;
};

void* threading(void* arg){
    ThreadData *data=(ThreadData*)arg;

    pthread_mutex_lock(data->mutex);
    printf("Thread ID: %d\n", data->thread_id);
    pthread_mutex_unlock(data->mutex);

    pthread_exit(NULL);
}

int main(){
    pthread_mutex_t count_mutex;
    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    ThreadData thread_data[NUM_THREADS];

    pthread_mutex_init(&count_mutex,NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);


    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_data[i].thread_id=i;
        thread_data[i].mutex=&count_mutex;

        int result_code=pthread_create(&thread[i],&attr,threading,(void*)&thread_data[i]);

        if(result_code!=0){
             fprintf(stderr, "Error creating thread %d: %d\n", i, result_code);
            return 1;
        }

    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(thread[i],NULL);
    }
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&count_mutex);


    return 0;    
}