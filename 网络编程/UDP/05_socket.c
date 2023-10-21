#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>


int socket(int domain, int type, int protocol);

/*
    domain:  通信域，协议族
        AF_UNIX     本地通信
        AF_INET     ipv4网络协议
        AF_INET6    ipv6网络协议
        AF_PACKET   底层接口
    type:    套接字类型
        SOCK_STREAM     流式套接字{tcp}
        SOCK_DGRAM      数据报套接字(udp)
        SOCK_RAW       原始套接字(用于链路层)

    protocol:
        附加协议，若不需要则设置为0

*/

int main(int argc, char const *argv[])
{
    //使用socket创建一个用于网络编程的套接字

    int sockfd;

    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
    {
        perror("fail to socket");
        exit(1);

    }

    printf("sockfd = %d\n",sockfd);

    return 0;
}