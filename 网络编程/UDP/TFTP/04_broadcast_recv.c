#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    if(argc < 3)
    {
        fprintf(stderr,"Usage: %s <ip> <port>",argv[0]);
        exit(1);
    }

    int sockfd;
    struct sockaddr_in recvaddr;

    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        perror("fail to socket");
        exit(1);
    }

    recvaddr.sin_family = AF_INET;
    recvaddr.sin_addr.s_addr = inet_addr(argv[1]);
    recvaddr.sin_port = htons(atoi(argv[2]));
    socklen_t addrlen = sizeof(recvaddr);

    if(bind(sockfd,(struct sockaddr*)&recvaddr,addrlen) < 0)
    {
        perror("fail to bind");
        exit(1);
    }

    char text[32] = "";
    struct sockaddr_in sendaddr;

    while(1)
    {
        if(recvfrom(sockfd,text,sizeof(text),0,(struct sockaddr *)&sendaddr,&addrlen) == -1)
        {
            perror("fail to recvfrom");
            exit(1);
        }

        printf("[%s - %d]: %s\n",inet_ntoa(sendaddr.sin_addr),ntohs(sendaddr.sin_port),text);

    }


    return 0;
}