#pragma once
#include "AxonNetworkPointer.hpp"

namespace Networking {
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
        synapse->sendTo(this->toMessage(), connected);
    }

    template<typename T>
    AxonMessage AxonNetworkPtr<T>::toMessage() const {
        return AxonMessage(
            ptr, sizeof( * ptr ), 0, flags
        );
    }
} // Networking