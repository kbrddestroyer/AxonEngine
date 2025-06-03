#pragma once
#include "networking/synapse/utils/SynapseUtility.hpp"

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
        throw Networking::AxonNetworkingInternalError(result);
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
        throw Networking::AxonNetworkingInternalError(result);
    }
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
Networking::BasicSynapse<conn, mode>::~BasicSynapse()
{
    isAlive = false;
    finalize<conn>(socketInfo);
}

#pragma endregion

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::BasicSynapse<conn, mode>::start()
{
    isAlive.store(true);
    listen();
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::BasicSynapse<conn, mode>::send(AxonMessage& message)
{
    sendTo(message, socketInfo);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::BasicSynapse<conn, mode>::sendTo(AxonMessage& message, const Socket& dest)
{
    sendTo(message.getSerialized(), dest);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::BasicSynapse<conn, mode>::sendTo(const SerializedAxonMessage& serialized, const Socket& dest) const
{
    send_message<conn>(dest, serialized.getBits(), serialized.getSize());
}

template<Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::BasicSynapse<conn, mode>::processIncomingMessage(const SerializedAxonMessage &message, const Socket& from) {
    onMessageReceived(AxonMessage(message), from);
}

#pragma region SERVER_FUNCTIONS

template<>
inline void Networking::BasicSynapse<Networking::ConnectionMode::TCP, Networking::SynapseMode::SERVER>::listen()
{
    fd_set master;
    FD_ZERO(&master);
    FD_SET(socketInfo.socket, &master);
    SOCKET_T maxSocket = socketInfo.socket;

    while (isAlive)
    {
        timeval tv = { 0, 10000 };
        fd_set reads = master;
        if (select(maxSocket + 1, &reads, nullptr, nullptr, &tv) <= 0) {
            continue;
        }
        for (SOCKET_T connectionSock = 1; connectionSock <= maxSocket; connectionSock++) {
            if (FD_ISSET(connectionSock, &reads)){
                if (connectionSock == socketInfo.socket) {
                    sockaddr_storage storage = {};
                    SOCKET_T client = accept_incoming(socketInfo.socket, reinterpret_cast<SOCKADDR_IN_T *>(&storage));

                    if (!CHECK_VALID(client)) {
                        continue;
                    }

                    FD_SET(client, &master);
                    if (client > maxSocket) {
                        maxSocket = client;
                    }
                }
                else {
                    char buffer[SYNAPSE_MESSAGE_SIZE_MAX] = {};
                    Socket socket = { connectionSock, {} };
                    const int32_t size = recv_message<TCP>(socket, buffer, 256);
                    if (size > 0)
                    {
                        processIncomingMessage(SerializedAxonMessage(buffer, size), socket);
                    }
                    else if (size < 0)
                    {
                        FD_CLR(connectionSock, &master);
                        CLOSESOCKET(connectionSock);
                        continue;
                    }
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
        char buffer[SYNAPSE_MESSAGE_SIZE_MAX] = {};
        int32_t size = recv_message<UDP>(socketInfo, buffer, SYNAPSE_MESSAGE_SIZE_MAX);
        if (size > 0) {
            processIncomingMessage(SerializedAxonMessage(buffer, size), socketInfo);
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
        char buffer[SYNAPSE_MESSAGE_SIZE_MAX] = {};
        const int32_t size = recv_tcp_message(reinterpret_cast<char *>(buffer), 256, client);
        if (size > 0)
        {
            processIncomingMessage(SerializedAxonMessage(buffer, size), socketInfo);
        }

        update();
    }
}

template<>
inline void Networking::BasicSynapse<Networking::ConnectionMode::UDP, Networking::SynapseMode::CLIENT>::listen() {
    SOCKADDR_IN_T host = {};

    while (isAlive) {
        char buffer[SYNAPSE_MESSAGE_SIZE_MAX] = {};
        int32_t size = recv_udp_message(reinterpret_cast<char *> (buffer), SYNAPSE_MESSAGE_SIZE_MAX, socketInfo.socket,
                                        &host);
        if (size > 0) {
            processIncomingMessage(SerializedAxonMessage(buffer, size), socketInfo);
        }

        update();
    }
}

#pragma endregion
#pragma endregion
