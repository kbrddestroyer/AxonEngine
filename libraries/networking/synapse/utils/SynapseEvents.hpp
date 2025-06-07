#pragma once
#include "events/AxonEvent.hpp"
#include "messages/AxonMessage.hpp"
#include "AxonUtility.h"

struct Socket;

namespace Networking {
    /**
    * Event, that is generated when message is received from the remote. Can be used for message handling or output
    */
    class AXON_DECLSPEC SynapseMessageReceivedEvent final : public EventSystem::AxonEvent
    {
    public:
        SynapseMessageReceivedEvent(const AxonMessage& message, const Socket &from) :
            AxonEvent(), message(message), from(from)
        {}

        GETTER const AxonMessage& getMessage() const { return message; }
        GETTER const Socket& getFrom() const { return from; }
    private:
        const AxonMessage& message;
        const Socket& from;
    };
}
