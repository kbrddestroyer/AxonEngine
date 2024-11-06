#include "AxonTypes.h"


Axon::Connection::UDPMessage::UDPMessage(const Axon::Connection::UDPMessage& other) noexcept
{
    this->size = other.size;
    this->tag = other.tag;

    this->data = new char[size];
    memcpy(data, other.data, size);
}

Axon::Connection::UDPMessage::~UDPMessage() noexcept {
    delete[] data;
}

Axon::Connection::UDPMessage Axon::Connection::UDPMessage::createUDPMessage(void* data, size_t size, uint32_t tag)
{
    UDPMessage message;

    message.size = size;
    message.data = new char[message.size];

    memcpy(message.data, data, message.size);
    
    message.tag = tag;

    return message;
}
