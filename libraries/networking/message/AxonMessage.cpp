#include "AxonMessage.hpp"
#include <memory.h>
#include <utility>


#pragma region SERIALIZED_AXON_MESSAGE

Networking::SerializedAxonMessage::SerializedAxonMessage(const char *raw, size_t size) :
    size(size)
{
    if (size == 0)
        return;

    bitstream = new char[size];
    memcpy((void*) bitstream, raw, size);
}

Networking::SerializedAxonMessage::SerializedAxonMessage(const Networking::AxonMessage &message) {
    TAG_T tag = generateTag(message.getPartID(), message.getFlags());

    bitstream = serialize(static_cast<char*>(message.getMessage()), message.getSize(), tag, &size);
}

Networking::SerializedAxonMessage::SerializedAxonMessage(const Networking::SerializedAxonMessage &message) :
    size(message.size)
{
    if (size > 0) {
        bitstream = new char[size];
        memcpy((void*) bitstream, message.bitstream, size);
    }
}

Networking::SerializedAxonMessage::SerializedAxonMessage(Networking::SerializedAxonMessage &message, size_t newSize, uintptr_t offset) :
    size(newSize),
    bitstream(message.bitstream),
    offset(offset)
{
    message.owning = false;
}

Networking::SerializedAxonMessage::SerializedAxonMessage(Networking::SerializedAxonMessage &&message) noexcept :
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
Networking::SerializedAxonMessage::operator=(const Networking::SerializedAxonMessage &message) {
    if (&message == this)
        return *this;

    size = message.size;

    if (size > 0)
    {
        bitstream = new char[size];
        memcpy((void*) bitstream, message.bitstream, size);
    }

    return * this;
}

Networking::SerializedAxonMessage &
Networking::SerializedAxonMessage::operator=(Networking::SerializedAxonMessage &&message) noexcept {
    if (&message == this)
        return *this;

    std::exchange(size, message.size);
    std::exchange(bitstream, message.bitstream);

    return * this;
}

#pragma endregion

#pragma region AxonMessage

Networking::AxonMessage::AxonMessage(void* message, size_t size, uint8_t partID, uint8_t flags) :
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

Networking::AxonMessage::AxonMessage(const Networking::SerializedAxonMessage &serialized) :
    message(),
    size(),
    partID(),
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
