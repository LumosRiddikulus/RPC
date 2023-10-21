#include <stdio.h>
#include <arpa/inet.h>

//地址转换函数

int main()
{
    unsigned char ip_int[] = {192,168,3,103};
    char ip_str[16] = "";

    inet_ntop(AF_INET,&ip_int,ip_str,16);

    printf("ip_s = %s\n",ip_str);


    return 0;
}