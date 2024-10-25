#include "serialization.h"


char* serialize(struct UserMessage message, size_t * total_size)
{
    *total_size = 0;
    size_t header_size = 0;
    size_t footer_size = 0;

    while (message.size >> header_size * 8)
        header_size += 1;
    while (message.tag >> footer_size * 8)
        footer_size += 1;

    *total_size = message.size + header_size + footer_size;
    char* buffer = calloc(*total_size, sizeof(char));

    memcpy(buffer, &message.size, header_size);
    memcpy(buffer + header_size, message.data, message.size);
    memcpy(buffer + header_size + message.size, &message.tag, footer_size);

    return buffer;
}

struct UserMessage deserialize(char* serialized, size_t size)
{
    struct UserMessage message;

    message.size = size;
    size_t header_size = sizeof(message.size);
    while (size <= message.size && header_size > 0)
    {
        size_t crop = (sizeof(message.size) - --header_size) * 8;
        message.size = (*(uint32_t*) (serialized)) << crop >> crop;
    }

    if (header_size == 0)
    {

    }

    message.data = calloc(message.size, sizeof(char));
    memcpy(message.data, serialized + header_size, message.size);

    size_t footer_size = size - message.size - header_size;
    size_t crop = (sizeof(message.size) - footer_size) * 8;
    message.tag = (*(uint64_t*) (serialized + message.size + header_size)) << crop >> crop;

    return message;
}