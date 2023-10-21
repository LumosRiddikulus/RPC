#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

#include <sys/stat.h>
#include <fcntl.h>

#define N 128
#define ERR_LOG(errmsg) do{\
                            perror(errmsg);\
                            exit(1);\
                        }while(0)

void handler(int sig)
{
    wait(NULL);

}


int main(int argc,char const *argv[])
{
    if(argc < 3)
    {
        fprintf(stderr,"Usage: %s <ip> <port>",argv[0]);
        exit(1);
    }

    int sockfd;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        ERR_LOG("fail to socket");
    }


    //将套接字设置为允许重复使用本机地址或设置为端口复用（不设置也行）
    int on = 1;
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) < 0)
    {
        ERR_LOG("fail to setsockopt");
    }



    struct sockaddr_in serveraddr;
    socklen_t addrlen = sizeof(serveraddr);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));

    if(bind(sockfd,(struct sockaddr*)&serveraddr,addrlen) == -1)
    {
        ERR_LOG("fail to bind");
    }

    if(listen(sockfd, 5) == -1)
    {
        perror("fail to listen");
    }

    signal(SIGCHLD,handler);        //使用信号，异步的方式处理僵尸进程

    int acceptfd;
    struct sockaddr_in clientaddr;

    while(1)
    {

        if((acceptfd = accept(sockfd,(struct sockaddr*)&clientaddr,&addrlen)) < 0)
        {
            ERR_LOG("fail to accept");
        }

        printf("%s -- %d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

        pid_t pid;

        if((pid = fork()) < 0)
        {
            ERR_LOG("fail to fork");
        }
        else if(pid > 0)    //父进程负责执行accept
        {

        }
        else    //子进程
        {
            char buf[N] = "";
            ssize_t bytes = 0;
            while(1)
            {
                if((bytes = recv(acceptfd,buf,sizeof(buf),0)) < 0)
                {
                    ERR_LOG("fail to recv");
                }
                if(bytes == 0)
                {
                    printf("The client quited\n");
                    exit(0);
                }

                strcat(buf," *_* ");

                if(send(acceptfd,buf,sizeof(buf),0) == -1)
                {
                    ERR_LOG("fail to send");
                }

            }
        }

    }


    return 0;
}