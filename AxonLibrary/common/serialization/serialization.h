#ifndef AXONENGINE_SERIALIZATION_H
#define AXONENGINE_SERIALIZATION_H


#include <stdlib.h>
#include <stdint.h>

extern char* serialize(char*, size_t, uint32_t, size_t*);
extern uint8_t deserialize(char*, size_t, char**, size_t*, uint32_t*);

#endif	// AXONENGINE_SERIALIZATION_H
