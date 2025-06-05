#pragma once
#include <backends/backend.hpp>
#include <networking/synapse/BasicSynapse.hpp>


namespace Networking {
    void AxonNetworkControllerBase::start() {
        this->alive = true;
    }

    void AxonNetworkControllerBase::kill() {
        this->alive = false;
    }

    AxonNetworkControllerBase::AxonNetworkControllerBase(SynapseInterface *owner) :
        owningSynapse(owner) {}

    template<ConnectionMode conn, SynapseMode mode>
    BerkeleyAxonNetworkController<conn, mode>::BerkeleyAxonNetworkController(SynapseInterface *owner, uint32_t port) :
        meta({ "", port }),
        connection(),
        AxonNetworkControllerBase(owner)
    {
        static_assert(mode == SynapseMode::SERVER);
        uint8_t ret = initialize_server<conn> ( connection, port );
        if ( ret != SUCCESS ) {
            throw Networking::AxonNetworkingInternalError(ret);
        }
    }

    template<ConnectionMode conn, SynapseMode mode>
    BerkeleyAxonNetworkController<conn, mode>::BerkeleyAxonNetworkController(SynapseInterface *owner, const ConnectionInfo info) :
            meta(info),
            connection(),
            AxonNetworkControllerBase(owner)
    {
        static_assert(mode == SynapseMode::CLIENT);
        uint8_t ret = initialize_client<conn> ( connection, info.hostname.c_str(), info.port );
        if ( ret != SUCCESS ) {
            throw Networking::AxonNetworkingInternalError(ret);
        }
    }

    template<ConnectionMode conn, SynapseMode mode>
    BerkeleyAxonNetworkController<conn, mode>::~BerkeleyAxonNetworkController() {
        this->kill();
        finalize<conn>(connection);
    }

    template<ConnectionMode conn, SynapseMode mode>
    void BerkeleyAxonNetworkController<conn, mode>::send(AxonMessage &message) {
        this->sendTo(message, this->connection);
    }

    template<ConnectionMode conn, SynapseMode mode>
    void BerkeleyAxonNetworkController<conn, mode>::sendTo(AxonMessage &message, const NetworkNodeInfo &dest) {
        this->sendSerializedTo(message.getSerialized(), dest);
    }

    template<ConnectionMode conn, SynapseMode mode>
    void BerkeleyAxonNetworkController<conn, mode>::sendSerializedTo(const SerializedAxonMessage &serialized,
                                                                     const NetworkNodeInfo &dest) {
        send_message<conn>(dest, serialized.getBits(), serialized.getSize());
    }


#pragma region SERVER_FUNCTIONS

    template<>
    inline void Networking::BerkeleyAxonNetworkController<Networking::ConnectionMode::TCP, Networking::SynapseMode::SERVER>::listen()
    {
        fd_set master;
        FD_ZERO(&master);
        FD_SET(connection.socket, &master);
        SOCKET_T maxSocket = connection.socket;

        while (this->isAlive())
        {
            timeval tv = { 0, 10000 };
            fd_set reads = master;
            if (select(maxSocket + 1, &reads, nullptr, nullptr, &tv) <= 0) {
                continue;
            }
            for (SOCKET_T connectionSock = 1; connectionSock <= maxSocket; connectionSock++) {
                if (FD_ISSET(connectionSock, &reads)){
                    if (connectionSock == connection.socket) {
                        sockaddr_storage storage = {};
                        SOCKET_T client = accept_incoming(connection.socket, reinterpret_cast<SOCKADDR_IN_T *>(&storage));

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
                            owningSynapse->processIncomingMessage(SerializedAxonMessage(buffer, size), socket);
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
            owningSynapse->update();
        }
    }

    template<>
    inline void Networking::BerkeleyAxonNetworkController<Networking::ConnectionMode::UDP, Networking::SynapseMode::SERVER>::listen()
    {
        while (this->isAlive()) {
            char buffer[SYNAPSE_MESSAGE_SIZE_MAX] = {};
            int32_t size = recv_message<UDP>(connection, buffer, SYNAPSE_MESSAGE_SIZE_MAX);
            if (size > 0) {
                owningSynapse->processIncomingMessage(SerializedAxonMessage(buffer, size), connection);
            }

            owningSynapse->update();
        }
    }

#pragma endregion

#pragma region CLIENT_FUNCTIONS

    template<>
    inline void Networking::BerkeleyAxonNetworkController<Networking::ConnectionMode::TCP, Networking::SynapseMode::CLIENT>::listen() {
        SOCKET_T client = connection.socket;

        while (this->isAlive())
        {
            char buffer[SYNAPSE_MESSAGE_SIZE_MAX] = {};
            const int32_t size = recv_tcp_message(reinterpret_cast<char *>(buffer), 256, client);
            if (size > 0)
            {
                owningSynapse->processIncomingMessage(SerializedAxonMessage(buffer, size), connection);
            }

            owningSynapse->update();
        }
    }

    template<>
    inline void Networking::BerkeleyAxonNetworkController<Networking::ConnectionMode::UDP, Networking::SynapseMode::CLIENT>::listen() {
        SOCKADDR_IN_T host = {};

        while (this->isAlive()) {
            char buffer[SYNAPSE_MESSAGE_SIZE_MAX] = {};
            int32_t size = recv_udp_message(reinterpret_cast<char *> (buffer), SYNAPSE_MESSAGE_SIZE_MAX, connection.socket,
                                            &host);
            if (size > 0) {
                owningSynapse->processIncomingMessage(SerializedAxonMessage(buffer, size), connection);
            }

            owningSynapse->update();
        }
    }

#pragma endregion
}