#pragma once
#include "netconfig.h"

#pragma region SYNAPSE

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::Synapse<conn, mode>::update() {
    MessagePoolNodePtr pNode = pool->pop();
    if (!pNode.get())
        return;

    this->sendTo(pNode->message, &pNode->destination);
}

template<Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::Synapse<conn, mode>::sendTo(AxonMessage &message, const SOCKADDR_IN_T *dest) {
    Networking::AxonMessage::UniqueAxonMessagePtr ptr = message.split(SYNAPSE_PAYLOAD_SIZE_MAX);
    if (ptr)
    {
        sendPooled(*ptr.get(), dest);
    }

    if (message.hasFlag(VALIDATE))
    {
        pendingValidation.push_back(message.ID());
    }

    BasicSynapse<conn, mode>::sendTo(message, dest);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::Synapse<conn, mode>::sendPooled(const AxonMessage& message, const SOCKADDR_IN_T* dest) const {
    if (!dest)
        dest = &this->socketInfo.conn;
    pool->push( { message, *dest } );
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::Synapse<conn, mode>::onMessageReceived(const AxonMessage& message, SOCKADDR_IN_T* from)
{
    if (message.hasFlag(VALIDATE))
    {
        sendPooled(AxonMessage(message, 0), from);
    }
    if (message.hasFlag(ACKNOWLEDGE) and !message.hasFlag(PARTIAL))
    {
        auto it = std::find(pendingValidation.begin(), pendingValidation.end(), message.ID());
        if (it != pendingValidation.end())
        {
            pendingValidation.erase(it);
        }
    }
    if (mmap->contains(message.ID()) || message.hasFlag(PARTIAL))
    {
        mmap->append(message);

        if (!message.hasFlag(PARTIAL))
        {
            // Fini
            std::shared_ptr<AxonMessage> res = mmap->collapse(message.ID());
            if (!res.get())
                return;

            SynapseMessageReceivedEvent event_ = SynapseMessageReceivedEvent(*res, from);
            events.invoke(&event_);
        }
        return;
    }

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
