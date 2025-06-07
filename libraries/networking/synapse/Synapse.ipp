#pragma once
#include <algorithm>


#pragma region SYNAPSE

template<class controller>
void Networking::Synapse<controller>::update() {
    MessagePoolNodePtr pNode = pool->pop();
    if (!pNode.get())
        return;

    this->sendTo(pNode->message, pNode->destination);
}

template<class controller>
void Networking::Synapse<controller>::sendTo(AxonMessage &message, const Socket &dest) {
    const AxonMessage::UniqueAxonMessagePtr ptr = message.split(SYNAPSE_PAYLOAD_SIZE_MAX);
    if (ptr)
    {
        sendPooled(*ptr.get(), dest);
    }

    if (message.hasFlag(VALIDATE))
    {
        pendingValidation.push_back(message.ID());
    }

    BasicSynapse<controller>::sendTo(message, dest);
}

template<class controller>
void Networking::Synapse<controller>::sendPooled(const AxonMessage& message, const Socket &dest) const {
    pool->push( { message, dest } );
}

template<class controller>
void Networking::Synapse<controller>::onMessageReceived(const AxonMessage& message, const Socket &from)
{
    if (message.hasFlag(VALIDATE))
    {
        sendPooled(AxonMessage(message, 0), from);
    }
    if (message.hasFlag(ACKNOWLEDGE) && !message.hasFlag(PARTIAL))
    {
        auto it = std::find(pendingValidation.begin(), pendingValidation.end(), message.ID());
        if (it != pendingValidation.end())
            pendingValidation.erase(it);
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

template<class controller>
void Networking::AsyncSynapse<controller>::kill()
{
    if (!this->networkController->isAlive())
        return;

    this->networkController->kill();
    proc.join();
}

template<class controller>
Networking::AsyncSynapse<controller>::~AsyncSynapse()
{
    kill();
}

template<class controller>
void Networking::AsyncSynapse<controller>::start()
{
    this->networkController->start();
    proc = std::thread(&AsyncSynapse::listen, this);
}

#pragma endregion
