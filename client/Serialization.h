//
// Created by 尹彦江 on 2024/3/14.
//

#ifndef SERVER_SERIALIZATION_H
#define SERVER_SERIALIZATION_H

#include "PayloadProcess.h"

void Serialization(char* buffer, struct Payload* paylaod);
void DeSerialization(char* data, struct Payload* payloadData);

#endif //SERVER_SERIALIZATION_H
