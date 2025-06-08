#pragma once
#include "AxonNetworkPointer.hpp"

#include <utility/MessageProcessor.hpp>

#include "MessageProcessor.hpp"

namespace Networking {
    template<typename T>
    AxonNetworkPtr<T>::AxonNetworkPtr(SynapseInterface * synapse, T * ptrTo) :
    synapse(synapse),
    ptr(ptrTo)
    {
        synapse->getEventManager().subscribe<
                AxonNetworkPtr, SynapseMessageReceivedEvent
            >(&AxonNetworkPtr::onRepl, this);

        initialize();
    }

    template <typename T>
    void AxonNetworkPtr<T>::set(T val) {
        *ptr = val;

        dispatchValueChangeEvent();
    }

    template<typename T>
    T &AxonNetworkPtr<T>::operator*() const {
        return *ptr;
    }

    template<typename T>
    T *AxonNetworkPtr<T>::operator->() const {
        return ptr;
    }

    template<typename T>
    void AxonNetworkPtr<T>::onRepl(const SynapseMessageReceivedEvent & event) {
        const AxonMessage & message = event.getMessage();

        if (!message.hasFlag(NETOBJ_REPL)) return;

        MessageProcessor::RequestUniqueIDProto repl = * static_cast < MessageProcessor::RequestUniqueIDProto * >( message.getMessage() );
        if (repl.clientSideID != this->clientID) return;

        this->serverID = repl.serverSideID;

        if (shouldUpdateSelf) {
            dispatchValueChangeEvent();
            shouldUpdateSelf = false;

            synapse->getEventManager().subscribe<
                    AxonNetworkPtr, SynapseMessageReceivedEvent
                >(&AxonNetworkPtr::onRepl, this);
        }
    }

    template <typename T>
    void AxonNetworkPtr<T>::onValueChanged() {

    }

    template<typename T>
    void AxonNetworkPtr<T>::dispatchValueChangeEvent() {
        if (!ready()) {
            shouldUpdateSelf = true;
            return;
        }

        synapse->sendTo(this->toMessage(), connected);
    }

    template<typename T>
    void AxonNetworkPtr<T>::initialize() {
        this->clientID = MessageProcessor::RequestUniqueIDProto::generateID();

        MessageProcessor::RequestUniqueIDProto request = {
            clientID, 0
        };

        AxonMessage msg(&request, sizeof(request), 0, NETOBJ_INI);
        synapse->send(msg);
    }

    template<typename T>
    AxonMessage AxonNetworkPtr<T>::toMessage() const {
        return AxonMessage(
            ptr, sizeof( * ptr ), 0, flags
        );
    }
} // Networking