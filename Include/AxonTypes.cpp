#include "AxonTypes.h"



Axon::Connection::Message::Message(const Axon::Connection::Message& other) noexcept
{
    this->size = other.size;
    this->tag = other.tag;

    this->data = new char[size];
    memcpy(data, other.data, size);
}

Axon::Connection::Message::~Message() noexcept {
    delete[] data;
}

Axon::Connection::Message Axon::Connection::Message::createUDPMessage(void* data, size_t size, uint32_t tag)
{
    Message message;

    message.size = size;
    message.data = new char[message.size];

    memcpy(message.data, data, message.size);
    
    message.tag = tag;

    return message;
}
