#include "AxonNetworkObject.hpp"
#include <networking/utility/MessageProcessor.hpp>



Networking::AxonNetworkObject::AxonNetworkObject(SynapseInterface * synapse) :
    synapse(synapse) {
    if (synapse) {

        synapse->getEventManager().subscribe<
            AxonNetworkObject, SynapseMessageReceivedEvent
            >(&AxonNetworkObject::onIDResolved, this);
        resolveNetworkID();
    }
}

void Networking::AxonNetworkObject::resolveNetworkID() {
    this->clientID = MessageProcessor::RequestUniqueIDProto::generateID();

    MessageProcessor::RequestUniqueIDProto request = {
        clientID, 0
    };

    AxonMessage msg(&request, sizeof(request), 0, NETOBJ_INI);
    synapse->send(msg);
}

void Networking::AxonNetworkObject::onIDResolved(const SynapseMessageReceivedEvent &event) {
    const AxonMessage & message = event.getMessage();

    if (!message.hasFlag(NETOBJ_REPL)) return;

    MessageProcessor::RequestUniqueIDProto repl = * static_cast < MessageProcessor::RequestUniqueIDProto * >( message.getMessage() );
    if (repl.clientSideID != this->clientID) return;

    this->serverID = repl.serverSideID;
}


