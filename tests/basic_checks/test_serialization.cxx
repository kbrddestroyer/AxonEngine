#include <serialization/serialization.hpp>
#include <assert.h>
#include <string.h>


int main() { 
	const char*		message		= "Some hella big data";
	char			buffer[256] = { 0 };
	uint32_t		tag			= 3;	// 3
	size_t			total		= 0;

	const char* serialized = const_cast<const char*>(serialize((char* ) message, strlen(message), tag, &total));

	assert(total > 0);
	size_t deserialized_size = 0;

	deserialize((char*)serialized, total, &buffer[0], &deserialized_size, &tag);

	assert(tag == 3);
	assert(strcmp(buffer, message) == 0);
}
