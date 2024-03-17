//
// Created by 尹彦江 on 2024/3/13.
//
#include "Caesar.h"
#include <string.h>
#include <ctype.h>

char lowerAlphabet[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };

char* Encryption(char* data, int shiftBit)
{
    for(int i = 0; i< strlen(data); i++)
    {
        if(isalpha(data[i]))
        {
            if(isupper(data[i]))
            {
                data[i] = tolower(data[i]);
                data[i] = lowerAlphabet[(data[i] - 'a' + shiftBit) % 26];
                data[i] = toupper(data[i]);
            }
            else
            {
                data[i] = lowerAlphabet[(data[i] - 'a' + shiftBit) % 26];
            }
        }

    }
    return data;
}


char* Decryption(char* data, int shiftBit)
{
    for(int i = 0; i< strlen(data); i++)
    {
        if (isalpha(data[i]))
        {
            if (isupper(data[i]))
            {
                data[i] = tolower(data[i]);
                int number = (data[i] - 'a' - shiftBit) % 26;
                if (number < 0)
                {
                    number += 26;
                }
                data[i] = lowerAlphabet[number];
                data[i] = toupper(data[i]);

            }
            else
            {
                int number = (data[i] - 'a' - shiftBit) % 26;
                if (number < 0)
                {
                    number += 26;
                }
                data[i] = lowerAlphabet[number];
            }
        }
    }

    return data;
}