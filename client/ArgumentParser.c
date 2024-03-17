//
// Created by 尹彦江 on 2024/3/14.
//

#include "ArgumentParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void Parser(int argc, char* argv[], int* port, int* op, int* shift, char* host)
{
    if(argc != 9)
    {
        fprintf(stderr, "Usage: ./client -h [host IP address] -p [port number] -o [op code] -s [shift number]\n");
        exit(EXIT_FAILURE);


    }

    for (int i = 0; i < argc; ++i) {
        if(strcmp(argv[i], "-h") == 0)
        {
            memcpy(host, argv[i + 1], strlen(argv[i + 1]));
        }
        else if(strcmp(argv[i], "-p") == 0)
        {
            *port = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-o") == 0)
        {
            *op = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-s") == 0)
        {
            *shift = atoi(argv[i + 1]);
        }
    }
}