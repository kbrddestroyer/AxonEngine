#pragma once


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
void Networking::BasicSynapse<conn, mode>::send(AxonMessage& message)
{
    sendTo(message, &socketInfo.conn);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::BasicSynapse<conn, mode>::sendTo(AxonMessage& message, const SOCKADDR_IN_T* dest)
{
    sendTo(message.getSerialized(), dest);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::BasicSynapse<conn, mode>::sendTo(const SerializedAxonMessage& serialized, const SOCKADDR_IN_T* dest) const
{
    send_message<conn>({ socketInfo.socket, *dest }, serialized.getBits(), serialized.getSize());
}

template<Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::BasicSynapse<conn, mode>::processIncomingMessage(SerializedAxonMessage message, SOCKADDR_IN_T *host) {
    onMessageReceived(AxonMessage(message), host);
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
                    const int32_t size = recv_message<TCP>(socketInfo, buffer, 256);

                    if (size < 0) {
                        FD_CLR(connectionSock, &master);
                        CLOSESOCKET(connectionSock);
                        continue;
                    }

                    processIncomingMessage(SerializedAxonMessage(buffer, size), &socketInfo.conn);
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
            processIncomingMessage(SerializedAxonMessage(buffer, size), &socketInfo.conn);
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
            processIncomingMessage(SerializedAxonMessage(buffer, size), &host);
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
            processIncomingMessage(SerializedAxonMessage(buffer, size), &host);
        }

        update();
    }
}

#pragma endregion
#pragma endregion
