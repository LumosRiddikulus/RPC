#include <stdio.h>

//判断字节序

union un
{
    int a;
    char b;
};


int main()
{
    union un myun;
    myun.a = 0x12345678;

    printf("a = %#x\n",myun.a);
    printf("b = %#x\n",myun.b);

    if(myun.b == 0x78)
    {
        printf("小端存储\n");
    }
    else
    {
        printf("大端存储\n");
    }

    return 0;
}