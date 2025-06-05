#pragma once
#include <algorithm>


#pragma region SYNAPSE

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class controller>
void Networking::Synapse<conn, mode, controller>::update() {
    MessagePoolNodePtr pNode = pool->pop();
    if (!pNode.get())
        return;

    this->sendTo(pNode->message, pNode->destination);
}

template<Networking::ConnectionMode conn, Networking::SynapseMode mode, class controller>
void Networking::Synapse<conn, mode, controller>::sendTo(AxonMessage &message, const Socket &dest) {
    const AxonMessage::UniqueAxonMessagePtr ptr = message.split(SYNAPSE_PAYLOAD_SIZE_MAX);
    if (ptr)
    {
        sendPooled(*ptr.get(), dest);
    }

    if (message.hasFlag(VALIDATE))
    {
        pendingValidation.push_back(message.ID());
    }

    BasicSynapse<conn, mode, controller>::sendTo(message, dest);
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class controller>
void Networking::Synapse<conn, mode, controller>::sendPooled(const AxonMessage& message, const Socket &dest) const {
    pool->push( { message, dest } );
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class controller>
void Networking::Synapse<conn, mode, controller>::onMessageReceived(const AxonMessage& message, const Socket &from)
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

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class controller>
void Networking::AsyncSynapse<conn, mode, controller>::kill()
{
    if (!this->controller->isAlive())
        return;

    this->controller->kill();
    proc.join();
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class controller>
Networking::AsyncSynapse<conn, mode, controller>::~AsyncSynapse()
{
    kill();
}

template <Networking::ConnectionMode conn, Networking::SynapseMode mode, class controller>
void Networking::AsyncSynapse<conn, mode, controller>::start()
{
    this->controller->start();
    proc = std::thread(&AsyncSynapse::listen, this);
}

#pragma endregion
