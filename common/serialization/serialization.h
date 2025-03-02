#ifndef AXONENGINE_SERIALIZATION_H
#define AXONENGINE_SERIALIZATION_H


#include <stdlib.h>
#include <stdint.h>
#include <AxonUtility.h>

/**
* Creates serialized string from data given
* 
* @param char* data - data to serialize
* @param size_t size - actual data size, bytes
* @param uint32_t tag - message footer with utility tags
* @param[out] size_t* total_size - will contain total serialized data size, bytes
* @returns char* serialized message
*/
AXON_DECLSPEC char* serialize(char*, size_t, uint32_t, size_t*);

/**
* @param char* serialized - raw data
* @param size_t size - actual data size, bytes
* @param char* deserialized - will contain deserialized message, dealloc will be needed
* @param size_t* actualSize - will contain deserialized data size, bytes
* @param uint32_t* tag - will contain deserialized message footer
* @returns 0 or error code
*/
AXON_DECLSPEC uint8_t deserialize(const char*, size_t, void**, size_t*, uint32_t*);

#endif	// AXONENGINE_SERIALIZATION_H
