#include <serialization/serialization.hpp>
#include <assert.h>
#include <string.h>
#include <cstdio>


int main() { 
	const char*		message		= "Some hella big data";
	void* buffer;
	TAG_T		tag			= 3;
	size_t			total		= 0;

	const char* serialized = const_cast<const char*>(serialize((char* ) message, strlen(message), tag, &total));

	assert(total > 0);
	size_t deserialized_size = 0;

	deserialize((char*)serialized, total, (void**) &buffer, &deserialized_size, &tag);

	assert(tag == 3);
	((char*) buffer)[deserialized_size] = 0;
	assert(strcmp((const char*) buffer, message) == 0);
	
	uint32_t data = 0;
	serialized = const_cast<const char*>(serialize((char*) &data, sizeof(data), 0, &total));
	assert(serialized);

	deserialize((char*)serialized, total, (void**)&buffer, &deserialized_size, &tag);
	uint32_t* dataptr = (uint32_t*) buffer;
	
	assert((*dataptr) == 0);

	free(buffer);
	free((void*) serialized);
}
