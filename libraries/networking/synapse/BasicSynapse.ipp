#pragma once
#include "networking/synapse/utils/SynapseUtility.hpp"

#pragma region BASIC_SYNAPSE
#pragma region CONTRUCTING

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
Networking::BasicSynapse<conn, mode>::BasicSynapse(uint32_t port) :
    connectionInfo({"", port}),
    socketInfo(),
    controller(std::make_unique<BerkeleyAxonNetworkController<conn, mode>>(this, port))
{}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
Networking::BasicSynapse<conn, mode>::BasicSynapse(const ConnectionInfo& connection) :
    connectionInfo(connection),
    socketInfo(),
    controller(std::make_unique<BerkeleyAxonNetworkController<conn, mode>>(this, connection))
{}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
Networking::BasicSynapse<conn, mode>::~BasicSynapse()
{
    isAlive = false;
    controller->kill();
    finalize<conn>(socketInfo);
}

#pragma endregion

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::BasicSynapse<conn, mode>::start()
{
    isAlive.store(true);
    controller->start();
    listen();
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::BasicSynapse<conn, mode>::send(AxonMessage& message)
{
    controller->send(message);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::BasicSynapse<conn, mode>::sendTo(AxonMessage& message, const Socket& dest)
{
    controller->sendTo(message, dest);
}

template<Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::BasicSynapse<conn, mode>::processIncomingMessage(const SerializedAxonMessage &message, const Socket& from) {
    onMessageReceived(AxonMessage(message), from);
}

template<Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::BasicSynapse<conn, mode>::listen() {
    controller->listen();
}

#pragma endregion
#pragma endregion
