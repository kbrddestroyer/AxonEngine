#include "AxonMessage.hpp"
#include <serialization/serialization.hpp>


#pragma region AxonMessage

Networking::AxonMessage::AxonMessage(const char* serialized, size_t size)
{
	this->serialized = serialized;
	this->size = size;
}

Networking::AxonMessage::AxonMessage(const AxonMessage& message)
{
	this->serialized = message.serialized;
	this->size = message.size;
}

#pragma endregion

#pragma region Message

Networking::Message::Message(const char* bitstream, size_t size, uint32_t tag)
{
	this->bitstream = new char[size];
	memcpy(this->bitstream, bitstream, size);

	this->size = size;
	this->tag = tag;
}

Networking::Message::Message(const AxonMessage& message)
{
	this->bitstream = new char[message.getSize()];
	deserialize(
		const_cast<char*>(message.getMessage()),
		message.getSize(),
		reinterpret_cast<char*>(this->bitstream),
		&this->size,
		&this->tag
	);
}

Networking::Message::~Message()
{
	if (this->bitstream)
	{
		delete[] this->bitstream;
	}
}

Networking::AxonMessage Networking::Message::toMessage()
{
	size_t total;
	char* serialized = serialize(
		reinterpret_cast<char*>(this->bitstream),
		this->size,
		this->tag,
		&total
	);

	char data[1024];
	memcpy(data, serialized, total);
	free(serialized);
	return AxonMessage(data, total);
}

#pragma endregion

/* AxonMessage.cpp */
