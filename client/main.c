/**
 * 客户端的socket连接步骤如下：
 *      第一步：使用socket函数建立套接字
 *      第二步：使用connect与服务端建立连接
 *      第三步：待服务器accept之后，使用send函数发送数据
 *      第四步：使用recv接收传送回来的数据
 *      第五步：如果要关闭连接，使用close函数关闭连接
 */

#include <stdio.h> // 用于基本输入/输出
#include <string.h> // 用于基本的字符串处理
#include <sys/types.h> // 数据类型定义
#include <sys/socket.h> // 提供socket函数及其相关数据结构
#include <arpa/inet.h> // 提供IP地址转换函数
#include <netinet/in.h> // 定义数据结构sockaddr_in
#include <errno.h> // 提供错误号errno的定义、用于错误处理
#include <unistd.h> // 提供通用额度文件、目录、程序及进程操作的函数
#include <stdlib.h>
#include "PayloadProcess.h"
#include "Serialization.h"
#include "ArgumentParser.h"

#define BUFFERSIZE 1024 // 定义数据缓冲区大小为1024字节

char ip_str[15]; // 表示IP地址字符串
int port; // 端口号
int op, shift;

int main(int argc, char* argv[])
{

    Parser(argc,argv, &port, &op, &shift, ip_str);

    /* 第一步：使用socket创建连接 */
    int s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sockaddr; // 定义IP地址结构
    char sendData[BUFFERSIZE]; // 创建用于发送数据的缓冲池  char sendData[1024];
    if(s == -1)
    {
        fprintf(stderr, "Socket create Error:%s\n", strerror(errno));
        return -1;
    }

    memset(sendData, 0, sizeof(sendData));
    memset(&sockaddr, 0, sizeof(sockaddr)); // 初始化为0
    sockaddr.sin_family = AF_INET; // 设置结构类型为TCP/IP
    sockaddr.sin_addr.s_addr = inet_addr(ip_str); // 将字符串的IP地址转换为int类型，这里的IP地址指的是客户端要连接的IP地址
    sockaddr.sin_port = htons(port); // 指定一个端口号并将主机序转换为字节序

    /* 第二步：使用connect建立连接 */
    int connectRecived = connect(s, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    if(connectRecived == -1)
    {
        fprintf(stderr, "Connect Error: %s\n", strerror(errno));
        return -1;
    }

    /* 第三步：向服务端发送数据 */
    struct Payload** result = ProcessSendData("abcdef", op, shift);
    for(int i = 0;; i++)
    {
        memset(sendData, 0, BUFFERSIZE);
        Serialization(sendData, result[i]);
        send(s, sendData, BUFFERSIZE, 0);
        if(result[i]->data[0] == 0)
            break;

        fprintf(stdout, "Have sent Message:\n");
        DisplayMessage(*result[i]);
        sleep(1);
    }


    /* 第四步：接收服务端发送的数据 */
    char recvData[BUFFERSIZE]; // 创建用于接收数据的缓冲池
    struct Payload recvPayloadData;

    while(1)
    {
        recv(s, recvData, BUFFERSIZE, 0);
        DeSerialization(recvData, &recvPayloadData);
        ProcessRecvData(&recvPayloadData);
        if(recvPayloadData.data[0] == 0)
            break;
        fprintf(stdout, "Have received message:\n");
        DisplayMessage(recvPayloadData);
    }

    /* 第五步：关闭连接 */
    close(s);

    return 0;

}