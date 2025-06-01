#pragma once
#include <events/AxonEvent.hpp>
#include <networking/message/AxonMessage.hpp>
#include <AxonUtility.h>

namespace Networking {
    /**
    * Event, that is generated when message is received from the remote. Can be used for message handling or output
    */
    class AXON_DECLSPEC SynapseMessageReceivedEvent final : public EventSystem::AxonEvent
    {
    public:
        SynapseMessageReceivedEvent(const AxonMessage& message, SOCKADDR_IN_T* from) : AxonEvent(), message(message)
        {
            this->from = from;
        }

        GETTER const AxonMessage& getMessage() const { return message; }
        GETTER SOCKADDR_IN_T* getFrom() const { return from; }
    private:
        const AxonMessage&	message;
        SOCKADDR_IN_T* from;
    };
}
