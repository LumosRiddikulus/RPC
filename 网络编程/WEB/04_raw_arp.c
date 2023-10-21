#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <netinet/ether.h>

#ifndef __USE_MISC
#define __USE_MISC
#endif
#include <net/if.h> //ifreq
#include <fcntl.h>
#include <sys/ioctl.h>  //ioctl
#include <netpacket/packet.h>   //sockaddr_ll
//#include <fstream>
#include <net/if_shaper.h>

#define ERRLOG(errmsg) do{\
                            perror(errmsg);\
                            exit(1);\
                        }while(0)

//使用ARP协议通过对方IP地址获取MAC地址
struct ifreq ethreq;

int main(int argc,char const *argv[])
{
    int sockfd;
    if((sockfd = socket(AF_PACKET,SOCK_RAW,0)) < 0)    
    {
        ERRLOG("fail to socket");
    }
    
    //组数据包
    //原MAC地址：
    unsigned char msg[1600] = {
        //组以太网首部
        0xff,0xff,0xff,0xff,0xff,0xff,  //目的MAC地址，广播地址
        0x00,0x0c,0x29,0x7b,0x35,0xd7,  //源MAC地址
        0x08,0x06,  //帧类型 ARP报文：0x0806

        //组ARP报文
        0x00,0x01,  //硬件类型 以太网：1
        0x08,0x06,  //协议类型 IP地址：0x0800
        6,          //硬件地址长度
        4,          //协议地址长度
        0x00,0x01,  //op  arp请求：1
        0x00,0x0c,0x29,0x7b,0x35,0xd7,  //源MAC地址
        192,168,1,13,  //源IP地址
        0x00,0x00,0x00,0x00,0x00,0x00,//目的MAC地址
        192,168,1,13    //目的IP地址

    };

    //获取接口信息
    //将ARP请求报文发送出去
    //使用iotcl函数获取本机网络接口
    struct ifreq ethreq;
    strncpy(ethreq.ifr_ifrn.ifrn_name,"wlp0s20f3",IF_NAMESIZE);

    if(ioctl(sockfd,SIOCGIFINDEX,&ethreq) == -1)
    {
        perror("fail to ioctl");
        exit(1);
    }



    //设置本机网络接口
    struct sockaddr_ll sll;
    bzero(&sll,sizeof(sll));
    sll.sll_ifindex = ethreq.ifr_ifru.ifru_netmask.sa_data;

    return 0;
}
