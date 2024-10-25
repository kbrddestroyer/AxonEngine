
#include "AxonTypes.h"

Axon::Connection::UDPMessage::UDPMessage() {
    this->data = nullptr;
    this->size = 0;
    this->tag  = -1;
}

Axon::Connection::UDPMessage::UDPMessage(char* data, size_t size, uint64_t tag)
{
    this->data = data;
    this->size = size;
    this->tag  = tag;
}

Axon::Connection::UDPMessage::~UDPMessage() {

}

std::shared_ptr<char[]> Axon::Connection::UDPMessage::getSerializedData(size_t& datasize) const {
    char* serialized;
    if (serialize(data, size, tag, &serialized, &datasize) == 0)
    {
        return std::shared_ptr<char[]> (serialized);
    }
    return { nullptr };
}

void Axon::Connection::UDPMessage::setDeserialized(const std::shared_ptr<char[]>& serialized, size_t datasize) {
    if (deserialize(serialized.get(), datasize, &data, &size, &tag) != 0)
    {
        throw 1;
    }
}
