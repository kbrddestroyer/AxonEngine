#include "AxonNetwork.hpp"
#include <backends/backend.hpp>


#pragma region AXON_NETWORKING_INTERNAL_ERROR
Networking::AxonNetworkingInternalError::AxonNetworkingInternalError(uint8_t code)
{
	this->err = code;
}


uint8_t Networking::AxonNetworkingInternalError::code()
{
	return err;
}
#pragma endregion


#pragma region SYNAPS
Networking::Synaps::Synaps(uint32_t port)
{
	isServer = true;
	info.port = port;

	local = nullptr;
	remote = nullptr;
}

Networking::Synaps::Synaps(const ConnectionInfo& connection)
{
	isServer = false;
	info.hostname = connection.hostname;
	info.port = connection.port;

	if (int code = connect_udp_client(
		remote,
		local,
		info.hostname.c_str(),
		info.port
	))
	{
		throw AxonNetworkingInternalError(code);
	}
}

void Networking::Synaps::send(AxonMessage& message)
{
	send_udp_message(message.getMessage(), message.getSize(), *local, remote);
}

void Networking::Synaps::listen()
{
	// Listen function impl
	// Separate thread? 
}

void Networking::Synaps::onMessageReceived(const AxonMessage& message, SOCKADDR_IN_T* from)
{
	// Notify
}
#pragma endregion


/* AxonNetwork.cpp */
