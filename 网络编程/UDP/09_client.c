#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const* argv[])
{
    int sockfd;

    if(argc < 3)
    {
        fprintf(stderr,"Usage: %s <ip> <port>\n",argv[0]);
        exit(1);
    }

    struct sockaddr_in serveraddr;
    in_addr_t addrlen = sizeof(serveraddr);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));
    


    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        perror("Fail to socket");
        exit(1);

    }

#if 0
    //客户端自己的IP和端口号，可指定可不指定，一般系统会自动分配
    struct sockaddr_in clientaddr;
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = inet_addr(argv[1]);
    clientaddr.sin_port = htons(atoi(argv[2]));
    socklen_t addrlen = sizeof(clientaddr);

    if(bind(sockfd,(struct sockaddr*)&clientaddr,addrlen) < 0)
    {
        perror("fail to bind");
        exit(1);
    }
#endif


    //第3步
    char buf[32] = "";

    while(1)
    {
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf) - 1] = '\0';

        if(sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
        {
            perror("fail to sendto");
            exit(1);
        }

        char text[32] = "";
        if(recvfrom(sockfd,text,sizeof(text),0,(struct sockaddr*)&serveraddr,&addrlen) == -1)
        {
            perror("fail to recvfrom");
            exit(1);
        }
        printf("from server:\n %s\n",text);


    }

    close(sockfd);


    return 0;
}