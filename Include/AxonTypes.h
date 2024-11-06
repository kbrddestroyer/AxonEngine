#pragma once
#include <cstdint>
#include <ostream>

#if defined(__unix__) || __APPLE__
#include <memory>
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

    struct UDPMessage
    {
        char* data;
        size_t size;
        uint32_t tag;
    public:
        UDPMessage() = default;
        UDPMessage(const UDPMessage&) noexcept;
        ~UDPMessage() noexcept;

        static UDPMessage createUDPMessage(void*, size_t, uint32_t);
    };
}
