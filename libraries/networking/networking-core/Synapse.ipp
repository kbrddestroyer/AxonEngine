#pragma once
#include "netconfig.h"
#include <iostream>

#include "Synapse.hpp"

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
