#include "AxonMessage.hpp"
#include <utility>
#include <memory.h>

#include "netconfig.h"
#include "backends/berkeley/basic_networking.h"

#pragma region SERIALIZED_AXON_MESSAGE

Networking::SerializedAxonMessage::SerializedAxonMessage(const AxonMessage &message) : 
    size(message.getSize())
{
    const TAG_T tag = generateTag(partID, flags);

    bytes = serialize(static_cast<char*>(message.getMessage()), message.getSize(), tag, &size);
}

Networking::SerializedAxonMessage::SerializedAxonMessage(const char* bits, size64_t actualSize) :
    size(actualSize)
{
    if (actualSize > 0) {
        bytes = new char[size];
        memcpy(const_cast<char *>(bytes), bits, size);
        TAG_T tag;
        size64_t actual;

        if (extractMetadata(
                bits, actualSize, &actual, &tag
        ) == 0) {
            partID = tag >> 8;
            flags = tag & ((1UL << 9) - 1);
        }
    }
}

Networking::SerializedAxonMessage::SerializedAxonMessage(const SerializedAxonMessage &message) :
    size(message.size),
    offset(message.offset),
    partID(message.partID),
    flags(message.flags)
{
    if (size > 0)
    {
        bytes = new char[size];
        memcpy(const_cast<char*>(bytes), message.bytes, size);
    }
}

Networking::SerializedAxonMessage::SerializedAxonMessage(SerializedAxonMessage &message, const size_t newSize, uintptr_t offset, uint8_t nPartID, uint8_t flagSet, uint64_t id) :
    size(newSize),
    uniqueID(id),
    offset(offset),
    partID(nPartID),
    flags(flagSet),
    bytes(message.bytes)
{
    message.owning = false;
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

std::unique_ptr<Networking::SerializedAxonMessage> Networking::SerializedAxonMessage::split() {
    if (size <= SYNAPSE_MESSAGE_SIZE_MAX) {
        return {};
    }

    size_t leftSize = size - SYNAPSE_MESSAGE_SIZE_MAX;
    this->size = SYNAPSE_MESSAGE_SIZE_MAX;
    addFlag(PARTIAL);

    return std::make_unique<SerializedAxonMessage>(
            *this,
            leftSize, offset, partID + 1, flags ^ PARTIAL, uniqueID
        );
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

    decompressTag(tag);
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

void Networking::AxonMessage::decompressTag(const TAG_T tag) {
    partID = tag >> 8;
    flags = tag & ((1UL << 9) - 1);
}

Networking::SerializedAxonMessage Networking::AxonMessage::getSerialized() const noexcept {
    return SerializedAxonMessage(*this);
}

#pragma endregion

/* AxonMessage.cpp */
