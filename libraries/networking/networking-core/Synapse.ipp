#pragma once
#include <SynapseConfig.h>
#include <iostream>

#include "Synapse.hpp"

namespace Networking {
    template <ConnectionMode conn, SynapseMode mode>
    void Synapse<conn, mode>::listen() {}

    template<>
    void Synapse<ConnectionMode::TCP, SynapseMode::SERVER>::listen();

    template<>
    void Synapse<ConnectionMode::UDP, SynapseMode::SERVER>::listen();

    template class Synapse<ConnectionMode::UDP, SynapseMode::SERVER>;
    template class Synapse<ConnectionMode::UDP, SynapseMode::CLIENT>;
    template class Synapse<ConnectionMode::TCP, SynapseMode::SERVER>;
    template class Synapse<ConnectionMode::TCP, SynapseMode::CLIENT>;


    template class AsyncSynapse<ConnectionMode::UDP, SynapseMode::SERVER>;
    template class AsyncSynapse<ConnectionMode::UDP, SynapseMode::CLIENT>;
    template class AsyncSynapse<ConnectionMode::TCP, SynapseMode::SERVER>;
    template class AsyncSynapse<ConnectionMode::TCP, SynapseMode::CLIENT>;
}

#pragma region SYNAPSE

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
Networking::Synapse<conn, mode>::Synapse(uint32_t port)
{
    isServer = true;
    info.port = port;

    socket = {};

    uint8_t result = initialize_server<conn>(socket, port);

    if (result != SUCCESS) {
        throw AxonNetworkingInternalError(result);
    }
}


template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
Networking::Synapse<conn, mode>::Synapse(const ConnectionInfo& connection)
{
    isServer = false;
    info.hostname = connection.hostname;
    info.port = connection.port;

    socket = {};

    uint8_t result = initialize_client<conn>(socket, connection.hostname.c_str(), connection.port);

    if (result != SUCCESS) {
        throw AxonNetworkingInternalError(result);
    }
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
Networking::Synapse<conn, mode>::~Synapse()
{
    isAlive = false;
    finalize_udp(socket.socket);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::Synapse<conn, mode>::start()
{
    isAlive = true;

    listen();
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::Synapse<conn, mode>::sendPooled(const AxonMessage& message, const SOCKADDR_IN_T* dest) const {
    if (!dest)
        dest = &socket.conn;

    pool->push( { message.getSerialized(), *dest } );
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::Synapse<conn, mode>::send(const AxonMessage& message)
{
    sendTo(message.getSerialized(), &socket.conn);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::Synapse<conn, mode>::sendTo(const SerializedAxonMessage& serialized, const SOCKADDR_IN_T* dest) const
{
    send_message<conn>({ socket.socket, *dest }, serialized.getBits(), serialized.getSize());
}

template<>
inline void Networking::Synapse<Networking::ConnectionMode::TCP, Networking::SynapseMode::SERVER>::listen()
{
    // Todo: async support

    if (isServer) {
        SOCKADDR_IN_T host = {};

        fd_set master;
        FD_ZERO(&master);
        FD_SET(socket.socket, &master);
        SOCKET_T maxSocket = socket.socket;

        while (isAlive)
        {
            timeval tv = { 0, 10000 };
            fd_set reads = master;
            if (select(maxSocket + 1, &reads, nullptr, nullptr, 0) <= 0) {
                continue;
            }
            std::cout << "Select() " << reads.fd_count << " " << maxSocket << std::endl;
            for (uint16_t i = 0; i < reads.fd_count; i++) {
                SOCKET_T connectionSock = reads.fd_array[i];

                std::cout << "Iterating on: " << i << " " << connectionSock << std::endl;

                if (FD_ISSET(connectionSock, &reads)){
                    std::cout << "Accepting on: " << i << " " << connectionSock << std::endl;

                    if (connectionSock == socket.socket) {
                        sockaddr_storage storage = {};
                        SOCKET_T client = accept_incoming(socket.socket, reinterpret_cast<SOCKADDR_IN *>(&storage));

                        if (!CHECK_VALID(client)) {
                            std::cout << "Error" << std::endl;
                            continue;
                        }

                        FD_SET(client, &master);
                        if (client > maxSocket) {
                            maxSocket = client;
                        }

                        std::cout << "Accepted " << client << std::endl;
                    }
                    else {
                        std::cout << "Listening on: " << i << " " << connectionSock << std::endl;

                        char *buffer[SYNAPSE_MESSAGE_SIZE_MAX] = {};
                        const int32_t size = recv_tcp_message(reinterpret_cast<char *>(buffer), 256, connectionSock);

                        if (size < 0) {
                            FD_CLR(connectionSock, &master);
                            CLOSESOCKET(connectionSock);
                            continue;
                        }

                        const char *message = reinterpret_cast<char *> (buffer);
                        AxonMessage message_(SerializedAxonMessage(message, size));
                        onMessageReceived(message_, &host);
                    }
                }
            }
            update();
        }
    }

    isAlive = false;
}

template<>
inline void Networking::Synapse<Networking::ConnectionMode::UDP, Networking::SynapseMode::SERVER>::listen()
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

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::Synapse<conn, mode>::update() {
    MessagePoolNodePtr pNode = pool->pop();
    if (!pNode.get())
        return;
    this->sendTo(pNode->message, &pNode->destination);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::Synapse<conn, mode>::onMessageReceived(const AxonMessage& message, SOCKADDR_IN_T* from)
{
    // Notify
    SynapseMessageReceivedEvent event_ = SynapseMessageReceivedEvent(message, from);
    events.invoke(&event_);
}

#pragma endregion

#pragma region ASYNC_SYNAPS

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::AsyncSynapse<conn, mode>::kill()
{
    if (!this->isAlive)
        return;

    this->isAlive = false;
    proc.join();
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
Networking::AsyncSynapse<conn, mode>::~AsyncSynapse()
{
    kill();
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::AsyncSynapse<conn, mode>::start()
{
    this->isAlive = true;
    proc = std::thread(&AsyncSynapse::listen, this);
}

#pragma endregion
