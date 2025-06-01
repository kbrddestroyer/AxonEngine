#include "AxonNetworkPointer.hpp"

namespace Networking {
    AxonNetworkPointerBase::AxonNetworkPointerBase(SynapseInterfacePtr synapse) :
        synapse(synapse) {
        synapse->getEventManager().subscribe<AxonNetworkPointerBase, SynapseMessageReceivedEvent>(
            onSynapseDispatchedMessageReceiveEvent,
            this
            );
    }

    void AxonNetworkPointerBase::sendSelf() {
        beforeSend();
        AxonMessage message = this->toMessage();
        synapse->send(message);
        onSend();
    }

    void AxonNetworkPointerBase::onSynapseDispatchedMessageReceiveEvent(const SynapseMessageReceivedEvent &event) {
        if (this->fromMessage(event.getMessage())) {
            onReceivedNetworkData();
        }
    }
} // Networking