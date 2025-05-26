#include "AxonMessage.hpp"
#include <utility>
#include <memory.h>

#include "netconfig.h"
#include "backends/berkeley/basic_networking.h"

#pragma region SERIALIZED_AXON_MESSAGE

Networking::SerializedAxonMessage::SerializedAxonMessage(const AxonMessage &message) : 
    size(message.getSize())
{
    const TAG_T tag = generateTag(message.getPartID(), message.getFlags());
    bytes = serialize(static_cast<char*>(message.getMessage()), message.getSize(), tag, &size);
}

Networking::SerializedAxonMessage::SerializedAxonMessage(const char* bits, size64_t actualSize) :
    size(actualSize)
{
    if (actualSize > 0) {
        bytes = new char[size];
        memcpy(const_cast<char *>(bytes), bits, size);
    }
}

Networking::SerializedAxonMessage::SerializedAxonMessage(const SerializedAxonMessage &message) :
    size(message.size),
    offset(message.offset)
{
    if (size > 0)
    {
        bytes = new char[size];
        memcpy(const_cast<char*>(bytes), message.bytes, size);
    }
}

Networking::SerializedAxonMessage::SerializedAxonMessage(SerializedAxonMessage &&message) noexcept :
        size(std::exchange(message.size, 0)),
        bytes(std::exchange(message.bytes, nullptr))
{}

Networking::SerializedAxonMessage::~SerializedAxonMessage() {
    if (bytes && owning)
        delete[] bytes;
}

TAG_T Networking::SerializedAxonMessage::generateTag(const uint8_t optionalData, const uint8_t newFlags) {
    return ((optionalData << 8) | newFlags);
}

Networking::SerializedAxonMessage &
Networking::SerializedAxonMessage::operator=(const SerializedAxonMessage &message) {
    if (&message == this)
        return *this;

    size = message.size;

    if (size > 0)
    {
        bytes = new char[size];
        memcpy(const_cast<char*>(bytes), message.bytes, size);
    }

    return * this;
}

Networking::SerializedAxonMessage &
Networking::SerializedAxonMessage::operator=(SerializedAxonMessage &&message) noexcept {
    if (&message == this)
        return *this;

    std::exchange(size, message.size);
    std::exchange(bytes, message.bytes);

    return * this;
}

#pragma endregion

#pragma region AxonMessage

Networking::AxonMessage::AxonMessage(const void* message, size64_t size, uint8_t partID, uint8_t flags) :
    size(size),
    partID(partID),
    flags(flags)
{
	if (message == nullptr || size == 0)
		return;

	this->message = new char[size];
	memcpy(this->message, message, size);
}

Networking::AxonMessage::AxonMessage(const SerializedAxonMessage &serialized) :
    flags()
{
    TAG_T tag;

    deserialize(
            serialized.bytes,
            serialized.size,
            &this->message,
            &this->size,
            &tag
    );

    decompressTag(tag, &partID, &flags);
}

Networking::AxonMessage::AxonMessage(const AxonMessage& message) :
    size(message.size),
    partID(message.partID),
    flags(message.flags)
{
    if (!message.message || message.size == 0)
        return;

	this->message = new char[size];
	memcpy(this->message, message.message, this->size);
}

Networking::AxonMessage::~AxonMessage()
{
	if (this->message)
		delete[] static_cast<char*>( message );
}

Networking::SerializedAxonMessage Networking::AxonMessage::getSerialized() const noexcept {
    return SerializedAxonMessage(*this);
}

void Networking::AxonMessage::decompressTag(
        const TAG_T tag,
        uint8_t *partID,
        uint8_t *flags
) {
    *partID = tag >> 8;
    *flags = tag & ((1UL << 9) - 1);
}
#pragma endregion

/* AxonMessage.cpp */
