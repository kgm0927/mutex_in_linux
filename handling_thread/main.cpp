#include <iostream>
#include "using_pthread.h"
#include <stdlib.h>
#include <pthread.h>

using namespace std;

#define END 1000
int num=0;
int breaking(using_mutex& um){
    if(num>=END){
        um.Finish_lock();
        return -1;
    }
    return 0;
}
void* func10(void* um)
{
    using_mutex* t_um=(using_mutex*)um;
    for(;;){
    t_um->Start_lock();
   if( -1==breaking(*t_um)){
    break;
   }
    num+=10;
cout<<"10 추가:"<<num<<endl;

t_um->Finish_lock();
}
}

void* func5(void * um)
{
    using_mutex* t_um=(using_mutex*)um;

   for(;;){ t_um->Start_lock();
    num+=5;
        if( -1==breaking(*t_um)){
    break;
   }
    cout<<"5 추가:"<<num<<endl;
    
t_um->Finish_lock();}
}

void* func1(void* um){
        using_mutex* t_um=(using_mutex*)um;

    for(;;){
        t_um->Start_lock();
       if( -1==breaking(*t_um)){
    break;
   }
    num+=1;
    cout<<"1 추가:"<<num<<endl;
    t_um->Finish_lock();}
}


int main(){
    using_mutex um(10);
    int start=0;
           
    pthread_t pth1,pth2,pth3;
    

    pthread_attr_init(&um.attr);
    pthread_create(
        &pth1,&um.attr,func10,(void*)&um
    );
    pthread_create(
        &pth2,&um.attr,func5,(void*)&um
    ); 
    pthread_create(
        &pth3,&um.attr,func1,(void*)&um
    );
        

        
            pthread_join(pth1,NULL);
            pthread_join(pth2,NULL);
            pthread_join(pth3,NULL);
        
        
    
    um.~using_mutex();
     
}