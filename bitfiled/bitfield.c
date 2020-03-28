#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>

#pragma pack(8) /* 指定 8 字节对齐 */
/* 测试机器字长: 32bit 
 * 小端测试: Intel i3
 * */

/* 位域字段长度大于 8 bit但非 8 bit整数倍, 且整个结构体占用刚好一个机器字长 */
struct bitField1 {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    unsigned long long a:10;
    unsigned long long b:10;
    unsigned long long c:20;
    unsigned long long d:24;
#endif
} bitFieldTest1;

/* 位域字段长度小于 8 bit, 且整个结构体占用刚好一个机器字长 */
struct bitField2 {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    unsigned long long a:2;
    unsigned long long b:6;
    unsigned long long c:4;
    unsigned long long d:52;;
#endif
} bitFieldTest2;

/* 占用不到一个机器字长 */
struct bitField3 {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    unsigned long a:2;
    unsigned long b:6;
    unsigned long c:4;
#endif
} bitFieldTest3;

/* 位域字段不到一个机器字长时，中间嵌入其他类型成员 */
struct bitField4 {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    unsigned long long a:2;
    unsigned long long b:6;
    unsigned long long c:4;
    unsigned int e;
    unsigned long long d:52;
#endif
} bitFieldTest4;

/* 各种类型的位域融合到一起 */
struct bitField5 {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    unsigned short a:2;
    unsigned short b:6;
    unsigned int c:4;
    unsigned int e;
    unsigned long long d:52;
#endif
} bitFieldTest5;

/* 各种类型的位域融合到一起, 且不指定 signed/unsigned */
struct bitField6 {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    short a:2;
    short b:6;
    int c:4;
    int e;
    long long d:52;
#endif
} bitFieldTest6;

int binary2decimal(char *str)
{
    int i;
    int ret = 0;
    
    for (i = 0; i < 8; i++) {
        ret += ((1 << (7 - i)) * (str[i] - '0'));
    }

    return ret;
}

char *expand_binary(unsigned long x, int len)
{
    static char str[128];
    int i;

    str[len] = '\0';
    for (i = len - 1; i >= 0; i--) {
        str[len - 1 - i] = 0x30 + !!(x & (1ul << i));
    }

    return str;
}

char *binaryStr(char *p)
{
    static char ret[9];

    memcpy(ret, p, 8);
    ret[8] = '\0';

    return ret;
}

void print_analy1(struct bitField1 *p)
{
    char str[512];
    int i;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    printf("Little-Endian Analysis:\n");
    printf("Expand order:");
    printf("%s, ", expand_binary(p->a, 10));
    printf("%s, ", expand_binary(p->b, 10));
    printf("%s, ", expand_binary(p->c, 20));
    printf("%s\n", expand_binary(p->d, 24));
    printf("High->Low:");
    strcpy(str, expand_binary(p->d, 24));
    strcat(str, expand_binary(p->c, 20));
    strcat(str, expand_binary(p->b, 10));
    strcat(str, expand_binary(p->a, 10));
    for (i = 0; i < strlen(str); i += 8) {
        printf("%08s,", binaryStr(&str[i]));
    }
    printf("\n");
    printf("Byte Order(Low->High):");
    for (i = strlen(str) - 8; i >= 0; i -= 8) {
        printf("%08s(%02x),", binaryStr(&str[i]), binary2decimal(&str[i]));
    }
    printf("\n");
#endif
}

void print_analy2(struct bitField2 *p)
{
    char str[512];
    int i;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    printf("Little-Endian Analysis:\n");
    printf("Expand order:");
    printf("%s, ", expand_binary(p->a, 2));
    printf("%s, ", expand_binary(p->b, 6));
    printf("%s, ", expand_binary(p->c, 4));
    printf("%s\n", expand_binary(p->d, 52));
    printf("High->Low:");
    strcpy(str, expand_binary(p->d, 52));
    strcat(str, expand_binary(p->c, 4));
    strcat(str, expand_binary(p->b, 6));
    strcat(str, expand_binary(p->a, 2));
    for (i = 0; i < strlen(str); i += 8) {
        printf("%08s,", binaryStr(&str[i]));
    }
    printf("\n");
    printf("Byte Order(Low->High):");
    for (i = strlen(str) - 8; i >= 0; i -= 8) {
        printf("%08s(%02x),", binaryStr(&str[i]), binary2decimal(&str[i]));
    }
    printf("\n");
#endif
}

