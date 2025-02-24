/* AxonMessage.hpp */

#pragma once
#include <serialization/serialization.hpp>
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
		size_t		size;
		const char* serialized;
	
	protected:
		AxonMessage() = default;
	public:
		AxonMessage(const char*, size_t);

		const char* getMessage() { return serialized; }
		size_t getSize() { return size; }
	}; 
}

// AxonMessage.hpp
