#include "Synaps.hpp"


#pragma region SYNAPS
Networking::Synaps::Synaps(uint32_t port)
{
	isServer = true;
	info.port = port;

	if (int code = create_udp_server(&socket_info, &socket, port) != SUCCESS)
	{
		throw AxonNetworkingInternalError(code);
	}
}

Networking::Synaps::Synaps(const ConnectionInfo& connection)
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

Networking::Synaps::~Synaps()
{
	isAlive = false;
	finalize_udp(socket);
}

void Networking::Synaps::start()
{
	isAlive = true;
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
			AxonMessage message_ = AxonMessage(message, size);
			onMessageReceived(message_, &host);
		}
	}
}

void Networking::Synaps::onMessageReceived(AxonMessage& message, SOCKADDR_IN_T* from)
{
	// Notify
	SynapsMessageReceivedEvent event_ = SynapsMessageReceivedEvent(message, from);
	events.invoke(&event_);
}
#pragma endregion


#pragma region ASYNC_SYNAPS

Networking::AsyncSynaps::~AsyncSynaps()
{
	kill();
	proc.join();	// Correctly kill Synaps in main thread
}

void Networking::AsyncSynaps::start()
{
	isAlive = true;
	proc = std::thread(&Networking::AsyncSynaps::listen, this);
}

void Networking::AsyncSynaps::kill()
{
	isAlive = false;
}

#pragma endregion
