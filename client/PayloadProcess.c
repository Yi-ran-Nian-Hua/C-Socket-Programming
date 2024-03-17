//
// Created by 尹彦江 on 2024/3/13.
//

#include "PayloadProcess.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <stdlib.h>


/**
 * @brief 显示接收/发送包的信息
 * @param payload
 */
void DisplayMessage(const struct Payload payload)
{
    printf("%s\n", payload.data);
}

/**
 * @brief 用于构建要发送的数据包
 * @param data 要发送的数据，长度需要在10e7字节以内
 * @param op 要进行的操作，只允许0和1两种值，0代表要进行加密，1代表要进行解密
 * @param shiftNumber 要移位的个数，用于进行加密/解密移位，范围从0 ~ 65535
 * @param payload 构建好的数据包要存储的位置
 * @param strLength 需要处理的数据字符串长度
 */
void BuildPayload(const char* data, const uint16_t op, const uint16_t shiftNumber, struct Payload* payload, const uint32_t strLength)
{
    payload->op = op; // 存储op码
    payload->shift = shiftNumber; // 存储移动位数
    payload->data = (char*)malloc(sizeof(char) * strLength + 1);
    strncpy(payload->data, data, strLength);
    payload->length = 2 + 2 + strlen(payload->data) + 4; // 总长度 = 两个字节的op + 两个字节的shift number + 四个字节的length + 数据长度（一个char占一个字节，因此直接使用strlen即可）

    /* 接下来将所有的数据从主机序转换成字节序 */

    payload->op = htons(payload->op);
    payload->shift = htons(payload->shift);
    payload->length = htonl(payload->length);

    /* 因为一个char是一个字节，不存在字节序转换问题 */
}

/**
 * 用于处理要发送的数据
 * @param data 要发送的数据
 * @param op 要进行的操作，只允许0和1两种值，0代表要进行加密，1代表要进行解密
 * @param shiftNumber 要移位的个数，用于进行加密/解密移位，范围从0 ~ 65535
 * @return 构建好的载荷数组，类型为struct Payload**
 */
struct Payload** ProcessSendData(const char* data, const uint16_t op, const uint16_t shiftNumber)
{
    uint32_t groupTime; // 用于存储需要将数据分成几个部分
    uint32_t totalLength = strlen(data); // 存储整个数据的长度
    groupTime = totalLength / (int)10e7 + 1; // 求出分组个数
    struct Payload** payloads = (struct Payload**)malloc((groupTime + 1) * sizeof(struct Payload*)); // 分配存储指针的数组内存
    if (payloads == NULL) {
        // 内存分配失败
        return NULL;
    }
    uint32_t currentProcess = 0; // 当前已经处理的长度
    uint32_t remainLength = totalLength - currentProcess; // 剩余长度
    for (int i = 0; i < groupTime; i++)
    {
        payloads[i] = (struct Payload*)malloc(sizeof(struct Payload)); // 采用堆区存储，创建元素
        if (payloads[i] == NULL) {
            // 内存分配失败
            // 释放之前分配的内存
            for (int j = 0; j < i; j++) {
                free(payloads[j]);
            }
            free(payloads);
            return NULL;
        }
        if(i!=groupTime - 1) // 对于第一组到倒数第二组
        {

            BuildPayload(&data[currentProcess], op, shiftNumber, payloads[i], (int)10e7); // 送入符合长度的数据进行载荷构造
            currentProcess += (int)10e7; // 更新当前处理的长度
            remainLength = totalLength - currentProcess; // 更新当前还没有处理的长度
        }
        else // 对于最后一组数据
        {
            BuildPayload(&data[currentProcess], op, shiftNumber, payloads[i],remainLength);
        }
    }
    payloads[groupTime] = (struct Payload*) malloc(sizeof(struct Payload));
    BuildPayload(NULL, op, shiftNumber, payloads[groupTime],0); // 创建一个额外的包，代表当前的所有数据均已经传输完
    return payloads;
}


/**
 * 用于对接收到的数据进行处理
 * @param payload 接收到的载荷
 * @param data 要保存的结果
 */
void ProcessRecvData(struct Payload* payload)
{
    /* 将接收到的数据从网络序转换为主机序 */
    payload->length = ntohl(payload->length);
    payload->shift = ntohs(payload->shift);
    payload->op = ntohs(payload->op);

}