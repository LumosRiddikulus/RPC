#include <stdio.h>
#include <arpa/inet.h>

//地址转换函数

int main(void)
{
    char ip_str[] = "192.168.3.103";
    unsigned int ip_int = 0;
    unsigned char *ip_p = NULL;

    inet_pton(AF_INET,ip_str,&ip_int);
    printf("in_uint = %d\n",ip_int);

    ip_p = (char*)&ip_int;
    printf("in_uint = %d,%d,%d,%d\n",*ip_p,*(ip_p+1),*(ip_p+2),*(ip_p+3));

    return 0;
}