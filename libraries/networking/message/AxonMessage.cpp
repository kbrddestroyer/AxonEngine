#include "AxonMessage.hpp"
#include <utility>
#include <memory.h>

#include "netconfig.h"
#include "backends/berkeley/basic_networking.h"

#pragma region SERIALIZED_AXON_MESSAGE


Networking::AxonMessage::AxonMessage(void* message, size64_t size, uint8_t partID, uint8_t flags) :
size(size)
{
    if (size == 0)
        return;
    bytes = new char[size];
    memcpy(const_cast<char*>(bytes), raw, size);
}

Networking::SerializedAxonMessage::SerializedAxonMessage(const AxonMessage &message) : 
size(message.size)
{
    TAG_T tag = generateTag(message.getPartID(), message.getFlags());

    bytes = serialize(static_cast<char*>(message.getMessage()), message.getSize(), tag, &size);
}

Networking::AxonMessage::AxonMessage(const char* bits, size64_t actualSize)
  
{
    if (size > 0) {
        bytes = new char[size];
        memcpy(const_cast<char*>(bytes), message.bytes, size);
    }
}

Networking::SerializedAxonMessage::SerializedAxonMessage(SerializedAxonMessage &message, const size_t newSize, uintptr_t offset, uint8_t nPartID, uint8_t flagSet, uint64_t id) :
        size(newSize),
        offset(offset),
        bytes(message.bytes),
        partID(nPartID),
        flags(flagSet),
        uniqueID(id)
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
    size = SYNAPSE_MESSAGE_SIZE_MAX;
    addFlag(PARTIAL);

    return std::make_unique<SerializedAxonMessage>(
            *this,
            leftSize, offset, partID + 1, flags ^ PARTIAL, size
        );
}

#pragma endregion

#pragma region AxonMessage

Networking::AxonMessage::AxonMessage(const void* message, size_t size, uint8_t partID, uint8_t flags) :
    size(size),
    partID(partID),
    flags(flags),
    uniqueID(getUniqueID())
{
	if (message == nullptr || size == 0)
		return;

	this->message = new char[size];
	memcpy(this->message, message, size);
}

Networking::AxonMessage::AxonMessage(const SerializedAxonMessage &serialized) :
    flags(),
    uniqueID(getUniqueID())
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
    flags(message.flags),
    uniqueID(getUniqueID())
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

Networking::AxonMessage Networking::AxonMessage::split(const size64_t sendSize) {
    if (sendSize >= size)
        throw 1;
    uintptr_t pMessage = reinterpret_cast<uintptr_t> (message);

    uint8_t tag = TAG_FLAGS::PARTIAL;

    return Networking::AxonMessage(reinterpret_cast<void*> (pMessage + sendSize), size - sendSize, TAG_FLAGS::FINISH);
}

Networking::SerializedAxonMessage Networking::AxonMessage::getSerialized() const {
    return SerializedAxonMessage(*this);
}

uint64_t Networking::AxonMessage::getUniqueID() {
    static uint64_t uniqueID;
    return uniqueID++;
}

#pragma endregion

/* AxonMessage.cpp */
