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
    if (bytes)
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

	this->message = new char[size];
	memcpy(this->message, message, size);
}

Networking::AxonMessage::AxonMessage(const SerializedAxonMessage &serialized)
{
    TAG_T tag;
    deserialize(
            serialized.bytes,
            serialized.size,
            reinterpret_cast<void**>(&this->message),
            &this->size,
            &tag
    );
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

	this->message = new char[size];
	memcpy(this->message, message.getMessage(), this->size);
}

Networking::AxonMessage::AxonMessage(AxonMessage &&other) noexcept :
    size(std::exchange(other.size, 0)),
    message(std::exchange(other.message, nullptr)),
    partID(std::exchange(other.partID, 0)),
    flags(std::exchange(other.flags, UNDEFINED)),
    uniqueID(std::exchange(other.uniqueID, 0))
{}

Networking::AxonMessage::AxonMessage(const AxonMessage &message, size64_t size, uint8_t partID, uint8_t flags, uint64_t uniqueID, size64_t offset) :
    size(size),
    partID(partID),
    flags(flags),
    uniqueID(uniqueID)
{
    if (!message.message || size == 0)
        return;

    this->message = new char[size];
    memcpy(this->message, message.message + offset, size);
}

Networking::AxonMessage::~AxonMessage()
{
	if (this->message)
		delete[] message;
    message = nullptr;
}

Networking::AxonMessage & Networking::AxonMessage::operator=(const AxonMessage &other) {
    if (&other == this) {
        return *this;
    }

    if (this->message) {
        delete[] this->message;
    }

    size = other.size;
    partID = other.partID;
    flags = other.flags;
    uniqueID = other.uniqueID;

    if (other.message && other.size > 0) {
        message = new char[size];
        memcpy(message, other.message, size);
    }

    return *this;
}

Networking::AxonMessage & Networking::AxonMessage::operator=(AxonMessage &&other) noexcept {
    if (&other == this) {
        return *this;
    }

    if (this->message) {
        delete[] message;
    }

    size = std::exchange(other.size, 0);
    message = std::exchange(other.message, nullptr);
    partID = std::exchange(other.partID, 0);
    flags = std::exchange(other.flags, UNDEFINED);
    uniqueID = std::exchange(other.uniqueID, 0);

    return *this;
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

void Networking::AxonMessage::append(const AxonMessage &other) {
    if (!other.getMessage() || other.size == 0 || other.partID != partID + 1)
        return;

    char* tempBuffer = new char[other.size + size];
    if (message) {
        memcpy(tempBuffer, getMessage(), size);
        delete[] message;
        message = nullptr;
    }

    memcpy(tempBuffer + size, other.getMessage(), other.size);
    message = tempBuffer;
    size += other.size;
    partID = other.partID;
}

#pragma endregion

/* AxonMessage.cpp */
