/**
 * BasicSynapse is a class that contains backend calls and basic message handling. It has no advanced functionality
 * such as event dispatching, thread managing, etc.
 *
 * It was created to separate low-level implementations from high level Synapse interface.
 * This class still can be used to create custom Synapse. Just override onMessageReceived and update methods.
 */

#pragma once
#include <atomic>
#include "networking/synapse/utils/SynapseUtility.hpp"
#include "messages/AxonMessage.hpp"
#include "networking/synapse/utils/SynapseEvents.hpp"
#include "SynapseInterface.hpp"
#include <synapse/netcontroller/AxonNetworkController.hpp>


namespace Networking {
    /**
    * Basic connection handler
    * Specifies one-to-one connection for client and one-to-many for server.
    *
    * @tparam conn connection mode (TCP|UDP)
    * @tparam mode synapse mode (CLIENT|SERVER)
    */
    template <ConnectionMode conn, SynapseMode mode>
    class AXON_DECLSPEC BasicSynapse : public SynapseInterface
    {
    public:
        /** Default creation is restricted */
        BasicSynapse() = delete;
        /** Initializes Synapse in server mode */
        explicit BasicSynapse(uint32_t);
        /** Initialize Synapse in client mode */
        explicit BasicSynapse(const ConnectionInfo&);

        explicit BasicSynapse(const BasicSynapse&) = delete;
        BasicSynapse(BasicSynapse&&) noexcept = delete;
        BasicSynapse& operator=(const BasicSynapse&) = delete;
        BasicSynapse& operator=(BasicSynapse&&) noexcept = delete;

        ~BasicSynapse() override;

        void start() override;
        void send(AxonMessage&) override;
        void sendTo(AxonMessage&, const Socket&) override;

        // This function should be instanced for each connection type
        void listen() override;
        void update() override {}
        void onMessageReceived(const AxonMessage&, const Socket&) override {};

        void processIncomingMessage(const SerializedAxonMessage&, const Socket&) override;
    protected:
        ConnectionInfo		connectionInfo;
        Socket              socketInfo;

        std::unique_ptr<BerkeleyAxonNetworkController<conn, mode>> controller;
    };
}

#include "BasicSynapse.ipp"
// BasicSynapse.hpp
