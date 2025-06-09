#include "AxonMessage.hpp"
#include <utility>
#include <cstring>

#pragma region SERIALIZED_AXON_MESSAGE

Networking::SerializedAxonMessage::SerializedAxonMessage(const AxonMessage &message) : 
    size(message.getSize())
{
    const TAG_T tag = compressTag(message.getPartID(), message.getFlags(), message.ID());
    const char *buffer = serialize(static_cast<char*>(message.getMessage()), message.getSize(), tag, &size);

    bytes = std::shared_ptr<const char>( buffer );
}

Networking::SerializedAxonMessage::SerializedAxonMessage(const char* bits, size64_t actualSize) :
    size(actualSize)
{
    if (actualSize > 0) {
        bytes = std::make_shared<const char>(size);
        memcpy(const_cast<char *>(bytes.get()), bits, size);
    }
}

TAG_T Networking::SerializedAxonMessage::compressTag(uint8_t optionalData, uint8_t newFlags, uint16_t id) {
    return ((optionalData << 8) | newFlags) << 16 | id;
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
    uint8_t _ret = deserialize(
            serialized.bytes.get(),
            serialized.size,
            &this->message,
            &this->size,
            &tag
    );

    if (_ret == 0)
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

Networking::AxonMessage::AxonMessage(AxonMessage &message, size64_t size, uint8_t partID, uint8_t flags, uint64_t uniqueID, size64_t offset) :
    size(size),
    message(message.message),
    partID(partID),
    flags(flags),
    uniqueID(uniqueID),
    offset(offset)
{
    message.owning = false;
}

Networking::AxonMessage::~AxonMessage()
{
	if (this->message && owning)
		delete[] static_cast<char*>( message );
    message = nullptr;
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
        if (owning)
            delete[] static_cast<char*>(message);
        message = nullptr;
    }
    memcpy((static_cast<char*>(tempBuffer) + size), other.getMessage(), other.size);

    message = tempBuffer;
    size += other.size;
    partID = other.partID;
}

#pragma endregion

/* AxonMessage.cpp */
