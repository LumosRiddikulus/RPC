#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#define N 128


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
        perror("fail to socket");
        exit(1);
    }

    struct sockaddr_in serveraddr;
    socklen_t addrlen = sizeof(serveraddr);
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(argv[2]));


    if(connect(sockfd,(struct sockaddr*)&serveraddr,addrlen) == -1)
    {
        perror("fail to connect");
        exit(1);
    }


    //step3
    //客户端主动，服务端被动

    //send
    while(1)
    {

    char buf[N] = "";
    fgets(buf,N,stdin);
    buf[sizeof(buf) - 1] = '\0';

    if(strcmp(buf,"quit") == 0)
    {
        printf("The client quited\n");
        exit(1);
    }


    if(send(sockfd,buf,N,0) == -1)
    {
        perror("fail to send");
        exit(1);
    }

    char text[N] = "";

    if(recv(sockfd,text,N,0) == -1)
    {
        perror("fail to recv");
        exit(1);
    }

    printf("from server: %s\n",text);
    }

    close(sockfd);


    return 0;
}