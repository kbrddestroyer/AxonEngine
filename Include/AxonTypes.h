#pragma once
#include <Axon.h>

#include <cstdint>
#include <ostream>


#if defined(UNIX_PLATFORM)
#include <memory>
#include <cstring>
#endif

#ifdef __cplusplus
extern "C" {
#endif
#include <serialization/serialization.h>
#ifdef __cplusplus
}
#endif


namespace Axon::Connection
{
	typedef uint16_t AXON_PORT;

	/** 
	 * 	Basic class that contains data to share across the network
	 * 
	 *  @param data - payload bitsream
	 *  @param size - size of initial data in bytes
	 *  @param tag - control tags, that define how this package should be treated on the server 
	*/ 
	class Message
	{
	public:
		char* data;
		size_t size;
		uint32_t tag;
	public:
		Message() = default;
		Message(const Message&) noexcept;
		~Message() noexcept;

		static Message createUDPMessage(void*, size_t, uint32_t);
	};
}
