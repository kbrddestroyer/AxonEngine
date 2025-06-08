#pragma once
#include <algorithm>


#pragma region SYNAPSE

template<class controller>
void Networking::Synapse<controller>::update() {
    MessagePoolNodePtr pNode = this->msgproc->processPool();
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
        this->msgproc->awaitValidation(message.ID());
    }

    BasicSynapse<controller>::sendTo(message, dest);
}

template<class controller>
void Networking::Synapse<controller>::sendPooled(const AxonMessage& message, const Socket &dest) const {
    this->msgproc->poolMessage( { message, dest } );
}

template<class controller>
void Networking::Synapse<controller>::onMessageReceived(const AxonMessage& message, const Socket &from)
{
    this->msgproc->process(message, from);
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
