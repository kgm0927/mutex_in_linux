#include <iostream>
#include "using_pthread.h"
#include <stdlib.h>
#include <pthread.h>

using namespace std;

#define END 1000

int num=0;

typedef struct mutex_and_int{
    int i;
    using_pthread* um;
};

int breaking(using_mutex& um){
    if(num>=END){
        um.Finish_lock();
        return -1;
    }
    return 0;
}

void* func10(void* um){
    mutex_and_int* t_um=(mutex_and_int*)um;
    for(;;){
       t_um->um.
        if(-1==breaking(*t_um)){
            break;
        }
        num+=10;
    cout<<"10 추가:"<<num<<endl;
    t_um->Finish_lock();
    }
    return nullptr;
}

void* func5(void* um){
    using_mutex* t_um=(using_mutex*)um;
    for(;;){
        t_um->Start_lock();
        if(-1==breaking(*t_um)){
            break;
        }
        num+=5;
        cout<<"5 추가:"<<num<<endl;
        t_um->Finish_lock();

    }
    return nullptr;
}

void* func1(void* um){
    using_mutex* t_um=(using_mutex*)um;
    for(;;){
        t_um->Start_lock();
        if(-1==breaking(*t_um)){
            break;
        }
        num+=1;
        cout<<"1 추가:"<<num<<endl;
        t_um->Finish_lock();
    }
    return nullptr;
}

int main(){
    using_mutex um(10);
    mutex_and_int mai;

    mai.um=um.returning_pthread();


    um.create_threads(func10,(void*)&mai);
    um.create_threads(func5,(void*)&mai);
    um.create_threads(func1,(void*)&mai);



    pthread_join(pth1, NULL);
    pthread_join(pth2, NULL);
    pthread_join(pth3, NULL);

    // um.~using_mutex(); // 생략 가능
    return 0;
}