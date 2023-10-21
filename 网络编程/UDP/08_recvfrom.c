#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

#define N 128


int main(int argc, char const *argv[])
{
    int sockfd;



        if(argc < 3)
        {
            fprintf(stderr,"Usage:%s ip port\n",argv[0]);
            exit(1);
        }

        if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
        {   
            perror("Fail to socket");
            exit(1);
        }

        struct sockaddr_in serveraddr;
        socklen_t socklen = sizeof(serveraddr);
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
        serveraddr.sin_port = htons(atoi(argv[2]));

  //  while(1)
    {
        if((bind(sockfd,(struct sockaddr*)&serveraddr,socklen)) == -1)
        {
            perror("Fail to bind");
            exit(1);
        }

    }

    char buf[N] = "";
    struct sockaddr_in clientaddr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    if(recvfrom(sockfd,buf,N,0,(struct sockaddr *)&clientaddr,&addrlen) == -1)
    {
        perror("fail to recvfrom");
        exit(1);
    }

    printf("ip:%s,port:%d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
    printf("from client:%s\n",buf);



    return 0;
}