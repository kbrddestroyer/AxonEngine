#include "serialization.h"
#include <string.h>


char* serialize(const char* data, const size_t size, const TAG_T tag, size_t* totalSize)
{
    *totalSize = 0;
    size_t header_size = 0;
    size_t footer_size = 0;

    while (size >> header_size * 8)
        header_size += 1;
    while (tag >> footer_size * 8)
        footer_size += 1;

    *totalSize = size + header_size + footer_size;
    char* buffer = calloc(*totalSize, sizeof(char));
    
    if (buffer == NULL)
        return NULL;   // ERR_COULD_NOT_ALLOC
    
    memcpy(buffer, &size, header_size);
    memcpy(buffer + header_size, data, size);
    memcpy(buffer + header_size + size, &tag, footer_size);

    return buffer;
}

uint8_t deserialize(const char* serialized, const size_t size, void** deserialized, size_t* actualSize, TAG_T* tag)
{
    size_t actual = size;
    size_t header_size = sizeof(actual);
    do
    {
        actual = (*(size_t*)(serialized));
        actual &= (1ULL << --header_size * 8) - 1;
    } while (size < actual && header_size > 0);

    if (header_size == 0 || actual == 0) {
        return 1;
    }

    /* Shrink header size */

    size_t shrunk = actual;
    while (shrunk == actual != 0 && header_size > 0)
    {
        shrunk = actual & (1ULL << --header_size * 8) - 1;
    }
    header_size++;

    if (header_size == 0)
    {
        return 1;
    }

    *deserialized = calloc(actual, sizeof(char));

    if (*deserialized == NULL)
        return 2;
    
    memcpy(*deserialized, serialized + header_size, actual);
    const size_t footer_size = size - actual - header_size;
    
    *tag = (*(uint64_t*) (serialized + actual + header_size)) & ((1ULL << footer_size * 8) - 1);
    *actualSize = actual;

    return 0;
}