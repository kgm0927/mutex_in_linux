#include <iostream>
#include <pthread.h>
#include "using_pthread_semaphor.h"

using namespace std;

#define END 1000

int num = 0;

typedef struct mutex_and_int{
    int i;
} ;

int breaking(int* i) {
    return (*i > END) ? 1 : 0;
}

void* func10(void* um) {
    mutex_and_int* mai = (mutex_and_int*)&um;
    mai->i += 10;
    cout << "func10 - mai->i: " << mai->i << endl;
    return breaking(&mai->i) ? nullptr : nullptr;
}

void* func5(void* um) {
    mutex_and_int* mai = (mutex_and_int*)&um;
    mai->i += 5;
    cout << "func5 - mai->i: " << mai->i << endl;
    return breaking(&mai->i) ? nullptr : nullptr;
}

void* func1(void* um) {
    mutex_and_int* mai = (mutex_and_int*)&um;
    mai->i += 1;
    cout << "func1 - mai->i: " << mai->i << endl;
    return breaking(&mai->i) ? nullptr : nullptr;
}

int main() {
    mutex_and_int i;
    i.i = 0;

    // 세 개의 스레드 함수를 사용하도록 설정
    using_semaphore_func usf1(func10, (void*)&i);
    using_semaphore_func usf2(func5, (void*)&i);
    using_semaphore_func usf3(func1, (void*)&i);

    // 각 스레드 실행
    usf1.Start_thread();
    usf2.Start_thread();
    usf3.Start_thread();

    pthread_exit(NULL);  // 메인 스레드가 종료되지 않도록 유지

    return 0;
}