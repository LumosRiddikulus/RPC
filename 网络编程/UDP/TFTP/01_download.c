#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>


void do_download(int sockfd, struct sockaddr_in serveraddr)
{
    char filename[128] = "";
    printf("Please input the file name:\n");
    scanf("%s",filename);


    //给服务器发送消息，告知服务器执行下载操作
    unsigned char text[1024] = "";
    int text_len;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int fd;
    int flags = 0;
    int num = 0;
    ssize_t bytes;

    //构建给服务器发送的tftp指令并发送
    text_len = sprintf(text, "%c%c%s%c%c%c",0,1,filename,0,"octet",0);
    if(sendto(sockfd,text,text_len,0,(struct sockaddr*)&serveraddr,addrlen) < 0)
    {
        perror("fail to sendto");
        exit(1);

    }


    while(1)
    {
        if((bytes = recvfrom(sockfd,text,sizeof(text),0,(struct sockaddr*)&serveraddr,addrlen)) == -1)
        {
            perror("fail to recvfrom");
            exit(1);
        }


        if(text[1] == 5)
        {
            printf("error: %s\n",text+4);   //直接打印错误信息
            return;
        }
        else if(text[1] == 3)
        {
            if(flags == 0)
            {
                if((fd = open(filename,O_WRONLY | O_CREAT | O_TRUNC,0664)) < 0)
                {
                    perror("fail to open");
                    exit(1);
                }
                flags = 1;
            }
        }

        if((num+1 == ntoh(*(unsigned short*)(text+2))) && (bytes == 516))   //对比编号和接收数据的大小并将文件内容写入文件
        {
            num = ntohs(*(unsigned short *)(text+2));

            if(write(fd,text+4,bytes-4) < 0)
            {
                perror("fail to write");
                exit(1);
            }


            //当文件写入完毕后，给服务器发送ACK
            text[1] = 4;
            if(sendto(sockfd,text,4,0,(struct sockaddr*)&serveraddr,addrlen) < 0)
            {
                perror("fail to sendto");
                exit(1);
            }
        }

        //当最后一个数据接受完毕后，写入文件后退出函数
        else if ((num+1 == ntohs(*(unsigned short*)(text+2))) && (bytes < 516))
        {
            if(write(fd,text+4,bytes-4) < 0)
            {
                perror("fail to write");
                exit(1);
            }

            text[1] = 4;
            if(sendto(sockfd,text,4,0,(struct sockaddr*)&serveraddr,addrlen) < 0)
            {
                perror("fail to sendto");
                exit(1);
            }

            printf("download success\n");
            return;
        }
        

    }

}

int main(int argc, char const *argv[])
{
    if(argc < 3)
    {
        fprintf(stderr,"Usage: %s <ip> <port>",argv[0]);
        exit(1);
    }

    int sockfd;
    struct sockaddr_in serveraddr;

    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        perror("fail to socket");
        exit(1);
    }


    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);    //tftp服务器端的ip
    serveraddr.sin_port = htons(69);     //这里是默认69


    do_download(sockfd, serveraddr);
    


    return 0;
}