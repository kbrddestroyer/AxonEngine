#include "AxonMessage.hpp"
#include <utility>
#include <cstring>

#pragma region SERIALIZED_AXON_MESSAGE

Networking::SerializedAxonMessage::SerializedAxonMessage(const AxonMessage &message) : 
    size(message.getSize())
{
    const TAG_T tag = compressTag(message.getPartID(), message.getFlags(), message.ID());
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

Networking::SerializedAxonMessage::SerializedAxonMessage(const SerializedAxonMessage &message)
{
    this->copy(message);
}

Networking::SerializedAxonMessage::SerializedAxonMessage(SerializedAxonMessage &&message) noexcept
{
    this->move(message);
}

Networking::SerializedAxonMessage::~SerializedAxonMessage() {
    if (bytes && owning)
        delete[] bytes;
}

TAG_T Networking::SerializedAxonMessage::compressTag(uint8_t optionalData, uint8_t newFlags, uint16_t id) {
    return ((optionalData << 8) | newFlags) << 16 | id;
}

Networking::SerializedAxonMessage &
Networking::SerializedAxonMessage::operator=(const SerializedAxonMessage &message) {
    if (&message == this)
        return *this;

    this->copy(message);
    return * this;
}

Networking::SerializedAxonMessage &
Networking::SerializedAxonMessage::operator=(SerializedAxonMessage &&message) noexcept {
    if (&message == this)
        return *this;

    this->move(message);
    return * this;
}

void Networking::SerializedAxonMessage::copy(const Networking::SerializedAxonMessage &message) {
    size = message.size;
    if (size > 0)
    {
        bytes = new char[size];
        memcpy(const_cast<char*>(bytes), message.bytes, size);
    }
}

void Networking::SerializedAxonMessage::move(Networking::SerializedAxonMessage &message) noexcept {
    size = std::exchange(message.size, 0);
    bytes = std::exchange(message.bytes, nullptr);
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

    this->message = new MessageHolder;
	this->message->messagePtr = new char[size];
	memcpy(this->message->messagePtr, message, size);
    this->message->references ++;
}

Networking::AxonMessage::AxonMessage(const SerializedAxonMessage &serialized)
{
    TAG_T tag;
    this->message = new MessageHolder;

    deserialize(
            serialized.bytes,
            serialized.size,
            reinterpret_cast<void**>(&this->message->messagePtr),
            &this->size,
            &tag
    );
    this->message->references ++;
    decompressTag(tag, &partID, &flags, &this->uniqueID);
}

Networking::AxonMessage::AxonMessage(const AxonMessage &message, const uint8_t additionalFlags) :
    flags(additionalFlags),
    uniqueID(message.uniqueID)
{
    addFlag(ACKNOWLEDGE);
}

Networking::AxonMessage::AxonMessage(const AxonMessage& message) :
    size(message.size),
    partID(message.partID),
    flags(message.flags),
    uniqueID(message.uniqueID)
{
    if (!message.getMessage() || message.size == 0)
        return;

    this->message = message.message;
	this->message->references++;
}

Networking::AxonMessage::AxonMessage(AxonMessage &message, size64_t size, uint8_t partID, uint8_t flags, uint64_t uniqueID, size64_t offset) :
    size(size),
    message(message.message),
    partID(partID),
    flags(flags),
    uniqueID(uniqueID),
    offset(offset)
{
    this->message->references ++;
}

Networking::AxonMessage::~AxonMessage()
{
	if (this->message) {
        if (-- this->message->references == 0)
        {
            delete[] this->message->messagePtr;
            delete this->message;
        }
    }
}

Networking::SerializedAxonMessage Networking::AxonMessage::getSerialized() const noexcept {
    return SerializedAxonMessage(*this);
}

void Networking::AxonMessage::decompressTag(
        const TAG_T tag,
        uint8_t *partID,
        uint8_t *flags,
        uint16_t *id
) {
    uint32_t mask = 0xFFFF;
    *id = tag & mask;
    *partID = tag >> 16 >> 8;
    *flags = (tag >> 16) & ((1UL << 9) - 1);
}

Networking::AxonMessage::UniqueAxonMessagePtr Networking::AxonMessage::split(const size64_t toSend) {
    if (toSend >= size)
        return {};

    size64_t left = size - toSend;
    size = toSend;
    addFlag(PARTIAL);

    return std::make_unique<AxonMessage>(*this, left, partID + 1, flags ^ PARTIAL, uniqueID, size);
}

void Networking::AxonMessage::append(const Networking::AxonMessage &other) {
    if (!other.getMessage() || other.size == 0 || other.partID != partID + 1)
        return;

    char* tempBuffer = new char[other.size + size];

    // assert((uintptr_t) &size < (uintptr_t) tempBuffer || (uintptr_t) &size > (uintptr_t) tempBuffer + other.size + size);

    if (message) {
        memcpy(tempBuffer, getMessage(), size);

        if (-- this->message->references == 0)
        {
            delete[] this->message->messagePtr;
            delete this->message;
        }

        message = nullptr;
    }
    memcpy((static_cast<char*>(tempBuffer) + size), other.getMessage(), other.size);

    message = new MessageHolder;
    message->messagePtr = tempBuffer;
    message->references ++;
    size += other.size;
    partID = other.partID;
}

#pragma endregion

/* AxonMessage.cpp */
