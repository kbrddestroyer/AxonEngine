#pragma once
#include <networking/AxonNetwork.hpp>


namespace Networking {
    class AXON_DECLSPEC AxonNetworkPointerBase {
    public:
        AxonNetworkPointerBase() = delete;
        explicit AxonNetworkPointerBase(SynapseInterfacePtr);
        virtual ~AxonNetworkPointerBase() = default;

        GETTER EventSystem::AxonEventManager& getEventManager() { return events; }
    protected:
        virtual AxonMessage toMessage() = 0;
        virtual bool fromMessage(const AxonMessage&) = 0;
        void sendSelf();
        virtual void beforeSend() = 0;
        virtual void onSend() = 0;

        void onSynapseDispatchedMessageReceiveEvent(const SynapseMessageReceivedEvent&);
        virtual void onReceivedNetworkData() = 0;
    private:
        SynapseInterfacePtr synapse = nullptr;
        EventSystem::AxonEventManager events;
    };

    template <typename WrappedType>
    class AXON_DECLSPEC AxonNetworkPointer : public AxonNetworkPointerBase {

    };
} // Networking

