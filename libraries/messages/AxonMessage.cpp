#include "AxonMessage.hpp"
#include <utility>
#include <cstring>

#pragma region SERIALIZED_AXON_MESSAGE

Networking::SerializedAxonMessage::SerializedAxonMessage(const AxonMessage &message) : 
    size(message.getSize())
{
    const TAG_T tag = compressTag(message.getPartID(), message.getFlags(), message.ID());
    bytes = std::shared_ptr<char[]> ( serialize(static_cast<char*>(message.getMessage()), message.getSize(), tag, &size) );
}

Networking::SerializedAxonMessage::SerializedAxonMessage(const char* bits, size64_t actualSize) :
    size(actualSize)
{
    if (actualSize > 0) {
        bytes = std::shared_ptr<char[]> ( new char[size] );
        memcpy(bytes.get(), bits, size);
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

	this->message = std::shared_ptr<char[]>(new char[size]);
	memcpy(this->message.get(), message, size);
}

Networking::AxonMessage::AxonMessage(const SerializedAxonMessage &serialized)
{
    TAG_T tag;

    void * buffer;

    deserialize(
            serialized.bytes.get(),
            serialized.size,
            &buffer,
            &this->size,
            &tag
    );

    message = std::shared_ptr<char[]>(static_cast<char*>(buffer));
    decompressTag(tag, &partID, &flags, &this->uniqueID);
}

Networking::AxonMessage::AxonMessage(const AxonMessage &message, const uint8_t additionalFlags) :
    flags(additionalFlags),
    uniqueID(message.uniqueID)
{
    addFlag(ACKNOWLEDGE);
}

Networking::AxonMessage::AxonMessage(const AxonMessage &message, size64_t size, uint8_t partID, uint8_t flags, uint64_t uniqueID, size64_t offset) :
    size(size),
    message(message.message),
    partID(partID),
    flags(flags),
    uniqueID(uniqueID),
    offset(offset)
{}

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
    }
    memcpy(tempBuffer + size, other.getMessage(), other.size);

    message = std::shared_ptr<char[]>(tempBuffer);
    size += other.size;
    partID = other.partID;
}

#pragma endregion

/* AxonMessage.cpp */
