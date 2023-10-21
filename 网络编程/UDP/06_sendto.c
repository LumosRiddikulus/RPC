#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define N 128

int main(int argc, char const *argv[])
{
    int sockfd;


    if(argc < 3)
    {
        fprintf(stderr, "Usage: %s ip port\n",argv[0]);
        exit(1);
    }


    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
    {
        perror("fail to socket");
        exit(1);
    }

    printf("sockfd = %d\n",sockfd);




    struct sockaddr_in serveraddr;
    socklen_t addrlen = sizeof(serveraddr);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr =  inet_addr(argv[1]);       //sin_addr为32位整形数据
    serveraddr.sin_port = htons(*argv[2]);                                  //转换为大端字节序     





    char buf[N] = "";

    while(1)
    {
        fgets(buf,N,stdin);

        buf[strlen(buf) -1] = '\0';

        if(sendto(sockfd,buf,N,0,(struct sockaddr *)&serveraddr,addrlen) == -1)
        {
            perror("fail to sendto");
            exit(1);

        }

    }



    close(sockfd);

    return 0;
}