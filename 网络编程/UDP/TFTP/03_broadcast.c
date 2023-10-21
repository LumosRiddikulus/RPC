#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
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
    struct sockaddr_in broadcastaddr;
    socklen_t addrlen = sizeof(broadcastaddr);


    //step1 创建套接字
    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        perror("fail to socket");
        exit(1);
    }


    int on = 1;
    if(setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on)) < 0)
    {
        perror("fail to setsockopt");
        exit(1);

    }


    broadcastaddr.sin_family = AF_INET;
    broadcastaddr.sin_addr.s_addr = inet_addr(argv[1]);
    broadcastaddr.sin_port = htons(atoi(argv[2]));

    char buf[128] = "";
    while(1)
    {
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf) - 1] = '\0';

        if(sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&broadcastaddr,addrlen) == -1)
        {
            perror("fail to sendto");
            exit(1);
        }

    }


    return 0;
}