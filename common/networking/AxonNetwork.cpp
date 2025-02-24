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
	isAlive = true;

	if (int code = create_udp_server(&socket_info, &socket, port) != SUCCESS)
	{
		throw AxonNetworkingInternalError(code);
	}
}

Networking::Synaps::Synaps(const ConnectionInfo& connection)
{
	isServer = false;
	isAlive = true;
	info.hostname = connection.hostname;
	info.port = connection.port;

	if (int code = connect_udp_client(
		&socket_info,
		&socket,
		info.hostname.c_str(),
		info.port
	) != SUCCESS)
	{
		throw AxonNetworkingInternalError(code);
	}
}

Networking::Synaps::~Synaps()
{
	isAlive = false;
	finalize_udp(socket);
}

void Networking::Synaps::send(AxonMessage& message)
{
	send_udp_message(message.getMessage(), message.getSize(), socket, &socket_info);
}

void Networking::Synaps::listen()
{
	// Listen function impl
	// Separate thread?

	char* buffer[1024];
	SOCKADDR_IN_T host;
	while (isAlive)
	{
		if (size_t size = recv_udp_message(reinterpret_cast<char**> (&buffer), 1024, socket, &host) > 0)
		{
			const char* message = const_cast<char*>(reinterpret_cast<char*> (buffer));
			onMessageReceived(AxonMessage(message, size), &host);
		}
	}
}

void Networking::Synaps::onMessageReceived(AxonMessage& message, SOCKADDR_IN_T* from)
{
	// Notify
	events.invoke(&SynapsMessageReceivedEvent(message, from));
}
#pragma endregion

/* AxonNetwork.cpp */
