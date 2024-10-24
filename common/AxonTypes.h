#pragma once
#include <cstdint>
#include <iostream>


namespace Axon {
    namespace Connection
    {
        typedef uint16_t AXON_PORT;

        typedef uint64_t UDPMessageTag;

        struct UDPMessage
        {
            UDPMessageTag   tag;
            void*           data;
            uint32_t        size;

        public:
            void serialize(char*, size_t&);
            static UDPMessage deserialize(char*);
        };
    }
}