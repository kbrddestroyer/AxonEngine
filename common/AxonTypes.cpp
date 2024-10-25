
#include "AxonTypes.h"

Axon::Connection::UDPMessage::UDPMessage() {
    this->contents = UserMessage();

    this->contents.data = nullptr;
    this->contents.size = 0;
    this->contents.tag  = -1;
}

Axon::Connection::UDPMessage::UDPMessage(void* data, size_t size, uint64_t tag)
{
    this->contents = UserMessage();

    this->contents.data = data;
    this->contents.size = size;
    this->contents.tag  = tag;
}

Axon::Connection::UDPMessage::~UDPMessage() {

}

std::shared_ptr<char[]> Axon::Connection::UDPMessage::getSerializedData(size_t & size) const {
    return {serialize(contents, &size), std::default_delete<char[]>()};
}

void Axon::Connection::UDPMessage::setDeserialized(const std::shared_ptr<char[]>& serialized, size_t size) {
    this->contents = deserialize(serialized.get(), size);
}
