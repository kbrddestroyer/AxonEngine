/**
 * BasicSynapse is a class that contains backend calls and basic message handling. It has no advanced functionality
 * such as event dispatching, thread managing, etc.
 *
 * It was created to separate low-level implementations from high level Synapse interface.
 * This class still can be used to create custom Synapse. Just override onMessageReceived and update methods.
 */

#pragma once
#include <backends/backend.hpp>
#include <networking/networking-core/SynapseUtility.hpp>
#include <networking/message/AxonMessage.hpp>
#include <networking-core/SynapseEvents.hpp>

#include <atomic>

namespace Networking {
    /**
     * Non-template class that can be used as Synapse pointer
     */
    class AXON_DECLSPEC SynapseInterface {
    public:
        SynapseInterface() = default;
        virtual ~SynapseInterface();

        WGETTER ( bool alive() ) { return isAlive.load(); }
        virtual void kill() { isAlive.store(false); }

        virtual void start() = 0;
        virtual void send(AxonMessage &) = 0;
        virtual void sendTo(AxonMessage&, const Socket&) = 0;
        virtual void sendTo(const SerializedAxonMessage&, const Socket&) const = 0;

        EventSystem::AxonEventManager& getEventManager() { return events; }
        virtual void listen() = 0;
        virtual void update() = 0;
        virtual void onMessageReceived(const AxonMessage&, const Socket&) = 0;
    protected:
        std::atomic<bool>	isAlive = false;
		EventSystem::AxonEventManager events;
    };

    typedef SynapseInterface* SynapseInterfacePtr;

    /**
    * Basic connection handler
    * Specifies one-to-one connection for client and one-to-many for server.
    *
    * @tparam conn connection mode (TCP|UDP)
    * @tparam mode synapse mode (CLIENT|SERVER)
    */
    template <ConnectionMode conn, SynapseMode mode> class AXON_DECLSPEC BasicSynapse : public SynapseInterface
    {
    public:
        /** Default creation is restricted */
        BasicSynapse() = delete;
        /** Initializes Synapse in server mode */
        explicit BasicSynapse(uint32_t);
        /** Initialize Synapse in client mode */
        explicit BasicSynapse(const ConnectionInfo&);

        ~BasicSynapse() override;

        void start() override;
        void send(AxonMessage&) override;
        void sendTo(AxonMessage&, const Socket&) override;
        void sendTo(const SerializedAxonMessage&, const Socket&) const override;

        // This function should be instanced for each connection type
        void listen() override {}
        void update() override {}
        void onMessageReceived(const AxonMessage&, const Socket&) override {};

        void processIncomingMessage(const SerializedAxonMessage&, const Socket&);
    protected:
        ConnectionInfo		connectionInfo;
        Socket              socketInfo;
    };
}

#include "BasicSynapse.ipp"

// BasicSynapse.hpp
