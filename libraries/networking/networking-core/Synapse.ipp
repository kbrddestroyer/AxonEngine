#pragma once
#include <SynapseConfig.h>

namespace Networking {
    template<>
    void Synapse<ConnectionMode::TCP>::listen();
    template<>
    void Synapse<ConnectionMode::UDP>::listen();

    template class Synapse<ConnectionMode::UDP>;
    template class Synapse<ConnectionMode::TCP>;

    template class AsyncSynapse<ConnectionMode::UDP>;
    template class AsyncSynapse<ConnectionMode::TCP>;
}

#pragma region SYNAPSE

template <Networking::ConnectionMode mode>
Networking::Synapse<mode>::Synapse(uint32_t port)
{
    isServer = true;
    info.port = port;

    socket = {};

    uint8_t result = initialize_server<mode>(socket, port);

    if (result != SUCCESS) {
        throw AxonNetworkingInternalError(result);
    }
}


template <Networking::ConnectionMode mode>
Networking::Synapse<mode>::Synapse(const ConnectionInfo& connection)
{
    isServer = false;
    info.hostname = connection.hostname;
    info.port = connection.port;

    socket = {};

    uint8_t result = initialize_client<mode>(socket, connection.hostname.c_str(), connection.port);

    if (result != SUCCESS) {
        throw AxonNetworkingInternalError(result);
    }
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
void Networking::Synapse<mode>::sendPooled(const AxonMessage& message, const SOCKADDR_IN_T* dest) const {
    if (!dest)
        dest = &socket.conn;

    pool->push( { message.getSerialized(), *dest } );
}

template <Networking::ConnectionMode mode>
void Networking::Synapse<mode>::send(const AxonMessage& message)
{
    sendTo(message.getSerialized(), &socket.conn);
}

template <Networking::ConnectionMode mode>
void Networking::Synapse<mode>::sendTo(const SerializedAxonMessage& serialized, const SOCKADDR_IN_T* dest) const
{
    send_message<mode>({ socket.socket, *dest }, serialized.getBits(), serialized.getSize());
}

template<>
inline void Networking::Synapse<Networking::ConnectionMode::TCP>::listen()
{
    // Todo: async support

    if (isServer) {
        SOCKADDR_IN_T host = {};

        fd_set master;
        FD_ZERO(&master);
        SOCKET_T maxSocket = socket.socket;
        while (isAlive)
        {
            fd_set reads = master;
            if (!CHECK_VALID(select(maxSocket + 1, &reads, nullptr, nullptr, nullptr))) {
                return;
            }
            for (SOCKET_T connectionSock = 1; connectionSock <= maxSocket; ++connectionSock) {
                if (FD_ISSET(connectionSock, &reads)){
                    if (connectionSock == socket.socket) {
                        sockaddr_storage storage = {};
                        SOCKET_T client = accept_incoming(socket.socket, reinterpret_cast<SOCKADDR_IN *>(&storage));

                        if (!CHECK_VALID(client)) {
                            return;
                        }

                        FD_SET(client, &master);
                        if (client > maxSocket)
                            maxSocket = client;
                    }

                }
                else {
                    char *buffer[SYNAPSE_MESSAGE_SIZE_MAX] = {};
                    const int32_t size = recv_tcp_message(reinterpret_cast<char *>(buffer), 256, connectionSock);
                    if (size > 0)
                    {
                        const char *message = reinterpret_cast<char *> (buffer);
                        AxonMessage message_(SerializedAxonMessage(message, size));
                        onMessageReceived(message_, &host);
                    }
                }
            }
            update();
        }
    }
}

template<>
inline void Networking::Synapse<Networking::ConnectionMode::UDP>::listen()
{
    SOCKADDR_IN_T host = {};

    while (isAlive) {
        char* buffer[SYNAPSE_MESSAGE_SIZE_MAX] = {};
        int32_t size = recv_udp_message(reinterpret_cast<char *> (buffer), SYNAPSE_MESSAGE_SIZE_MAX, socket.socket,
                                        &host);
        if (size > 0) {
            const char *message = reinterpret_cast<char *> (buffer);
            AxonMessage message_(SerializedAxonMessage(message, size));
            onMessageReceived(message_, &host);
        }

        update();
    }

    // If being killed
    while (pool->getPoolSize() > 0)
        update();
}

template <Networking::ConnectionMode mode>
void Networking::Synapse<mode>::update() {
    MessagePoolNodePtr pNode = pool->pop();
    if (!pNode.get())
        return;
    this->sendTo(pNode->message, &pNode->destination);
}

template <Networking::ConnectionMode mode>
void Networking::Synapse<mode>::onMessageReceived(const AxonMessage& message, SOCKADDR_IN_T* from)
{
    // Notify
    SynapseMessageReceivedEvent event_ = SynapseMessageReceivedEvent(message, from);
    events.invoke(&event_);
}

#pragma endregion

#pragma region ASYNC_SYNAPS

template <Networking::ConnectionMode mode>
void Networking::AsyncSynapse<mode>::kill()
{
    if (!this->isAlive)
        return;

    this->isAlive = false;
    proc.join();
}

template <Networking::ConnectionMode mode>
Networking::AsyncSynapse<mode>::~AsyncSynapse()
{
    kill();
}

template <Networking::ConnectionMode mode>
void Networking::AsyncSynapse<mode>::start()
{
    this->isAlive = true;
    proc = std::thread(&AsyncSynapse::listen, this);
}

#pragma endregion
