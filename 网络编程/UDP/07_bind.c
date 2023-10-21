#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//bind


int main(int argc, char const *argv[])
{
    int sockfd;

    if(argc < 3)
    {
        fprintf(stderr,"Usage: %s ip port\n",argv[0]);
        exit(1);
    }

    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
    {
        perror("Fail to socket");
        exit(1);
    }


    struct sockaddr_in serversock;
    socklen_t addrlen = sizeof(serversock);
    serversock.sin_family = AF_INET;
    serversock.sin_addr.s_addr = inet_addr(argv[1]);
    serversock.sin_port = htons(*argv[2]);

    if(bind(sockfd,(struct sockaddr *)&serversock, sizeof(serversock)) == -1)
    {
        perror("fail to bind");
        exit(1);

    }


    close(sockfd);



    return 0;
}