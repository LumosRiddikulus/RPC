#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/stat.h>

#define N 1024
#define ERROR_LOG(errmsg) do{\
                                perror(errmsg);\
                                printf("[%s - %s - %d]\n",__FILE__,__func__,__LINE__);\
                                exit(1);\
                            }while(0)



void *pthread_fun(void *arg)
{
    int acceptfd = *(int *)arg;
    char buf[N] = "";
    char head[] = "HTTP/1.1 200 OK\r\n"
                    "Content_Type: text/html\r\n"
                    "\r\n";
    char err[] = "HTTP/1.1 404 Not Found\r\n"
                "Content_Type: text/html\r\n"
                "\r\n"
                "<HTML><BODY>File not found</BODY></HTML>";

    if(recv(acceptfd,buf,N,0) < 0)
    {
        ERROR_LOG("fail to recv");
    }

    printf("*********************************\n\n");
    printf("%s\n",buf);

    printf("\n*********************************\n");


    //通过获取的数据包中得到浏览器要访问的网页文件名
    //GET /about.html http/1.1

    char filename[128] = "";
    sscanf(buf,"GET /%s",filename);

    if(strncmp(filename,"HTTP/1.1",strlen("http/1.1")) == 0)
    {
        strcpy(filename,"about.html");
    }

    printf("filename = %s\n",filename);

    char path[128] = "/media/lumos/Document/grpc/TCP/HTTP/";
    strcat(path,filename);

    //通过解析出的网页文件名，查找本地中有没有这个文件
    int fd;
    if((fd =  open(path,O_RDONLY)) < 0)
    {
        if(errno == ENOENT)
        {
            if(send(acceptfd,err,strlen(err),0) < 0)
            {
                ERROR_LOG("fail to send");
            }

            close(acceptfd);
            pthread_exit(NULL);
        }
        else
        {
            ERROR_LOG("fail to open");
        }
    }


    //如果文件存在，先发送指令告知浏览器
    if(send(acceptfd,head,strlen(head),0) < 0)
    {
        ERROR_LOG("fail to send");
    }


    //读取网页中的内容并发送给浏览器
    ssize_t bytes;
    char text[1024] = "";

    while((bytes = read(fd,text,1024)) > 0)
    {
        if(send(acceptfd,text,bytes,0) < 0)
        {
            ERROR_LOG("fail to send");

        }
    }

    pthread_exit(NULL);

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
        ERROR_LOG("fail to socket");

    }

    int on = 1;
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) < 0)
    {
        ERROR_LOG("fail to setsockopt");
    }

    struct sockaddr_in serveraddr;
    int acceptfd;
    struct sockaddr_in clientaddr;
    socklen_t addrlen = sizeof(serveraddr);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));

    if(bind(sockfd,(struct sockaddr*)&serveraddr,addrlen) < 0)
    {
        ERROR_LOG("fail to bind");
    }

    if(listen(sockfd,5) < 0)
    {
        ERROR_LOG("fail to listen");
    }

    while(1)
    {
        if((acceptfd = accept(sockfd,(struct sockaddr*)&clientaddr,&addrlen)) < 0)
        {
            ERROR_LOG("fail to accept");
        }

        printf("%s -- %d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

        pthread_t thread;
        if(pthread_create(&thread,NULL,pthread_fun,&acceptfd) != 0)
        {
            ERROR_LOG("fail to pthread_create");
        }
        pthread_detach(thread);
    }





    return 0;
}