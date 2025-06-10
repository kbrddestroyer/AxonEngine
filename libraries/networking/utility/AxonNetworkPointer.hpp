#pragma once
#include <AxonUtility.h>
#include <synapse/BasicSynapse.hpp>
#include <synapse/utils/SynapseEvents.hpp>
#include <events/AxonEvent.hpp>
#include <networking/AxonNetworkObject.hpp>

namespace Networking {
    template<typename T>
    class AXON_DECLSPEC AxonNetworkPtr : public AxonNetworkObject {
        static_assert(!std::is_abstract<T>::value);
        static_assert(
                std::is_trivially_copyable<T>::value,
                "Type must be trivially copyable to be compatible with Axon Network Pointer");

        struct payload {
            T val;
            uint64_t id;
        };
    public:
        AxonNetworkPtr() = delete;
        AxonNetworkPtr(SynapseInterface *, T *);
        ~AxonNetworkPtr() override = default;

        GETTER T * get() const { return ptr; }
        void set(T);
        T &operator* () const;
        T *operator->() const;
    protected:
        virtual void onValueChanged();
        virtual void dispatchValueChangeEvent();
        AxonMessage toMessage() const;
    private:
        T *ptr = nullptr;
        uint8_t flags = 0;
        bool shouldUpdateSelf = false;
        EventSystem::AxonEventManager events;
    };
}

#include "AxonNetworkPointer.ipp"
