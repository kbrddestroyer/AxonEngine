#pragma once
#include <cstdint>

namespace Axon::Connection {
    typedef uint16_t AXON_PORT;

    typedef uint64_t UDPMessageTag;

    struct UDPMessage
    {
        UDPMessageTag   tag;
        void*           data;
        uint32_t        size;
    };
}