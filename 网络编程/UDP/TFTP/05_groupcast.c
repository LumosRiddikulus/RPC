#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>


int main(int argc,char const *argv[])
{
    if(argc < 3)
    {
        fprintf(stderr,"Usage: %s <ip> <port>",argv[0]);
        exit(1);
    }

    int sockfd;
    struct sockaddr_in groupcastaddr;
    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        perror("fail to socket");
        exit(1);
    }

    groupcastaddr.sin_family = AF_INET;
    groupcastaddr.sin_addr.s_addr = inet_addr(argv[1]);
    groupcastaddr.sin_port = htons(atoi(argv[2]));
    socklen_t addrlen = sizeof(groupcastaddr);
    char buf[128] = "";
    while(1)
    {
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf) - 1] = '\0';

        if(sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&groupcastaddr,addrlen) < 0)
        {
            perror("fail to sendto");
            exit(1);
        }


    }


    return 0;
}