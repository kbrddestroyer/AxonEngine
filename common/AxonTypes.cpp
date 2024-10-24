#include "AxonTypes.h"

void Axon::Connection::UDPMessage::serialize(char* buffer, size_t& total_size)
{
    size_t offset = 0;

    std::memcpy(buffer, &tag, sizeof(UDPMessageTag));
    offset += sizeof(UDPMessageTag);

    std::memcpy(buffer + offset, &size, sizeof(size));
    offset += sizeof(uint32_t);

    if (size > 0 && data != nullptr) {
        std::memcpy(buffer + offset, data, size);
        offset += size;
    }

    total_size = offset;
}

Axon::Connection::UDPMessage Axon::Connection::UDPMessage::deserialize(char* serialized)
{
    return {};
}
