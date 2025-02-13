/* AxonMessage.hpp */

#pragma once

#include "serialization/serialization.h"
#include <string>

namespace Networking
{
	enum FLAGS
	{
		F_VALIDATE		= 0b0001
	};

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
