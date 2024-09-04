#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <random>
#include <cstdlib>
#include <time.h>
#include <unistd.h>
#define SEM_N 100

using namespace std;


// 잠시 보류


typedef struct set_sema_and_thread{
    pthread_t* pth;
    sem_t* sema;
    void* (*func)(void*);
    void* structure;
    
};


class busy_semaphore
{
private:
    set_sema_and_thread sst[SEM_N];
    pthread_mutex_t critical_section;
    int amount_of_sema;

void setting_First_sema_th(void* (*func)(void*),void* func_para){ // 배열 중 첫 번째 스레드를 채워 넣음.
    if(amount_of_sema==0)
   { 
    this->sst[amount_of_sema].pth=new pthread_t;
    this->sst[amount_of_sema].sema=new sem_t;
    this->sst[amount_of_sema].func=func;
    this->sst[amount_of_sema].structure=func_para;
    sem_init(this->sst[amount_of_sema].sema,0,SEM_N);
    amount_of_sema++;
    }

    else{
        cout<<"첫 번째 스레드가 이미 있음"<<endl;

        Setting_rest_sema_th(func,func_para);


    }
}
    
public:

    int amount_of_thread(){
        return amount_of_sema;
    }

    pthread_mutex_t& return_mutex(){ // 뮤텍스를 반환
        return critical_section;
    }

    pthread_t* return_pth(int index){ // 특정 배열 위치의 스레드의 참조를 반환
        return this->sst[index].pth;
    }

    sem_t* return_sema(int index){ // 특정 위치의 세마포어 참조를 반환.
        return this->sst[index].sema;
    }

    void* return_structure(int index){// 함수의 파라미터로 쓰일 포인터를 반환
        return &this->sst[index].structure;
    }

 void* (*(&return_func(int index)))(void*) { // 특정 위치의 함수를 반환 반드시 void* 타입
    // static으로 선언된 함수 포인터 배열
    return this->sst[index].func;
}
    

    busy_semaphore& return_self(){ // 인스턴스 자체를 반환.
        return *this;
    }


    busy_semaphore(){
        amount_of_sema=0;
        pthread_mutex_init(&critical_section,NULL);
    }

    busy_semaphore(void* (*func)(void*),void* func_para){
        amount_of_sema=0;
        setting_First_sema_th(func,func_para);
        pthread_mutex_init(&critical_section,NULL);
}


    ~busy_semaphore(){
        for (int i = 0; i < amount_of_sema-1; i++)
        {
            pthread_join(*this->sst[i].pth,NULL);
            sem_destroy(this->sst[i].sema);

            delete this->sst[i].pth;
            delete this->sst[i].sema;

            this->sst[i].func=NULL;
            this->sst[i].structure=NULL;
        }

        pthread_mutex_destroy(&critical_section);
        
    }

void Setting_rest_sema_th(void* (*func)(void*),void* func_para){

    if(amount_of_sema!=0){
    
    this->sst[amount_of_sema].pth=new pthread_t;
    this->sst[amount_of_sema].sema=new sem_t;
    this->sst[amount_of_sema].func=func;
    this->sst[amount_of_sema].structure=func_para;
    sem_init(this->sst[amount_of_sema].sema,0,SEM_N);
    amount_of_sema++;

    }else{
        cout<<"스레드가 채워져 있지 않음"<<endl;
        setting_First_sema_th(func,func_para);
    }
}

    
};

class using_semaphore_func:public busy_semaphore{


int start_thread_index;

void randoming(){
    std::random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<> dis(0,amount_of_thread()-1);

    start_thread_index=dis(gen);
    
}


 static void* using_thread(void* arg){


auto* instance=static_cast<using_semaphore_func*>(arg);
sem_wait(instance->return_sema(instance->start_thread_index));
void* (*func)(void*) = instance->return_func(instance->start_thread_index);

 while (true) {
            pthread_mutex_lock(&instance->return_mutex());


            func(instance->return_structure(instance->start_thread_index));

            pthread_mutex_unlock(&instance->return_mutex());

 
break;
           
}
  instance->randoming();
            sem_post(instance->return_sema(instance->start_thread_index));
         

return NULL;

}


public:


using_semaphore_func():busy_semaphore(){
    this->start_thread_index=0;
}

using_semaphore_func(void* (*func)(void*),void* func_para)
:busy_semaphore(func,func_para){
        this->start_thread_index=0;

}


    void Start_thread(){
        pthread_create(return_pth( start_thread_index),NULL,using_thread,this);
        this->start_thread_index++;
    }



    };