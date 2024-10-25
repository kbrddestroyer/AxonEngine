#include "serialization.h"


uint8_t serialize(char* data, size_t size, uint32_t tag, char** serialized, size_t* total_size)
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

    memcpy(buffer, &size, header_size);
    memcpy(buffer + header_size, data, size);
    memcpy(buffer + header_size + size, &tag, footer_size);

    *serialized = buffer;

    return 0;
}

uint8_t deserialize(char* serialized, size_t size, char** deserialized, size_t* actualSize, uint32_t* tag)
{
    size_t actual;
    actual = size;
    size_t header_size = sizeof(actual);
    while (size <= actual && header_size > 0)
    {
        size_t crop = (sizeof(actual) - --header_size) * 8;
        actual = (*(size_t*) (serialized)) << crop >> crop;
    }

    if (header_size == 0)
    {
        return 1;
    }

    *deserialized = calloc(actual, sizeof(char));
    memcpy(*deserialized, serialized + header_size, actual);

    size_t footer_size = size - actual - header_size;
    size_t crop = (sizeof(actual) - footer_size) * 8;
    *tag = (*(uint64_t*) (serialized + actual + header_size)) << crop >> crop;
    *actualSize = size;
    return 0;
}