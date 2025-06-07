#pragma once
#include <AxonUtility.h>
#include <synapse/BasicSynapse.hpp>
#include <synapse/utils/SynapseEvents.hpp>
#include <events/AxonEvent.hpp>

namespace Networking {
    template<typename T>
    class AXON_DECLSPEC AxonNetworkPtr {
        static_assert(!std::is_abstract<T>::value);
        static_assert(
                !std::is_trivially_copyable_v<T>(),
                "Type must be trivially copyable to be compatible with Axon Network Pointer");
    public:
        AxonNetworkPtr() = delete;

        virtual ~AxonNetworkPtr() = default;

        GETTER T *get() const { return ptr; }

        void set(T);

        T &operator*() const;

        T *operator->() const;

    protected:
        virtual void onValueChanged();

        virtual void dispatchValueChangeEvent();

        AxonMessage toMessage() const;

    private:
        T *ptr = nullptr;
        uint8_t flags = 0;
        SynapseInterface *synapse = nullptr;
        Socket connected = {};
        EventSystem::AxonEventManager events;
    };
}

#include "AxonNetworkPointer.ipp"
