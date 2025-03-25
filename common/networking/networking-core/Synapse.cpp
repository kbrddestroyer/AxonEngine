#include "Synapse.hpp"
#include <cstdio>


#pragma region SYNAPS
Networking::Synapse::Synapse(uint32_t port)
{
	isServer = true;
	info.port = port;

	if (int code = create_udp_server(&socket_info, &socket, port) != SUCCESS)
	{
		throw AxonNetworkingInternalError(code);
	}
}

Networking::Synapse::Synapse(const ConnectionInfo& connection)
{
	isServer = false;
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

Networking::Synapse::~Synapse()
{
	isAlive = false;
	finalize_udp(socket);
}

void Networking::Synapse::start()
{
	isAlive = true;
	listen();
}

void Networking::Synapse::send(const AxonMessage& message)
{
	if (!isServer)
		sendTo(message, &socket_info);
}

void Networking::Synapse::sendTo(const AxonMessage& message, const SOCKADDR_IN_T* info)
{
	send_udp_message(
		message.getSerializedBuffer(), 
		message.getSerializedSize(), 
		socket, 
		info
	);
}

void Networking::Synapse::listen()
{
	char* buffer[SYNAPSE_MESSAGE_MAX_SIZE];
	SOCKADDR_IN_T host;
	while (isAlive)
	{
		int32_t size = recv_udp_message(reinterpret_cast<char**> (&buffer), SYNAPSE_MESSAGE_MAX_SIZE, socket, &host);
		if (size > 0)
		{
			const char* message = const_cast<char*>(reinterpret_cast<char*> (buffer));
			AxonMessage message_ = AxonMessage(message, size);
			onMessageReceived(message_, &host);
		}
	}
}

void Networking::Synapse::onMessageReceived(const AxonMessage& message, SOCKADDR_IN_T* from)
{
	// Notify
	SynapseMessageReceivedEvent event_ = SynapseMessageReceivedEvent(message, from);
	events.invoke(&event_);
}
#pragma endregion


#pragma region ASYNC_SYNAPS

Networking::AsyncSynapse::~AsyncSynapse()
{
	kill();
}

void Networking::AsyncSynapse::start()
{
	isAlive = true;
	proc = std::thread(&Networking::AsyncSynapse::listen, this);
}

void Networking::AsyncSynapse::kill()
{
	isAlive = false;
	proc.join();
}

#pragma endregion
