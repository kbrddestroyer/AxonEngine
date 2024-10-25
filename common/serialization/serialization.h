#ifndef AXONENGINE_SERIALIZATION_H
#define AXONENGINE_SERIALIZATION_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct UserMessage
{
    uint32_t    size;
    void*       data;
    uint32_t    tag;
};

extern char* serialize(struct UserMessage, size_t*);
extern struct UserMessage deserialize(char*, size_t);


#endif //AXONENGINE_SERIALIZATION_H
