#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <cstdarg>
#define NUM_THREADS 100

typedef struct using_pthread{
    pthread_mutex_t cm;
    pthread_cond_t cv;
    
};


class using_mutex{
    public:
    pthread_t thread[NUM_THREADS]; // 제일 첫번째는 1로 시작한다.
    int cnt_thread;
    pthread_attr_t attr;
    

    using_pthread up;
    int numThreads;

   using_pthread* returning_pthread(){
        return&up;
    }


using_mutex(int numThreads){
    cnt_thread=0;
    pthread_mutex_init(&up.cm,NULL);
    pthread_cond_init(&up.cv,NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

}
  // 클래스에 소멸자를 추가하여 리소스 해제
    ~using_mutex() {

        for (int i = 1; i < cnt_thread; i++)
        {
            pthread_join(thread[i],0);
        }
        


        pthread_attr_destroy(&attr);
        pthread_mutex_destroy(&up.cm);
        pthread_cond_destroy(&up.cv);
    }


    void create_threads(void* (*func)(void*),void* whatever){

int result_code;
        cnt_thread++;
        if(cnt_thread<NUM_THREADS){
       result_code=pthread_create(
        &thread[cnt_thread],&attr,func,(void*)&whatever
      );
      }
      else{
        cout<<" 더 이상 스레드를 만들 수 없습니다."<<endl;
      }

         if(result_code!=0){
        std::cerr << "Error creating thread "<< result_code << std::endl;
        return;
        }
    }

/*
    void create_threads(int* para,int size_of_func,void* (*func)(void*) , ...){

        va_list args;
        va_start(args,func);

        for (int i = 0; i < size_of_func; i++){

            int result_code=pthread_create(
            &thread[i],&attr,va_arg(args,void*(*)(void*)),NULL);
        
        if(result_code!=0){
        std::cerr << "Error creating thread "<< result_code << std::endl;
        break;
        }
        }
    }
    */
    void Start_lock(){
        pthread_mutex_lock(&up.cm);
    }

   
     void Finish_lock(){
        pthread_mutex_unlock(&up.cm);
    }

    

    void signaling(){
        pthread_cond_signal(&up.cv);
    }
};