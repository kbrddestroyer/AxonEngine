#pragma once
#include <cstdint>
#include <ostream>

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
        UserMessage contents{};
    public:
        UDPMessage();
        explicit UDPMessage(void*, size_t, uint64_t);

        ~UDPMessage();

        std::shared_ptr<char[]> getSerializedData(size_t&) const;
        void setDeserialized(const std::shared_ptr<char[]>&, size_t);
    };
}
