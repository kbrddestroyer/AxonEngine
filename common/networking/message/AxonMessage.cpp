#include "AxonMessage.hpp"



/* AxonMessage.cpp */

Networking::AxonMessage::AxonMessage(const char* serialized, size_t size)
{
	this->serialized = serialized;
	this->size = size;
}
