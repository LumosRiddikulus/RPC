#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
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
        perror("fail to socket");
        exit(1);
    }

    struct ip_mreq mreq;
    struct sockaddr_in groupcastaddr;
    mreq.imr_multiaddr.s_addr = inet_addr(argv[1]);
    mreq.imr_interface.s_addr = INADDR_ANY;
    if(setsockopt (sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0)
    {
        perror("fail to setsockopt");
        exit(1);
    }

    socklen_t addrlen = sizeof(groupcastaddr);
    groupcastaddr.sin_family = AF_INET;
    groupcastaddr.sin_addr.s_addr = inet_addr(argv[1]);
    groupcastaddr.sin_port = htons(atoi(argv[2]));

    if(bind(sockfd,(struct sockaddr*)&groupcastaddr,addrlen) < 0)
    {
        perror("fail to bind");
        exit(1);

    }


    char text[32] = "";
    struct sockaddr_in sendaddr;
    socklen_t sendlen = sizeof(sendaddr);

    while(1)
    {
        if(recvfrom(sockfd,text,sizeof(text),0,(struct sockaddr*)&sendaddr,&sendlen) < 0)
        {
            perror("fail to recvfrom");
            exit(1);
        }


        printf("[%s - %d]: %s \n",inet_ntoa(sendaddr.sin_addr),ntohs(sendaddr.sin_port),text);



    }



    return 0;
}