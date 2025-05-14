#include "AxonMessage.hpp"
#include <assert.h>
#include <memory.h>


#pragma region AxonMessage

Networking::AxonMessage::AxonMessage(void* message, size_t size, uint8_t partID, uint8_t flags)
{
	if (message == nullptr)
		return;

	this->message = calloc(size, sizeof(char));
	
	if (this->message == nullptr)
        // TODO: Raise
		return;  // Could not alloc

	memcpy(this->message, message, size);
	
	this->size = size;
    this->partID = partID;
    this->flags = flags;
}

Networking::AxonMessage::AxonMessage(const char* bits, size_t actualSize)
{
    TAG_T tag;

	deserialize(
		bits,
		actualSize,
		&this->message,
		&this->size,
		&tag
	);

    decompressTag(tag);
}


Networking::AxonMessage::AxonMessage(const Networking::SerializedAxonMessage &serialized) {
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

Networking::AxonMessage::AxonMessage(const AxonMessage& message)
{
	this->size = message.size;
	this->message = calloc(this->size, sizeof(char));
    this->partID = message.partID;
    this->flags = message.flags;
    this->uniqueID = message.uniqueID;
	if (!this->message)
		return;

	memcpy(this->message, message.message, this->size);
}

Networking::AxonMessage::~AxonMessage()
{
	if (this->message)
		free(this->message);
}

TAG_T Networking::AxonMessage::generateTag(const uint8_t optionalData, const uint8_t newFlags) {
    return ((optionalData << 8) | newFlags);
}

void Networking::AxonMessage::decompressTag(const TAG_T tag) {
    partID = tag >> 8;
    flags = tag & ((1UL << 9) - 1);
}

Networking::AxonMessage Networking::AxonMessage::split(const size_t sendSize) {
    if (sendSize >= size)
        throw 1;
    uintptr_t pMessage = reinterpret_cast<uintptr_t> (message);

    uint8_t tag = TAG_FLAGS::PARTIAL;

    return Networking::AxonMessage(reinterpret_cast<void*> (pMessage + sendSize), size - sendSize, TAG_FLAGS::FINISH);
}

Networking::SerializedAxonMessage Networking::AxonMessage::getSerialized() const {
    Networking::SerializedAxonMessage serialized = {};
    TAG_T tag = generateTag(partID, flags);

    serialized.bitstream = serialize(static_cast<char*>(message), size, tag, &serialized.size);
    return serialized;
}

#pragma endregion

/* AxonMessage.cpp */
