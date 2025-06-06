#pragma once
#include "networking/synapse/utils/SynapseUtility.hpp"

#pragma region BASIC_SYNAPSE
#pragma region CONTRUCTING

template <class NetworkController>
Networking::BasicSynapse<NetworkController>::BasicSynapse(uint32_t port) :
    networkController(std::make_unique<NetworkController>(this, port))
{}

template <class NetworkController>
Networking::BasicSynapse<NetworkController>::BasicSynapse(const ConnectionInfo& connection) :
    networkController(std::make_unique<NetworkController>(this, connection))
{}

template <class NetworkController>
Networking::BasicSynapse<NetworkController>::~BasicSynapse()
{
    networkController->kill();
}

#pragma endregion

template <class NetworkController>
void Networking::BasicSynapse<NetworkController>::start()
{
    networkController->start();
    listen();
}

template <class NetworkController>
void Networking::BasicSynapse<NetworkController>::send(AxonMessage& message)
{
    networkController->send(message);
}

template <class NetworkController>
void Networking::BasicSynapse<NetworkController>::sendTo(AxonMessage& message, const Socket& dest)
{
    networkController->sendTo(message, dest);
}

template <class NetworkController>
void Networking::BasicSynapse<NetworkController>::processIncomingMessage(const SerializedAxonMessage &message, const Socket& from) {
    onMessageReceived(AxonMessage(message), from);
}

template <class NetworkController>
void Networking::BasicSynapse<NetworkController>::listen() {
    networkController->listen();
}

#pragma endregion
#pragma endregion
