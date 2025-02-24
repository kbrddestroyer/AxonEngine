#include "AxonMessage.hpp"


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

/* AxonMessage.cpp */
