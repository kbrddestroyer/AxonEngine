#pragma once
#include <atomic>
#include "networking/synapse/utils/SynapseUtility.hpp"
#include "networking/synapse/utils/SynapseEvents.hpp"


namespace Networking {
    class AXON_DECLSPEC SynapseInterface {
    public:
        SynapseInterface() = default;
        virtual ~SynapseInterface() = default;

        virtual void kill() = 0;

        virtual void start() = 0;
        virtual void send(AxonMessage &) = 0;
        virtual void sendTo(AxonMessage&, const Socket&) = 0;

        EventSystem::AxonEventManager& getEventManager() { return events; }
        virtual void listen() = 0;
        virtual void update() = 0;
        virtual void onMessageReceived(const AxonMessage&, const Socket&) = 0;
        virtual void processIncomingMessage(const SerializedAxonMessage&, const Socket&) = 0;
    protected:
        EventSystem::AxonEventManager events;
    };
}