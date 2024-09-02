#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <random>

#define SEM_N 100

using namespace std;

typedef struct set_sema_and_thread{
    pthread_t pth;
    sem_t sema;
    void* (*func)(void*);
    void* structure;
    
};


class busy_semaphore
{
private:
    set_sema_and_thread sst[SEM_N];

    pthread_mutex_t critical_section;
    int amount_of_sema;

void setting_First_sema_th(pthread_t pth,sem_t sema,void* (*func)(void*),void* func_para){
    if(amount_of_sema==0)
   { 
    this->sst[amount_of_sema].pth=pth;
    this->sst[amount_of_sema].sema=sema;
    this->sst[amount_of_sema].func=func;
    this->sst[amount_of_sema].structure=func_para;
    sem_init(&this->sst[amount_of_sema].sema,0,SEM_N);
    amount_of_sema++;
    }

    else{
        cout<<"첫 번째 스레드가 이미 있음"<<endl;

        Setting_rest_sema_th(pth,sema,func,func_para);


    }
}
    
public:

    pthread_mutex_t* return_mutex(){
        return &critical_section;
    }

    pthread_t& return_pth(int index){
        return this->sst[index].pth;
    }

    sem_t& return_sema(int index){
        return this->sst[index].sema;
    }

    void* return_structure(int index){
        return &this->sst[index].structure;
    }

 void* (*(&return_func(int index)))(void*) {
    // static으로 선언된 함수 포인터 배열
    return this->sst[index].func;
}
    

    busy_semaphore* return_self(){
        return this;
    }


    busy_semaphore(){
        amount_of_sema=0;
        pthread_mutex_init(&critical_section,NULL);
    }

    busy_semaphore(pthread_t pth,sem_t sema,void* (*func)(void*),void* func_para){
        amount_of_sema=0;
        setting_First_sema_th(pth, sema,func,func_para);
        pthread_mutex_init(&critical_section,NULL);
}


    ~busy_semaphore(){

    }

void Setting_rest_sema_th(pthread_t pth,sem_t sema,void* (*func)(void*),void* func_para){

    if(amount_of_sema!=0){
    
    this->sst[amount_of_sema].pth=pth;
    this->sst[amount_of_sema].sema=sema;
    this->sst[amount_of_sema].func=func;
    sem_init(&this->sst[amount_of_sema].sema,0,SEM_N);
    amount_of_sema++;

    }else{
        cout<<"스레드가 채워져 있지 않음"<<endl;
        setting_First_sema_th(pth,sema,func,func_para);
    }
}

    
};

class using_semaphore_func:public busy_semaphore{


int start_thread_index;

void randoming(){
    std::random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<int> dis(0,100);

    start_thread_index=dis(gen);
    
}


static void* using_thread(void* i){
sem_wait(&return_sema(start_thread_index));


pthread_mutex_lock(return_mutex());

void *(*func)(void*)=return_func(start_thread_index);
func(return_structure(start_thread_index));

pthread_mutex_unlock(return_mutex());

randoming();
sem_post(&return_sema(start_thread_index));
}


public:

    using_semaphore_func(){

    }

    void Start_thread(){
        pthread_create(&return_pth(start_thread_index),NULL,using_thread,NULL);
    }



    };