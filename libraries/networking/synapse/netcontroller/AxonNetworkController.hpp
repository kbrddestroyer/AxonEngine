#pragma once
#include <atomic>

#include <synapse/SynapseInterface.hpp>

#include <AxonUtility.h>
#include <messages/AxonMessage.hpp>
#include <synapse/utils/SynapseUtility.hpp>

namespace Networking {
    typedef Socket NetworkNodeInfo;

    class AXON_DECLSPEC AxonNetworkControllerBase {
    public:
        virtual ~AxonNetworkControllerBase() = default;

        GETTER bool isAlive() { return alive; }

        virtual void start();
        void kill();

        virtual void listen() = 0;
        virtual void send(AxonMessage&) = 0;
        virtual void sendTo(AxonMessage&, const NetworkNodeInfo&) = 0;
    protected:
        virtual void sendSerializedTo(const SerializedAxonMessage&, const NetworkNodeInfo&) = 0;
    private:
        std::atomic<bool> alive = false;
    };

    template <ConnectionMode conn, SynapseMode mode>
    class AXON_DECLSPEC BerkeleyAxonNetworkController : public AxonNetworkControllerBase {
    public:
        BerkeleyAxonNetworkController() = delete;

        explicit BerkeleyAxonNetworkController(SynapseInterface * , uint32_t);
        explicit BerkeleyAxonNetworkController(SynapseInterface * , ConnectionInfo);

        ~BerkeleyAxonNetworkController() override;

        void listen() override {};
        void send(AxonMessage&) override;
        void sendTo(AxonMessage&, const NetworkNodeInfo&) override;
    protected:
        void sendSerializedTo(const SerializedAxonMessage&, const NetworkNodeInfo&) override;
    private:
        ConnectionInfo  meta;
        NetworkNodeInfo connection; // Client-only feature, server will store nothing
    protected:
        SynapseInterface *owningSynapse;
    };
}


#include "AxonNetworkController.ipp"
