/**
 * 服务端的socket连接步骤如下：
 *      第一步：使用socket函数创建套接字
 *      第二步：使用bind将该套接字与固定端口进行绑定
 *      第三步：绑定成功之后，使用listen函数开始在该端口监听以等待客户端发送数据
 *      第四步：接收客户端传来的连接请求，使用accept函数接收请求
 *      第五步：使用recv函数接收客户端发送的内容
 *      第六步：使用send函数传送服务端要发送给客户端的数据
 *      第七步：使用close函数关闭socket连接
 */

#include <stdio.h> // 用于基本输入/输出
#include <string.h> // 用于基本的字符串处理
#include <sys/types.h> // 数据类型定义
#include <sys/socket.h> // 提供socket函数及其相关数据结构
#include <arpa/inet.h> // 提供IP地址转换函数
#include <netinet/in.h> // 定义数据结构sockaddr_in
#include <errno.h> // 提供错误号errno的定义、用于错误处理
#include <unistd.h> // 提供通用额度文件、目录、程序及进程操作的函数
#include "PayloadProcess.h"
#include <stdint.h>
#include <stdlib.h>
#include "Serialization.h"
#include "ArgumentParser.h"

#define BUFFERSIZE 1024 // 定义数据缓冲区大小为1024字节


int port; // 端口号

int main(int argc, char* argv[]) {

    Parser(argc, argv, &port);

    /* 第一步：创建socket连接 */
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if(s == -1)
    {
        fprintf(stderr, "Socket create error: %s\n", strerror(errno));
        return -1;
    }

    /* 设置sockaddr相关信息 */
    struct sockaddr_in sockaddrServer; // 定义IP地址结构
    memset(&sockaddrServer, 0, sizeof (sockaddrServer));
    sockaddrServer.sin_port = htons(port); // 指定一个端口号并将主机序转换为网络序
    sockaddrServer.sin_family = AF_INET; // 指定结构类型为TCP/IP
    sockaddrServer.sin_addr.s_addr = htonl(INADDR_ANY); // 将要连接的IP地址设置为INADDR_ANY, 表示服务端接受任意地址的连接

    /* 第二步：使用bind绑定端口 */
    int bindResult = bind(s, (struct sockaddr*)&sockaddrServer, sizeof(sockaddrServer));
    if(bindResult == -1)
    {
        fprintf(stderr, "Bind Error: %s\n", strerror(errno));
        return -1;
    }
while(1)
{
    /* 第三步：使用listen监听等待连接 */
    fprintf(stdout, "Wait for connection: \n");
    int listenResult = listen(s, 5);
    if(listenResult == -1)
    {
        fprintf(stderr, "Listen Error: %s\n", strerror(errno));
        return -1;
    }


    /* 第四步：使用accept接收来自客户端的连接请求 */
    struct sockaddr_in sockaddrClinet; // 用于接收客户端发送的socket连接
    int lengthofClient = 0; // 用于存储客户端的socket长度
    memset(&sockaddrClinet,0, sizeof(sockaddrClinet));
    int receiveSocket = accept(s, (struct sockaddr*)&sockaddrClinet, (socklen_t*)&lengthofClient);
    if(receiveSocket == -1)
    {
        fprintf(stderr, "Accept Error: %s\n", strerror(errno));

        return -1;
    }

    int pid = fork();
    if(pid == -1)
    {
        fprintf(stderr, "Fork Failed: %s", strerror(errno)); // 多进程失败

    }
    if(pid > 0)
    {
        continue; // 父进程返回循环开始位置继续受理与客户端的连接
    }

    /* 第五步：接收来自客户端发送的信息 */
    char recvData[BUFFERSIZE]; // 创建用于接收数据的缓冲池
    struct Payload recvPayloadData;
    char* recvDataBuffer;
    char sendData[BUFFERSIZE];
    struct Payload** result;
    while(1)
    {
        //recv(receiveSocket, &recvPayloadData, sizeof(struct Payload), 0);
        recv(receiveSocket, recvData, BUFFERSIZE, 0);
        DeSerialization(recvData, &recvPayloadData);
        if(recvPayloadData.data[0] == 0)
            break;
        fprintf(stdout, "Have received message:\n");
        DisplayMessage(recvPayloadData);
        ProcessRecvData(&recvPayloadData, & recvDataBuffer);
        fprintf(stdout, "After process, the message is:\n");
        DisplayMessage(recvPayloadData);
        fprintf(stdout, "\nPrepare to send back:\n");


        result = ProcessSendData(recvDataBuffer, recvPayloadData.op, recvPayloadData.shift);
        for(int i = 0;; i++)
        {
            memset(sendData, 0, BUFFERSIZE);
            Serialization(sendData, result[i]);
            send(receiveSocket, sendData, BUFFERSIZE, 0);
            if(result[i]->data[0] == 0)
                break;
            fprintf(stdout, "Have sent Message:\n");
            DisplayMessage(*result[i]);
            sleep(1);
        }
    }
    /* 第六步：关闭连接 */
    close(s);
}
    return 0;
}
