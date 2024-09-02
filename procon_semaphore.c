#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>


#define N_COUNTER 4
#define MILLI 1000

void mywrite(int n);
int myread();


pthread_mutex_t critical_section; // 뮤텍스
sem_t semWrite, semRead;          // POSIX 세마포
int queue[N_COUNTER];             
int wIdx=0;                       // 생산자 스레드가 queue[]에 저장할 다음 인덱스
int rIdx=0;                       // 소비자 스레드가 queue[]에서 읽을 다음 인덱스

void* producer(void* arg){ // 생산자 스레드 함수
    for (int i = 0; i < 10; i++)
    {
        mywrite(i);

        int m=rand()%10;
        usleep(MILLI*m*10); // m*10 밀리초 동안 잠자기
    }

    return NULL;

}

void* consumer(void* arg){
    for (int i = 0; i < 10; i++)
    {
        int n=myread();

        int m=rand()%10;
        usleep(MILLI*m*10);
    }

    return NULL;
    
}

void mywrite(int n){                           // 정수 n을 queue[]에 삽입
    sem_wait(&semWrite);                        // queue[]에 저장수 있는지 확인하고 있다면 대기
    pthread_mutex_lock(&critical_section);      // 뮤텍스 락 잠그기
    queue[wIdx]=n;                              // 버퍼에 정수 n을 삽입한다.
    wIdx++;
    wIdx%=N_COUNTER;
    pthread_mutex_unlock(&critical_section);    // 뮤텍스 락 열기
    printf("producer : wrote %d\n",n);

    sem_post(&semRead);                         // consumer 스레드 깨우기
}

int myread(){                                   // queue[]의 맨 앞에 있는 정수를 읽어 리턴
    sem_wait(&semRead);                         // queue[]에서 읽을 수 있는지 없는지 확인하고 없다면 대기
    pthread_mutex_lock(&critical_section);      // 뮤텍스 락 잠그기
    int n=queue[rIdx];                          // 버퍼에서 정수를 읽는다.
    rIdx++;
    rIdx%=N_COUNTER;
    pthread_mutex_unlock(&critical_section);    // 락 잠그기
    printf("\tconsumer: read %d\n",n);
    sem_post(&semWrite);
    return n;
}

int main(){
    pthread_t t[2]; // 스레드구조체

    srand(time(NULL)); // 난수 발생 초기화(seed 랜덤 지점)
    pthread_mutex_init(&critical_section,NULL); // 뮤텍스 락 초기화
    int res=sem_init(&semWrite,0,N_COUNTER);
    if(res==-1){
        printf("semaphore is not supported\n");
        return 0;
    }
    sem_init(&semRead,0,0);

    // producer와 consumer 스레드 생성
    pthread_create(&t[0],NULL,producer,NULL);
    pthread_create(&t[1],NULL,consumer,NULL);

    for (int i = 0; i < 2; i++)
    {
        pthread_join(t[i],NULL);
    }
    
    sem_destroy(&semRead);
    sem_destroy(&semWrite);

    pthread_mutex_destroy(&critical_section);
    return 0;
}