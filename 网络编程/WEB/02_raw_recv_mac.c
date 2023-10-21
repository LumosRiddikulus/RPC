#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ether.h> // ETH_P_ALL
#include <unistd.h>
#include <arpa/inet.h>

#define ERRLOG(errmsg) do{\
                            perror(errmsg);\
                            exit(1);\
                        }while(0)


int main(int argc,char const *argv[])
{
    int sockfd;
    if((sockfd = socket(AF_PACKET,SOCK_RAW, htons(ETH_P_ALL))) < 0)         //只有原始套接字才能完完整整地抓住一个数据包
    {
        ERRLOG("fail to socket");

    }


    //接收数据并分析
    unsigned char msg[1600] = "";
    while(1)
    {
        //recv recvfrom read 都可以用
        if(recvfrom(sockfd,msg,sizeof(msg),0,NULL,NULL) < 0)
        {
            ERRLOG("fail to recvfrom");

        }

        //分析接收到的数据包
        unsigned char dst_mac[18] = "";
        unsigned char src_mac[18] = "";
        unsigned short type;
        sprintf(dst_mac,"%x:%x:%x:%x:%x:%x",msg[0],msg[1],msg[2],msg[3],msg[4],msg[5]);
        sprintf(src_mac,"%x:%x:%x:%x:%x:%x",msg[6],msg[7],msg[8],msg[9],msg[10],msg[11]);
        type = ntohs(*(unsigned short*)(msg + 12));


        printf("源mac: %s --> 目的mac: %s\n",src_mac,dst_mac);
        printf("type = %#x\n",type);

    }


    return 0;
}