void print_analy3(struct bitField3 *p)
{
    char str[512];
    int i;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    printf("Little-Endian Analysis:\n");
    printf("Expand order:");
    printf("%s, ", expand_binary(p->a, 2));
    printf("%s, ", expand_binary(p->b, 6));
    printf("%s, ", expand_binary(p->c, 4));
    printf("%s\n", expand_binary(0, 52));
    printf("High->Low:");
    strcpy(str, expand_binary(0, 52));
    strcat(str, expand_binary(p->c, 4));
    strcat(str, expand_binary(p->b, 6));
    strcat(str, expand_binary(p->a, 2));
    for (i = 0; i < strlen(str); i += 8) {
        printf("%08s,", binaryStr(&str[i]));
    }
    printf("\n");
    printf("Byte Order(Low->High):");
    for (i = strlen(str) - 8; i >= 0; i -= 8) {
        printf("%08s(%02x),", binaryStr(&str[i]), binary2decimal(&str[i]));
    }
    printf("\n");
#endif
}

void print_analy4(struct bitField4 *p)
{
    char str[512];
    int i;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    printf("Little-Endian Analysis:\n");
    printf("Expand order:");
    printf("%s, ", expand_binary(p->a, 2));
    printf("%s, ", expand_binary(p->b, 6));
    printf("%s, ", expand_binary(p->c, 4));
    printf("%s, ", expand_binary(0, 20)); // 对齐到 32 位即可
    printf("%s, ", expand_binary(p->e, 32));
    printf("%s, ", expand_binary(p->d, 52));
    printf("%s\n", expand_binary(0, 12));
    printf("High->Low:");
    strcpy(str, expand_binary(0, 12));
    strcat(str, expand_binary(p->d, 52));
    strcat(str, expand_binary(p->e, 32));
    strcat(str, expand_binary(0, 20));
    strcat(str, expand_binary(p->c, 4));
    strcat(str, expand_binary(p->b, 6));
    strcat(str, expand_binary(p->a, 2));
    for (i = 0; i < strlen(str); i += 8) {
        printf("%08s,", binaryStr(&str[i]));
    }
    printf("\n");
    printf("Byte Order(Low->High):");
    for (i = strlen(str) - 8; i >= 0; i -= 8) {
        printf("%08s(%02x),", binaryStr(&str[i]), binary2decimal(&str[i]));
    }
    printf("\n");
#endif
}

void print_analy5(struct bitField5 *p)
{
    char str[512];
    int i;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    printf("Little-Endian Analysis:\n");
    printf("Expand order:");
    printf("%s, ", expand_binary(p->a, 2));
    printf("%s, ", expand_binary(p->b, 6));
    printf("%s, ", expand_binary(p->c, 4));
    printf("%s, ", expand_binary(0, 20));
    printf("%s, ", expand_binary(p->e, 32));
    printf("%s, ", expand_binary(p->d, 52));
    printf("%s\n", expand_binary(0, 12));
    printf("High->Low:");
    strcpy(str, expand_binary(0, 12));
    strcat(str, expand_binary(p->d, 52));
    strcat(str, expand_binary(p->e, 32));
    strcat(str, expand_binary(0, 20));
    strcat(str, expand_binary(p->c, 4));
    strcat(str, expand_binary(p->b, 6));
    strcat(str, expand_binary(p->a, 2));
    for (i = 0; i < strlen(str); i += 8) {
        printf("%08s,", binaryStr(&str[i]));
    }
    printf("\n");
    printf("Byte Order(Low->High):");
    for (i = strlen(str) - 8; i >= 0; i -= 8) {
        printf("%08s(%02x),", binaryStr(&str[i]), binary2decimal(&str[i]));
    }
    printf("\n");
#endif
}

