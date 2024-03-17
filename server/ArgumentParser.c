//
// Created by 尹彦江 on 2024/3/14.
//

#include "ArgumentParser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void Parser(const int argc, char* argv[], int* port)
{
    if(argc != 3)
    {
        fprintf(stderr, "Usage：./server -p [port number]\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "-p") == 0)
        {
            *port = atoi(argv[i + 1]);
            break;
        }
    }
}