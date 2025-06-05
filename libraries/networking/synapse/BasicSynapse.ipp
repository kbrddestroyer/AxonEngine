#pragma once
#include "networking/synapse/utils/SynapseUtility.hpp"

#pragma region BASIC_SYNAPSE
#pragma region CONTRUCTING

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class NetworkController>
Networking::BasicSynapse<conn, mode, NetworkController>::BasicSynapse(uint32_t port) :
    networkController(std::make_unique<NetworkController>(this, port))
{}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class NetworkController>
Networking::BasicSynapse<conn, mode, NetworkController>::BasicSynapse(const ConnectionInfo& connection) :
    networkController(std::make_unique<NetworkController>(this, connection))
{}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class NetworkController>
Networking::BasicSynapse<conn, mode, NetworkController>::~BasicSynapse()
{
    networkController->kill();
}

#pragma endregion

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class NetworkController>
void Networking::BasicSynapse<conn, mode, NetworkController>::start()
{
    networkController->start();
    listen();
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class NetworkController>
void Networking::BasicSynapse<conn, mode, NetworkController>::send(AxonMessage& message)
{
    networkController->send(message);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class NetworkController>
void Networking::BasicSynapse<conn, mode, NetworkController>::sendTo(AxonMessage& message, const Socket& dest)
{
    networkController->sendTo(message, dest);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class NetworkController>
void Networking::BasicSynapse<conn, mode, NetworkController>::processIncomingMessage(const SerializedAxonMessage &message, const Socket& from) {
    onMessageReceived(AxonMessage(message), from);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class NetworkController>
void Networking::BasicSynapse<conn, mode, NetworkController>::listen() {
    networkController->listen();
}

#pragma endregion
#pragma endregion
