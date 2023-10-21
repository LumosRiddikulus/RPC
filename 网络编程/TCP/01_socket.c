#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc,char const *argv[])
{
    int sockfd;

    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("fail to socket");
        exit(1);

    }


    printf("sockfd = %d\n",sockfd);



    return 0;
}