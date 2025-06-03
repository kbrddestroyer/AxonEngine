#pragma once
#include <algorithm>


#pragma region SYNAPSE

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::Synapse<conn, mode>::update() {
    MessagePoolNodePtr pNode = pool->pop();
    if (!pNode.get())
        return;

    this->sendTo(pNode->message, pNode->destination);
}

template<Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::Synapse<conn, mode>::sendTo(AxonMessage &message, const Socket &dest) {
    const AxonMessage::UniqueAxonMessagePtr ptr = message.split(SYNAPSE_PAYLOAD_SIZE_MAX);
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
void Networking::Synapse<conn, mode>::sendPooled(const AxonMessage& message, const Socket &dest) const {
    pool->push( { message, dest } );
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::Synapse<conn, mode>::onMessageReceived(const AxonMessage& message, const Socket &from)
{
    if (message.hasFlag(VALIDATE))
    {
        sendPooled(AxonMessage(message, 0), from);
    }
    if (message.hasFlag(ACKNOWLEDGE) && !message.hasFlag(PARTIAL))
    {
        pendingValidation.erase(std::find(pendingValidation.begin(), pendingValidation.end(), message.ID()));
    }
    if (mmap->contains(message.ID()) || message.hasFlag(PARTIAL))
    {
        mmap->append(message);

        if (!message.hasFlag(PARTIAL))
        {
            // Fini
            std::shared_ptr<AxonMessage> res = mmap->collapse(message.ID());
            if (!res)
                return;

            SynapseMessageReceivedEvent event_(*res, from);
            this->events.invoke(&event_);
        }
        return;
    }

    SynapseMessageReceivedEvent event_ = SynapseMessageReceivedEvent(message, from);
    this->events.invoke(&event_);
}

#pragma endregion

#pragma region ASYNC_SYNAPSE

template <Networking::ConnectionMode conn, Networking::SynapseMode mode>
void Networking::AsyncSynapse<conn, mode>::kill()
{
    if (!this->isAlive)
        return;

    this->isAlive = false;
    this->controller->kill();
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
    this->controller->start();
    proc = std::thread(&AsyncSynapse::listen, this);
}

#pragma endregion
