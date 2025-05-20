#include "AxonMessage.hpp"
#include <utility>

#include <SynapseConfig.h>

#pragma region SERIALIZED_AXON_MESSAGE

Networking::SerializedAxonMessage::SerializedAxonMessage(const char *raw, size_t size) :
    size(size)
{
    if (size == 0)
        return;

    bitstream = new char[size];
    memcpy(const_cast<char*>(bitstream), raw, size);
}

Networking::SerializedAxonMessage::SerializedAxonMessage(const AxonMessage &message) {
    TAG_T tag = generateTag(message.getPartID(), message.getFlags());

    bitstream = serialize(static_cast<char*>(message.getMessage()), message.getSize(), tag, &size);
}

Networking::SerializedAxonMessage::SerializedAxonMessage(const SerializedAxonMessage &message) :
    size(message.size)
{
    if (size > 0) {
        bitstream = new char[size];
        memcpy(const_cast<char*>(bitstream), message.bitstream, size);
    }
}

Networking::SerializedAxonMessage::SerializedAxonMessage(SerializedAxonMessage &message, const size_t newSize, uintptr_t offset) :
    size(newSize),
    offset(offset),
    bitstream(message.bitstream)
{
    message.owning = false;
}

Networking::SerializedAxonMessage::SerializedAxonMessage(SerializedAxonMessage &&message) noexcept :
    size(std::exchange(message.size, 0)),
    bitstream(std::exchange(message.bitstream, nullptr))
{}

Networking::SerializedAxonMessage::~SerializedAxonMessage() {
    if (bitstream && owning)
        delete[] bitstream;
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
        bitstream = new char[size];
        memcpy(const_cast<char*>(bitstream), message.bitstream, size);
    }

    return * this;
}

Networking::SerializedAxonMessage &
Networking::SerializedAxonMessage::operator=(SerializedAxonMessage &&message) noexcept {
    if (&message == this)
        return *this;

    std::exchange(size, message.size);
    std::exchange(bitstream, message.bitstream);

    return * this;
}

Networking::SerializedAxonMessage Networking::SerializedAxonMessage::split() {
    size_t sendSize = SYNAPSE_MESSAGE_SIZE_MAX;

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
            serialized.bitstream,
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

Networking::SerializedAxonMessage Networking::AxonMessage::getSerialized() const {
    return SerializedAxonMessage(*this);
}

uint64_t Networking::AxonMessage::getUniqueID() {
    static uint64_t uniqueID;
    return uniqueID++;
}

#pragma endregion

/* AxonMessage.cpp */
