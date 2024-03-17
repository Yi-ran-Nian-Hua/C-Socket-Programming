//
// Created by 尹彦江 on 2024/3/14.
//

#include "Serialization.h"
#include <string.h>

/**
 * 将结构体paylaod的数据存储到一块内存区域buffer中
 * @param buffer 数据存储区域
 * @param payload 要序列化的数据
 */
void Serialization(char* buffer, struct Payload* payload)
{
    size_t len = 0;
    memcpy(buffer, &payload->op, sizeof(payload->op));
    len+=sizeof(payload->op);
    memcpy(buffer + len, &payload->shift, sizeof(payload->shift));
    len+=sizeof(payload->shift);
    memcpy(buffer + len, &payload->length, sizeof(payload->length));
    len += sizeof(payload->length);
    memcpy(buffer + len, payload->data, strlen(payload->data) + 1);
}

/**
 * 将data中的数据重新处理成原始数据
 * @param data 进行序列化后的数据
 * @param payloadData 重新构成的原始数据
 */
void DeSerialization(char* data, struct Payload* payloadData)
{
    size_t len = 0;
    memcpy((uint16_t*)&payloadData->op, data, sizeof(payloadData->op));
    len += sizeof(payloadData->op);
    memcpy((uint16_t*)&payloadData->shift, data + len, sizeof((payloadData->shift)));
    len += sizeof(payloadData->shift);
    memcpy((uint32_t*)&payloadData->length, data + len, sizeof(payloadData->length));
    len += sizeof(payloadData->length);
    payloadData->data = strdup(data + len);

}