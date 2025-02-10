/* AxonMessage.hpp */

#pragma once

#include <serialization.h>
#include <string>

namespace Networking
{
	class AxonMessage
	{
	private:
		const char* serialized;
	
	public:
		const char* getMessage() { return serialized; }
		size_t getSize() { strlen(serialized); }
	}; 
}

// AxonMessage.hpp
