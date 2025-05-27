#include "serialization.h"
#include <string.h>


uint8_t extractMetadata(const char* serialized , size64_t size, size64_t *actualSize, TAG_T * tag) {
    size64_t actual;
    size64_t header_size = sizeof(actual);

    do
    {
        actual = (*(size64_t*)(serialized));
        actual &= (1ULL << --header_size * 8) - 1;
    } while (size < actual && header_size > 0);

    if (header_size == 0 || actual == 0) {
        return 1;
    }

    /* Shrink header size */

    size64_t shrunk = actual;
    while (shrunk == actual != 0 && header_size > 0)
    {
        shrunk = actual & (1ULL << --header_size * 8) - 1;
    }
    header_size++;

    if (header_size == 0)
    {
        return 1;
    }

    const size64_t footer_size = size - actual - header_size;
    *tag = (*(uint64_t*) (serialized + actual + header_size)) & ((1ULL << footer_size * 8) - 1);
    *actualSize = actual;

    return 0;
}


char* serialize(const char* data, const size64_t size, const TAG_T tag, size64_t* totalSize)
{
    *totalSize = 0;
    size64_t header_size = 0;
    size64_t footer_size = 0;

    while (size >> header_size * 8)
        header_size += 1;
    while (tag >> footer_size * 8)
        footer_size += 1;

    *totalSize = size + header_size + footer_size;
    if (*totalSize == 0)
        return NULL;

    char* buffer = calloc(*totalSize, sizeof(char));
    
    if (buffer == NULL)
        return NULL;   // ERR_COULD_NOT_ALLOC

    if (header_size > 0)
        memcpy(buffer, &size, header_size);
    if (data != NULL && size > 0)
        memcpy(buffer + header_size, data, size);
    memcpy(buffer + header_size + size, &tag, footer_size);

    return buffer;
}

uint8_t deserialize(const char* serialized, const size64_t size, void** deserialized, size64_t* actualSize, TAG_T* tag)
{
    if (size == 0)
        return 0;

    size64_t actual;
    size64_t header_size = sizeof(actual);
    do
    {
        actual = (*(size64_t*)(serialized));
        actual &= (1ULL << --header_size * 8) - 1;
    } while (size < actual && header_size > 0);

    if (actual == 0) {
        const size64_t footer_size = size - actual;
        deserialized = NULL;
        *actualSize = 0;
        *tag = (*(uint64_t*) (serialized + actual)) & ((1ULL << footer_size * 8) - 1);

        return 0;
    }

    /* Shrink header size */

    size64_t shrunk = actual;
    while (shrunk == actual != 0 && header_size > 0)
    {
        shrunk = actual & (1ULL << --header_size * 8) - 1;
    }
    if ( size == actual + header_size )
    {
        // Actual size is 0
        if (header_size == 0)
            actual = 0;
        const size64_t footer_size = size - actual - header_size;
        deserialized = NULL;
        *actualSize = 0;
        *tag = (*(uint64_t*) (serialized + actual + header_size)) & ((1ULL << footer_size * 8) - 1);

        return 0;
    }
    header_size++;

    const size64_t footer_size = size - actual - header_size;

    *deserialized = calloc(actual, sizeof(char));

    if (*deserialized == NULL)
        return 2;
    
    memcpy(*deserialized, serialized + header_size, actual);

    *tag = (*(uint64_t*) (serialized + actual + header_size)) & ((1ULL << footer_size * 8) - 1);
    *actualSize = actual;

    return 0;
}
