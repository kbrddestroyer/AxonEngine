#include <serialization/serialization.hpp>
#include <assert.h>
#include <string.h>


int main() { 
	const char*		message		= "Some hella big data";
	void* buffer;
	uint32_t		tag			= 3;
	size_t			total		= 0;

	const char* serialized = const_cast<const char*>(serialize((char* ) message, strlen(message), tag, &total));

	assert(total > 0);
	size_t deserialized_size = 0;

	deserialize((char*)serialized, total, (void**) &buffer, &deserialized_size, &tag);

	assert(tag == 3);
	assert(strcmp((const char*) buffer, message) == 0);

	free(buffer);
}
