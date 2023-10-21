#include <stdio.h>
#include <sys/socket.h>        //socket
#include <sys/types.h>          //socket
#include <netinet/ether.h>  //ETH_P_ALL
#include <unistd.h>     //close
#include <stdlib.h>     //exit
#include <arpa/inet.h>  //htons
#include <string.h>

//原始套接字要sudo!!!!!!!!!!!!!!!

int main(int argc, char const* argv[])
{
    int sockfd;
    if((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
    {
        perror("fail to socket");
        exit(1);
    }

    printf("sockfd = %d\n",sockfd);

    close(sockfd);
    return 0;
}