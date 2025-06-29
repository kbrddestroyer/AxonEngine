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
#include <networking/synapse/netcontroller/AxonNetworkController.hpp>


namespace Networking {
    /**
    * Basic connection handler
    * Specifies one-to-one connection for client and one-to-many for server.
    *
    * @tparam NetworkController derived from AxonNetworkControllerBase class
    */
    template <class NetworkController>
    class AXON_DECLSPEC BasicSynapse : public SynapseInterface
    {
        static_assert(std::is_base_of<AxonNetworkControllerBase, NetworkController>());
        static_assert(!std::is_abstract<NetworkController>());
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

        void kill() override { this->networkController->kill(); }
        void start() override;
        void send(AxonMessage&) override;
        void sendTo(AxonMessage&, const Socket&) override;

        void listen() override;
        void update() override {}
        void onMessageReceived(const AxonMessage&, const Socket&) override {};

        void processIncomingMessage(const SerializedAxonMessage&, const Socket&) override;
    protected:
        std::unique_ptr<NetworkController> networkController;
    };
}

#include "BasicSynapse.ipp"
// BasicSynapse.hpp
