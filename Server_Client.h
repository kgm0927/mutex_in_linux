#pragma

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <string>

#define BUFSIZE 512

using namespace std;

typedef struct other_sock {
    int clientsock;
    sockaddr_in clientaddr;
    int addrlen;
    char buf[BUFSIZE+1];
    
};


class Server{
    int listen_sock; // 서버의 소켓
    sockaddr_in serveraddr;
    other_sock other;
    
    int retval;
    int addrlen;

    void listening(){
        listen_sock=socket(AF_INET,SOCK_STREAM,0);
        if(listen_sock<0){
            cout<<"소켓 생성 실패"<<endl;
            exit(1);
        }
    }
    void binding(){
        serveraddr.sin_family=AF_INET;
        serveraddr.sin_port=htons(9000);
        serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
        retval=bind(listen_sock,(sockaddr*)&serveraddr,sizeof(serveraddr));
        if(retval<0){
            cout<<"바인딩 실패"<<endl;
            exit(1);
        }
    }

    void listening(){
        retval=listen(listen_sock,SOMAXCONN);
        if(retval<0){
            cout<<"리스닝 실패"<<endl;
            exit(1);
        }
    }

    public:

    void initializeClientAddr(){
        this->addrlen=sizeof(other.clientaddr);
    }

    void Accepting(){
        other.clientsock=accept(listen_sock,(sockaddr*)&other.clientaddr,(socklen_t*)&other.addrlen); // 오류 가능성 있음
        if(other.clientsock<0){
            cout<<"클라이언트 수락 실패"<<endl;
            exit(1);
        }
    }

    void Receiving(){
        retval=recv(other.clientsock,other.buf,BUFSIZE,0);
        if(retval<0){
            cout<<"데이터 수신 실패"<<endl;
        }
    }

    ~Server(){
           close(listen_sock);
        if (other.clientsock > 0) {
            close(other.clientsock);
        }
    }
};


class Client{
     int listen_sock; // 서버의 소켓
    sockaddr_in serveraddr;
    other_sock other;
    
    int retval;
    int addrlen;

    char buf[BUFSIZE+1];
    int len;
  
    void Listening(){
        listen_sock=socket(AF_INET,SOCK_STREAM,0);
        if(listen_sock<0){
            perror("소켓 생성 실패");
        }
    }

   void connecting(){
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(9000);
    serveraddr.sin_addr.s_addr=inet_addr("");
    retval=connect(listen_sock,(sockaddr*)&serveraddr,sizeof(serveraddr));
    if(retval<0){
        perror("에러");
    }
   }

   public:
   // 데이터 입력
   void Typing(){
    printf("텍스트 입력");
    

   }
};