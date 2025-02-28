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
	public:
		AxonMessage() = default;
		AxonMessage(const char*, size_t);
		AxonMessage(const AxonMessage&);

		const char* getMessage() { return serialized; }
		size_t getSize() { return size; }
	}; 

	class Message
	{
	private:
		void* bitstream; 
		size_t size;
		uint32_t tag;
	public:
		Message() = default;
		Message(const char*, size_t, uint32_t);
		Message(AxonMessage&);

		~Message();

		AxonMessage toMessage();

	public:
		inline void* getBitstream() { return bitstream; }
		inline size_t getSize() { return size; }
		inline uint32_t getTag() { return tag; }
	};
}

// AxonMessage.hpp
