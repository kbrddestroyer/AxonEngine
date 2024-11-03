#include "serialization.h"


char* serialize(char* data, size_t size, uint32_t tag, size_t* total_size)
{
    *total_size = 0;
    size_t header_size = 0;
    size_t footer_size = 0;

    while (size >> header_size * 8)
        header_size += 1;
    while (tag >> footer_size * 8)
        footer_size += 1;

    *total_size = size + header_size + footer_size;
    char* buffer = calloc(*total_size, sizeof(char));
    
    if (buffer == NULL)
        return NULL;   // ERR_COULD_NOT_ALLOC

    memcpy(buffer, &size, header_size);
    memcpy(buffer + header_size, data, size);
    memcpy(buffer + header_size + size, &tag, footer_size);

    return buffer;
}

uint8_t deserialize(char* serialized, size_t size, char** deserialized, size_t* actualSize, uint32_t* tag)
{
    if (deserialized == NULL)
        return 2;   // ERR_NO_BUFFER

    size_t actual;
    actual = size;
    size_t header_size = sizeof(actual);
    while (size <= actual && header_size > 0)
    {
        size_t crop = (sizeof(actual) - --header_size) * 8;
        actual = (*(size_t*)(serialized));
        actual &= (1ULL << header_size * 8) - 1;
    }

    if (header_size == 0)
    {
        return 1;
    }

    *deserialized = calloc(actual + 1, sizeof(char));
    
    if (*deserialized == NULL)
        return 1;   // ERR_COULD_NOT_ALLOC

    memcpy(*deserialized, serialized + header_size, actual);
    (*deserialized)[actual] = 0;

    size_t footer_size = size - actual - header_size;
    size_t crop = (sizeof(actual) - footer_size) * 8;

    /*
    *   Here we need to crop N higher bits of TAG variable
    * 
    * e.g. footer_size = 2
    * (1ULL << 2 * 8) ,b	    0b0000000000000000000000000000000000000000000000010000000000000000	unsigned __int64
    * (1ULL << 2 * 8) - 1 ,b	0b0000000000000000000000000000000000000000000000001111111111111111	unsigned __int64
    */

    *tag = (*(uint64_t*) (serialized + actual + header_size)) & ((1ULL << footer_size * 8) - 1);
    // *tag = *tag << crop >> crop; 
    *actualSize = actual;
    return 0;
}