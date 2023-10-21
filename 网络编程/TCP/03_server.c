#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define N 128

int main(int argc,char const * argv[])
{
    if(argc < 3)
    {
        fprintf(stderr,"Usage: %s <ip> <port>",argv[0]);
        exit(1);
    }


    //step1 创建套接字
    int sockfd;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("fail to socket");
        exit(1);
    }


    //step2 将套接字与服务器网络信息结构体绑定

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));
    socklen_t addrlen = sizeof(serveraddr);

    if(bind(sockfd,(struct sockaddr*)&serveraddr,addrlen) < 0)
    {
        perror("fail to bind");
        exit(1);
    }

    //将套接字设置为被动监听状态
    if(listen(sockfd, 10) == -1)
    {
        perror("fail to listen");
        exit(1);
    }

    //阻塞等待客户端的连接
    int acceptfd;
    struct sockaddr_in clientaddr;            //保存客户端网络信息的结构体
    if((acceptfd = accept(sockfd,(struct sockaddr*)&clientaddr,&addrlen)) == -1)
    {
        perror("fail to accept");
        exit(1);
    }

    //打印链接客户端的信息
    printf("ip: %s\nport: %d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

    //进行通信

    //!!!!!!!!!!!tcp服务器与客户端通信是，需要使用accept函数的返回值(acceptfd)！！！！！！！！！！！！！！！！！
    char buf[N] = "";

    if(recv(acceptfd,buf,sizeof(buf),0) == -1)      //用对方的返回值
    {
        perror("fail to recv");
        exit(1);

    }

    printf("from client: %s\n",buf);
    buf[sizeof(buf) - 1] = '\0';

    //发送给客户端

    strcat(buf,"*_*");
    if(send(acceptfd,buf,sizeof(buf),0) == -1)
    {
        perror("fail to send");
        exit(1);
    }

    close(acceptfd);
    close(sockfd);

    return 0;
}