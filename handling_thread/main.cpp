#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include "using_pthread_semaphor.h"

using namespace std;

#define END 1000

int num=0;

typedef struct mutex_and_int{
    int i;
};

int breaking(int* i){
    if(*i>1000){
        return 1;
    }
    return 0;
}

void* func10(void* um){
    mutex_and_int* mai=(mutex_and_int*)um;
    mai->i+=10;

     if(breaking(&mai->i)){
        return nullptr;
    }


    return nullptr;
}

void* func5(void* um){
    mutex_and_int* mai=(mutex_and_int*)um;
    mai->i+=5;

    if(breaking(&mai->i)){
                return nullptr;
;
    }

    return nullptr;
}

void* func1(void* um){
    mutex_and_int* mai=(mutex_and_int*)um;
    mai->i+=1;

     if(breaking(&mai->i)){
                return nullptr;
;
    }


    return nullptr;
}

int main(){

    mutex_and_int i;
    i.i=0;

  using_semaphore_func usf(func10,(void*)&i);
  


    usf.Start_thread();
    usf.Start_thread();
    usf.Start_thread();
  
}