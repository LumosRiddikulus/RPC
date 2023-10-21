#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>


#include <pthread.h>

#define N 128
#define ERR_PRINT(errmsg) do{\
                            perror(errmsg);\
                            exit(1);\
                            }while(0)

typedef struct
{
    struct sockaddr_in addr;
    int acceptfd;

}MSG;

void *pthread_fun(void *arg)
{
    char buf[N] = "";
    ssize_t bytes;
    MSG msg = *(MSG * )arg;

    while(1)
    {
        if((bytes = recv(msg.acceptfd,buf,N,0)) < 0)
        {
            ERR_PRINT("fail to recv");
        }
        else if(bytes == 0)
        {
            printf("The client quited\n");
            pthread_exit(NULL);
        }

        if(strncmp(buf,"quit",4) == 0)
        {
            printf("The client quited\n");
            pthread_exit(NULL);
        }

        printf("[%s -- %d]: %s\n",inet_ntoa(msg.addr.sin_addr),ntohs(msg.addr.sin_port),buf);

        strcat(buf," ^_^ ");
        if(send(msg.acceptfd,buf,N,0) < 0)
        {
            ERR_PRINT("fail to send");
        }

    }

}


int main(int argc, char const *argv[])
{
    if(argc < 3)
    {
        fprintf(stderr,"Usage: %s <ip> <port>",argv[0]);
        exit(1);
    }

    int sockfd;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        ERR_PRINT("fail to socket");
    }

    int on = 1;
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) < 0)
    {
        ERR_PRINT("fail to setsockopt");
    }

    struct sockaddr_in serveraddr;
    socklen_t addrlen = sizeof(serveraddr);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));

    if((bind(sockfd,(struct sockaddr*)&serveraddr,addrlen)) < 0)
    {
        ERR_PRINT("fail to bind");
    }

    if(listen(sockfd,5) < 0)
    {
        ERR_PRINT("fail to listen");
    }

    struct sockaddr_in clientaddr;
    int acceptfd;

    while(1)
    {
        if((acceptfd = accept(sockfd,(struct sockaddr*)&clientaddr,&addrlen)) < 0)
        {
            ERR_PRINT("fail to accept");
        }

        MSG msg;
        msg.addr = clientaddr;
        msg.acceptfd = acceptfd;
        pthread_t thread;

        if((pthread_create(&thread,NULL,pthread_fun,&msg)) != 0)
        {
            ERR_PRINT("fail to pthread_create");
        }


    }


    return 0;
}