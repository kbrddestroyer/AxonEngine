#include "Synapse.hpp"


#pragma region SYNAPS

template <Networking::ConnectionMode mode>
Networking::Synapse<mode>::Synapse(uint32_t port)
{
	isServer = true;

	info.port = port;
	initializeFromConnectionMode();
}


template <Networking::ConnectionMode mode>
Networking::Synapse<mode>::Synapse(const ConnectionInfo& connection)
{
	isServer = false;
	info.hostname = connection.hostname;
	info.port = connection.port;

	initializeFromConnectionMode();
}


template <Networking::ConnectionMode mode>
Networking::Synapse<mode>::~Synapse()
{
	isAlive = false;
	finalize_udp(socket.socket);
}

template <Networking::ConnectionMode mode>
void Networking::Synapse<mode>::start()
{
	isAlive = true;
	listen();
}

template <Networking::ConnectionMode mode>
void Networking::Synapse<mode>::send(const AxonMessage& message)
{
	if (!isServer)
		sendTo(message, &socket.conn);
}

template <Networking::ConnectionMode mode>
void Networking::Synapse<mode>::sendTo(const AxonMessage& message, const SOCKADDR_IN_T* dest) const
{
	send_message<mode>(socket, message.getSerializedBuffer(), message.getSize());
}

template <Networking::ConnectionMode mode>
void Networking::Synapse<mode>::listen()
{
	char *buffer[SYNAPSE_MESSAGE_MAX_SIZE] = {};
	SOCKADDR_IN_T host = {};

	if (mode == ConnectionMode::UDP) {
		while (isAlive) {
			int32_t size = recv_udp_message(reinterpret_cast<char *> (buffer), SYNAPSE_MESSAGE_MAX_SIZE, socket.socket,
											&host);
			if (size > 0) {
				const char *message = const_cast<char *>(reinterpret_cast<char *> (buffer));
				AxonMessage message_ = AxonMessage(message, size);
				onMessageReceived(message_, &host);
			}
		}
	}
	else if (mode == ConnectionMode::TCP)
	{
		SOCKET_T client;
		do {
			client = accept_incoming(socket.socket, &host);
		} while (!CHECK_VALID(client));

		while (isAlive)
		{
			int size = recv_tcp_message((char*) buffer, 256, client);
			if (size > 0)
			{
				const char *message = const_cast<char *>(reinterpret_cast<char *> (buffer));
				AxonMessage message_ = AxonMessage(message, size);
				onMessageReceived(message_, &host);
			}
		}
	}
}

template <Networking::ConnectionMode mode>
void Networking::Synapse<mode>::onMessageReceived(const AxonMessage& message, SOCKADDR_IN_T* from)
{
	// Notify
	SynapseMessageReceivedEvent event_ = SynapseMessageReceivedEvent(message, from);
	events.invoke(&event_);
}

template <Networking::ConnectionMode mode>
void Networking::Synapse<mode>::initializeFromConnectionMode() {
	if (isServer)
	{
		uint8_t returnCode = initialize_server<mode>(socket, info.port);
	}
}

#pragma endregion


#pragma region ASYNC_SYNAPS

template <Networking::ConnectionMode mode>
Networking::AsyncSynapse<mode>::~AsyncSynapse()
{
	kill();
}

template <Networking::ConnectionMode mode>
void Networking::AsyncSynapse<mode>::start()
{
	isAlive = true;
	proc = std::thread(&Networking::AsyncSynapse<mode>::listen, this);
}

template <Networking::ConnectionMode mode>
void Networking::AsyncSynapse<mode>::kill()
{
	isAlive = false;
	proc.join();
}

#pragma endregion

namespace Networking {
    template
    class Synapse<ConnectionMode::UDP>;

    template
    class Synapse<ConnectionMode::TCP>;

    template
    class AsyncSynapse<ConnectionMode::UDP>;

    template
    class AsyncSynapse<ConnectionMode::TCP>;
}
