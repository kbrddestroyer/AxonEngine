
#include "AxonTypes.h"

Axon::Connection::UDPMessage::UDPMessage() {
    this->data = nullptr;
    this->size = 0;
    this->tag  = -1;
}

Axon::Connection::UDPMessage::UDPMessage(Axon::Connection::UDPMessage& other) {
    this->size = other.size;
    this->tag = other.tag;

    this->data = new char[size];
    memcpy(data, other.data, size);
}

Axon::Connection::UDPMessage::~UDPMessage() {
    delete[] data;
}

void Axon::Connection::UDPMessage::createUDPMessage(Axon::Connection::UDPMessage& message, void* data, size_t size, uint32_t tag)
{
    message.size = size;
    message.data = new char[message.size];

    memcpy(message.data, data, message.size);
    
    message.tag = tag;
}

Axon::Connection::UDPMessage& Axon::Connection::UDPMessage::operator=(const Axon::Connection::UDPMessage& other) {
    if (this != &other) {
        delete[] data;

        size = other.size;
        tag = other.tag;

        data = new char[size];
        memcpy(data, other.data, size);
    }
    return *this;
}