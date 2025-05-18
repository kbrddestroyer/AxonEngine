#pragma once
#include <SynapseConfig.h>
#include <iostream>

#include "Synapse.hpp"

namespace Networking {
    template<>
    void BasicSynapse<TCP, SynapseMode::SERVER>::listen();
    template<>
    void BasicSynapse<UDP, SynapseMode::SERVER>::listen();
    template<>
    void BasicSynapse<TCP, SynapseMode::CLIENT>::listen();
    template<>
    void BasicSynapse<UDP, SynapseMode::CLIENT>::listen();
}

#pragma region BASIC_SYNAPSE

#pragma region CONTRUCTING

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
Networking::BasicSynapse<conn, mode>::BasicSynapse(uint32_t port)
{
    connectionInfo.port = port;

    socketInfo = {};

    uint8_t result = initialize_server<conn>(socketInfo, port);

    if (result != SUCCESS) {
        throw AxonNetworkingInternalError(result);
    }
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
Networking::BasicSynapse<conn, mode>::BasicSynapse(const ConnectionInfo& connection)
{
    connectionInfo.hostname = connection.hostname;
    connectionInfo.port = connection.port;

    socketInfo = {};

    uint8_t result = initialize_client<conn>(socketInfo, connection.hostname.c_str(), connection.port);

    if (result != SUCCESS) {
        throw AxonNetworkingInternalError(result);
    }
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
Networking::BasicSynapse<conn, mode>::~BasicSynapse()
{
    isAlive = false;
    finalize_udp(socketInfo.socket);
}

#pragma endregion

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::BasicSynapse<conn, mode>::start()
{
    isAlive.store(true);
    listen();
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::BasicSynapse<conn, mode>::send(const AxonMessage& message)
{
    sendTo(message.getSerialized(), &socketInfo.conn);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::BasicSynapse<conn, mode>::sendTo(const SerializedAxonMessage& serialized, const SOCKADDR_IN_T* dest) const
{
    send_message<conn>({ socketInfo.socket, *dest }, serialized.getBits(), serialized.getSize());
}

template<Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::BasicSynapse<conn, mode>::processIncomingMessage(const char *message, const size_t size, SOCKADDR_IN *host) {
    onMessageReceived(AxonMessage(SerializedAxonMessage(message, size)), host);
}

#pragma region SERVER_FUNCTIONS

template<>
inline void Networking::BasicSynapse<Networking::ConnectionMode::TCP, Networking::SynapseMode::SERVER>::listen()
{
    SOCKADDR_IN_T host = {};

    fd_set master;
    FD_ZERO(&master);
    FD_SET(socketInfo.socket, &master);
    SOCKET_T maxSocket = socketInfo.socket;

    while (isAlive)
    {
        timeval tv = { 0, 10000 };
        fd_set reads = master;
        if (select(maxSocket + 1, &reads, nullptr, nullptr, 0) <= 0) {
            continue;
        }
        for (uint16_t i = 0; i < reads.fd_count; i++) {
            SOCKET_T connectionSock = reads.fd_array[i];

            if (FD_ISSET(connectionSock, &reads)){
                if (connectionSock == socketInfo.socket) {
                    sockaddr_storage storage = {};
                    SOCKET_T client = accept_incoming(socketInfo.socket, reinterpret_cast<SOCKADDR_IN *>(&storage));

                    if (!CHECK_VALID(client)) {
                        continue;
                    }

                    FD_SET(client, &master);
                    if (client > maxSocket) {
                        maxSocket = client;
                    }
                }
                else {
                    char *buffer[SYNAPSE_MESSAGE_SIZE_MAX] = {};
                    const int32_t size = recv_tcp_message(reinterpret_cast<char *>(buffer), 256, connectionSock);

                    if (size < 0) {
                        FD_CLR(connectionSock, &master);
                        CLOSESOCKET(connectionSock);
                        continue;
                    }

                    processIncomingMessage(reinterpret_cast<const char*>(buffer), size, &host);
                }
            }
        }
        update();
    }
}

template<>
inline void Networking::BasicSynapse<Networking::ConnectionMode::UDP, Networking::SynapseMode::SERVER>::listen()
{
    SOCKADDR_IN_T host = {};

    while (isAlive) {
        char* buffer[SYNAPSE_MESSAGE_SIZE_MAX] = {};
        int32_t size = recv_udp_message(reinterpret_cast<char *> (buffer), SYNAPSE_MESSAGE_SIZE_MAX, socketInfo.socket,
                                        &host);
        if (size > 0) {
            processIncomingMessage(reinterpret_cast<const char*>(buffer), size, &host);
        }

        update();
    }
}

#pragma endregion

#pragma region CLIENT_FUNCTIONS

template<>
inline void Networking::BasicSynapse<Networking::ConnectionMode::TCP, Networking::SynapseMode::CLIENT>::listen() {
    // Todo: async support
    SOCKADDR_IN_T host = socketInfo.conn;
    SOCKET_T client = socketInfo.socket;

    while (isAlive)
    {
        char *buffer[SYNAPSE_MESSAGE_SIZE_MAX] = {};
        const int32_t size = recv_tcp_message(reinterpret_cast<char *>(buffer), 256, client);
        if (size > 0)
        {
            processIncomingMessage(reinterpret_cast<const char*>(buffer), size, &host);
        }

        update();
    }
}

template<>
inline void Networking::BasicSynapse<Networking::ConnectionMode::UDP, Networking::SynapseMode::CLIENT>::listen() {
    SOCKADDR_IN_T host = {};

    while (isAlive) {
        char* buffer[SYNAPSE_MESSAGE_SIZE_MAX] = {};
        int32_t size = recv_udp_message(reinterpret_cast<char *> (buffer), SYNAPSE_MESSAGE_SIZE_MAX, socketInfo.socket,
                                        &host);
        if (size > 0) {
            processIncomingMessage(reinterpret_cast<const char*>(buffer), size, &host);
        }

        update();
    }
}

#pragma endregion

#pragma endregion

#pragma region SYNAPSE

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::Synapse<conn, mode>::update() {
    MessagePoolNodePtr pNode = pool->pop();
    if (!pNode.get())
        return;
    this->sendTo(pNode->message, &pNode->destination);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::Synapse<conn, mode>::sendPooled(const AxonMessage& message, const SOCKADDR_IN_T* dest) const {
    if (!dest)
        dest = &this->socketInfo.conn;

    pool->push( { message.getSerialized(), *dest } );
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::Synapse<conn, mode>::onMessageReceived(const AxonMessage& message, SOCKADDR_IN_T* from)
{
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
