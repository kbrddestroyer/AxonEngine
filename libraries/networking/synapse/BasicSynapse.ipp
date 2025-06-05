#pragma once
#include "networking/synapse/utils/SynapseUtility.hpp"

#pragma region BASIC_SYNAPSE
#pragma region CONTRUCTING

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class NetworkController>
Networking::BasicSynapse<conn, mode, NetworkController>::BasicSynapse(uint32_t port) :
    controller(std::make_unique<BerkeleyAxonNetworkController<conn, mode>>(this, port))
{}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class NetworkController>
Networking::BasicSynapse<conn, mode, NetworkController>::BasicSynapse(const ConnectionInfo& connection) :
    controller(std::make_unique<BerkeleyAxonNetworkController<conn, mode>>(this, connection))
{}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class NetworkController>
Networking::BasicSynapse<conn, mode, NetworkController>::~BasicSynapse()
{
    controller->kill();
}

#pragma endregion

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class NetworkController>
void Networking::BasicSynapse<conn, mode, NetworkController>::start()
{
    controller->start();
    listen();
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class NetworkController>
void Networking::BasicSynapse<conn, mode, NetworkController>::send(AxonMessage& message)
{
    controller->send(message);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class NetworkController>
void Networking::BasicSynapse<conn, mode, NetworkController>::sendTo(AxonMessage& message, const Socket& dest)
{
    controller->sendTo(message, dest);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class NetworkController>
void Networking::BasicSynapse<conn, mode, NetworkController>::processIncomingMessage(const SerializedAxonMessage &message, const Socket& from) {
    onMessageReceived(AxonMessage(message), from);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class NetworkController>
void Networking::BasicSynapse<conn, mode, NetworkController>::listen() {
    controller->listen();
}

#pragma endregion
#pragma endregion
