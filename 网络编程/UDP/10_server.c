#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
    if(argc < 3)
    {
        fprintf(stderr,"Usage: %s <ip> <port>",argv[0]);
        exit(1);
    }


    int sockfd;
    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        perror("fail in socket");
        exit(1);
    }

    struct sockaddr_in serveraddr;
    in_addr_t addrlen = sizeof(serveraddr);
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(argv[2]));

    if(bind(sockfd,(struct sockaddr *)&serveraddr,addrlen) < 0)
    {
        perror("fail to bind");
        exit(1);
    }

    while(1)
    {
        char text[32] = "";
        struct sockaddr_in clientaddr;

        if(recvfrom(sockfd,text,sizeof(text),0,(struct sockaddr *)&clientaddr,&addrlen) == -1)
        {
            perror("fail to recvfrom");
            exit(1);
        }

        printf("[%s - %d]: %s\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port),text);
        strcat(text," *_* ");

        if(sendto(sockfd,text,sizeof(text),0,(struct sockaddr*)&clientaddr,addrlen) == -1)
        {
            perror("fail to sendto");
            exit(1);
        }

    }

    close(sockfd);




    return 0;
}