void print_analy6(struct bitField6 *p)
{
    char str[512];
    int i;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    printf("Little-Endian Analysis:\n");
    printf("Expand order:");
    printf("%s, ", expand_binary(p->a, 2));
    printf("%s, ", expand_binary(p->b, 6));
    printf("%s, ", expand_binary(p->c, 4));
    printf("%s, ", expand_binary(0, 20));
    printf("%s, ", expand_binary(p->e, 32));
    printf("%s, ", expand_binary(p->d, 52));
    printf("%s\n", expand_binary(0, 12));
    printf("High->Low:");
    strcpy(str, expand_binary(0, 12));
    strcat(str, expand_binary(p->d, 52));
    strcat(str, expand_binary(p->e, 32));
    strcat(str, expand_binary(0, 20));
    strcat(str, expand_binary(p->c, 4));
    strcat(str, expand_binary(p->b, 6));
    strcat(str, expand_binary(p->a, 2));
    for (i = 0; i < strlen(str); i += 8) {
        printf("%08s,", binaryStr(&str[i]));
    }
    printf("\n");
    printf("Byte Order(Low->High):");
    for (i = strlen(str) - 8; i >= 0; i -= 8) {
        printf("%08s(%02x),", binaryStr(&str[i]), binary2decimal(&str[i]));
    }
    printf("\n");
#endif
}

void struc_print(char *p, int len)
{
    int i,j;

    printf("binary output: ");
    for (i = 0; i < len; i++) {
        for (j = 7; j >= 0; j--) 
          printf("%d", !!(*(p + i) & (1 << j)));
        printf("(%02x) ", *(p + i) & 0xff);
    }
    printf("\n");
}

/*代码片段从网络截取，个人学习用*/
int main(void)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    printf("This Little-Endian Machine!\n");
#endif

    bitFieldTest1.a = 0x55;
    bitFieldTest1.b = 0xaa;
    bitFieldTest1.c = 0x5ff;
    bitFieldTest1.d = 0xff1;

    printf("\nbitFieldTest1 size %u sizeof long long %u\n", sizeof(bitFieldTest1), sizeof(unsigned long long));
    struc_print((char *)&bitFieldTest1, sizeof(bitFieldTest1));
    print_analy1(&bitFieldTest1);

    /*
    unsigned long long a:2;
    unsigned long long b:6;
    unsigned long long c:4;
    unsigned long long d:52;;    
    */
    bitFieldTest2.a = 0x3;
    bitFieldTest2.b = 0x14;
    bitFieldTest2.c = 0x5;
    bitFieldTest2.d = 0xc3f1794a;

    printf("\nbitFieldTest2 ");
    struc_print((char *)&bitFieldTest2, sizeof(bitFieldTest2));
    print_analy2(&bitFieldTest2);
    
    bitFieldTest3.a = 0x1;
    bitFieldTest3.b = 0x14;
    bitFieldTest3.c = 0x5;
    unsigned long *test = (unsigned long *)&(bitFieldTest3);

    printf("\nbitFieldTest3 ");
    struc_print((char *)&bitFieldTest3, sizeof(bitFieldTest3));
    print_analy3(&bitFieldTest3);
    printf("\r\n test3: 0x%x\n", *test);


    /*
    unsigned long long a:2;
    unsigned long long b:6;
    unsigned long long c:4;
    unsigned int e;
    unsigned long long d:52;
    */
    bitFieldTest4.a = 0x1;
    bitFieldTest4.b = 0x14;
    bitFieldTest4.c = 0x5;
    bitFieldTest4.d = 0xc3f1794a;
    bitFieldTest4.e = 0xcceeffaa;

    printf("\nbitFieldTest4 size %u\n", sizeof(bitFieldTest4));
    struc_print((char *)&bitFieldTest4, sizeof(bitFieldTest4));
    print_analy4(&bitFieldTest4);

    /*
    unsigned short a:2;
    unsigned short b:6;
    unsigned int c:4;
    unsigned int e;
    unsigned long long d:52;
    */
    bitFieldTest5.a = 0x1;
    bitFieldTest5.b = 0x14;
    bitFieldTest5.c = 0x5;
    bitFieldTest5.d = 0xc3f1794a;
    bitFieldTest5.e = 0xcceeffaa;

    printf("\nbitFieldTest5 %u\n", sizeof(bitFieldTest5));
    struc_print((char *)&bitFieldTest5, sizeof(bitFieldTest5));
    print_analy5(&bitFieldTest5);
	
    /*
    short a:2;
    short b:6;
    int c:4;
    int e;
    long long d:52;    
    */
    bitFieldTest6.a = 0x1;
    bitFieldTest6.b = 0x14;
    bitFieldTest6.c = 0x5;
    bitFieldTest6.d = 0xc3f1794a;
    bitFieldTest6.e = 0xcceeffaa;

    printf("\nbitFieldTest6 %u", sizeof(bitFieldTest6));
    struc_print((char *)&bitFieldTest6, sizeof(bitFieldTest6));
    print_analy6(&bitFieldTest6);
    return 0;
}