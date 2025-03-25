#include "Synapse.hpp"
#include <cstdio>


#pragma region SYNAPS
Networking::Synapse::Synapse(uint32_t port, ConnectionMode connectionMode)
{
	isServer = true;

	info.port = port;
	info.mode = connectionMode;

	initializeFromConnectionMode();
}

Networking::Synapse::Synapse(const ConnectionInfo& connection)
{
	isServer = false;
	info.hostname = connection.hostname;
	info.port = connection.port;
	info.mode = connection.mode;

	initializeFromConnectionMode();
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

void Networking::Synapse::sendTo(const AxonMessage& message, const SOCKADDR_IN_T* dest) const
{
	if (info.mode == ConnectionMode::UDP)
	{
		send_udp_message(
				message.getSerializedBuffer(),
				message.getSerializedSize(),
				socket,
				dest
		);
	}
	else if (info.mode == ConnectionMode::TCP)
	{
		send_tcp_message(
				message.getSerializedBuffer(),
				message.getSerializedSize(),
				socket
				);
	}
}

void Networking::Synapse::listen()
{
	char *buffer[SYNAPSE_MESSAGE_MAX_SIZE] = {};
	SOCKADDR_IN_T host = {};

	if (info.mode == ConnectionMode::UDP) {
		while (isAlive) {
			int32_t size = recv_udp_message(reinterpret_cast<char **> (&buffer), SYNAPSE_MESSAGE_MAX_SIZE, socket,
											&host);
			if (size > 0) {
				const char *message = const_cast<char *>(reinterpret_cast<char *> (buffer));
				AxonMessage message_ = AxonMessage(message, size);
				onMessageReceived(message_, &host);
			}
		}
	}
	else if (info.mode == ConnectionMode::TCP)
	{
		SOCKET_T client;
		do {
			client = accept_incoming(socket, &host);
		} while (!CHECK_VALID(client));

		while (isAlive)
		{
			int size = recv_tcp_message((char**) buffer, 256, client);
			if (size > 0)
			{
				const char *message = const_cast<char *>(reinterpret_cast<char *> (buffer));
				AxonMessage message_ = AxonMessage(message, size);
				onMessageReceived(message_, &host);
			}
		}
	}
}

void Networking::Synapse::onMessageReceived(const AxonMessage& message, SOCKADDR_IN_T* from)
{
	// Notify
	SynapseMessageReceivedEvent event_ = SynapseMessageReceivedEvent(message, from);
	events.invoke(&event_);
}

void Networking::Synapse::initializeFromConnectionMode() {
	if (info.mode == ConnectionMode::TCP)
	{
		if (isServer)
		{
			create_tcp_server(&socket_info, &socket, info.port);
		}
		else
		{
			if (int code = connect_tcp_client(
					&socket_info,
					&socket,
					info.hostname.c_str(),
					info.port
			) != SUCCESS)
			{
				throw AxonNetworkingInternalError(code);
			}
		}
	}
	else if (info.mode == ConnectionMode::UDP)
	{
		if (isServer)
		{
			create_udp_server(&socket_info, &socket, info.port);
		}
		else {
			if (int code = connect_udp_client(
					&socket_info,
					&socket,
					info.hostname.c_str(),
					info.port
			) != SUCCESS) {
				throw AxonNetworkingInternalError(code);
			}
		}
	}
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
