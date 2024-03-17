//
// Created by 尹彦江 on 2024/3/13.
//

#ifndef SERVER_PAYLOADPROCESS_H
#define SERVER_PAYLOADPROCESS_H
#include <stdint.h>

/**
 * Payload定义
 */
struct Payload
{
    uint16_t op; // 存储op的16位，也就是两个字节
    uint16_t shift; // 存储用于位移的16位，也就是两个字节
    uint32_t length; // 用于存储32位该数据载荷的长度，也就是4个字节，可以使用一个unsigned int类型存储
    char* data; // 用于存储数据
};

void DisplayMessage(struct Payload payload);
void BuildPayload(const char* data, uint16_t op, uint16_t shiftNumber, struct Payload* payload, uint32_t strLength);
struct Payload** ProcessSendData(const char* data, uint16_t op, uint16_t shiftNumber);
void ProcessRecvData(struct Payload* payload, char** data);
#endif //SERVER_PAYLOADPROCESS_H
