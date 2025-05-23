#pragma once

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
void Networking::Synapse<mode>::sendPooled(const AxonMessage& message, const SOCKADDR_IN_T* dest) {
    if (!dest)
        dest = &socket.conn;

    pool.push( { message, *dest } );
}

template <Networking::ConnectionMode mode>
void Networking::Synapse<mode>::send(const AxonMessage& message)
{
    sendTo(message, &socket.conn);
}

template <Networking::ConnectionMode mode>
void Networking::Synapse<mode>::sendTo(const AxonMessage& message, const SOCKADDR_IN_T* dest) const
{
    SerializedAxonMessage serialized = message.getSerialized();
    send_message<mode>(socket, serialized.bitstream, serialized.size);
}

template<>
inline void Networking::Synapse<Networking::ConnectionMode::TCP>::listen()
{
    // Todo: async support
    char *buffer[SYNAPSE_MESSAGE_MAX_SIZE] = {};
    SOCKADDR_IN_T host = {};
    SOCKET_T client;

    if (isServer) {
        do {
            client = accept_incoming(socket.socket, &host);
        } while (!CHECK_VALID(client));
    }
    else {
        host = socket.conn;
        client = socket.socket;
    }

    while (isAlive)
    {
        const int32_t size = recv_tcp_message(reinterpret_cast<char *>(buffer), 256, client);
        if (size > 0)
        {
            const char *message = reinterpret_cast<char *> (buffer);
            AxonMessage message_ = AxonMessage(message, size);
            onMessageReceived(message_, &host);
        }

        update();
    }
}

template<>
inline void Networking::Synapse<Networking::ConnectionMode::UDP>::listen()
{
    SOCKADDR_IN_T host = {};

    while (isAlive) {
        char* buffer[SYNAPSE_MESSAGE_MAX_SIZE] = {};
        int32_t size = recv_udp_message(reinterpret_cast<char *> (buffer), SYNAPSE_MESSAGE_MAX_SIZE, socket.socket,
                                        &host);
        if (size > 0) {
            const char *message = reinterpret_cast<char *> (buffer);
            AxonMessage message_ = AxonMessage(message, size);
            onMessageReceived(message_, &host);
        }

        update();
    }
}

template <Networking::ConnectionMode mode>
void Networking::Synapse<mode>::update() {
    if (pool.getPoolSize() > 0) {
        MessagePoolNodePtr pNode = pool.pop();

        sendTo(pNode->message, &pNode->destination);
    }
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
