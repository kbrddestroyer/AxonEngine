#include "AxonMessage.hpp"
#include <serialization/serialization.hpp>
#include <assert.h>


#pragma region AxonMessage

Networking::AxonMessage::AxonMessage(void* message, size_t size, uint32_t tag)
{
	if (message == nullptr)
		return;

	this->message = calloc(size, sizeof(char));
	
	if (this->message == nullptr)
		return;  // Could not alloc

	memcpy(this->message, message, size);
	
	this->size = size;
	this->tag = tag;

	this->serialized = serialize(reinterpret_cast<char*>(message), size, tag, &this->serializedSize);
}

Networking::AxonMessage::AxonMessage(const char* bits, size_t actualSize)
{
	this->serialized = calloc(actualSize, sizeof(char));
	if (this->serialized == nullptr)
		return; 

	this->serializedSize = actualSize;

	memcpy(this->serialized, bits, actualSize);

	this->message = nullptr;
	this->size = this->tag = 0;

	deserialize(
		bits,
		actualSize,
		&this->message,
		&this->size,
		&this->tag
	);
}

Networking::AxonMessage::AxonMessage(const AxonMessage& message)
{
	this->size = message.size;
	this->tag = message.tag;
	this->serializedSize = message.serializedSize;

	this->serialized = calloc(this->serializedSize, sizeof(char));
	this->message = calloc(this->size, sizeof(char));

	if (!this->message || !this->serialized)
		return;

	memcpy(this->message, message.message, this->size);
	memcpy(this->serialized, message.serialized, this->serializedSize);
}

Networking::AxonMessage::~AxonMessage()
{
	if (this->serialized)
		free(this->serialized);
	if (this->message)
		free(this->message);
}

#pragma endregion

/* AxonMessage.cpp */
