#pragma once
#include "AxonNetworkPointer.hpp"

#include <utility/MessageProcessor.hpp>

#include "MessageProcessor.hpp"

namespace Networking {
    template<typename T>
    AxonNetworkPtr<T>::AxonNetworkPtr(SynapseInterface * synapse, T * ptrTo) :
    AxonNetworkObject(synapse),
    ptr(ptrTo)
    {}

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

    template <typename T>
    void AxonNetworkPtr<T>::onValueChanged() {

    }

    template<typename T>
    void AxonNetworkPtr<T>::dispatchValueChangeEvent() {
        if (!ready()) {
            shouldUpdateSelf = true;
            return;
        }
        synapse->send(this->toMessage());
    }

    template<typename T>
    AxonMessage AxonNetworkPtr<T>::toMessage() const {
        return AxonMessage(
            ptr, sizeof( * ptr ), 0, flags
        );
    }
} // Networking