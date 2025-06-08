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
                std::is_literal_type<T>::value || !std::is_trivially_copyable<T>::value,
                "Type must be trivially copyable to be compatible with Axon Network Pointer");
    public:
        AxonNetworkPtr() = delete;

        AxonNetworkPtr(SynapseInterface *, T *);

        virtual ~AxonNetworkPtr() = default;

        GETTER T *get() const { return ptr; }
        GETTER bool ready() const noexcept { return serverID != 0; }

        void set(T);

        T &operator*() const;

        T *operator->() const;

    protected:
        void onRepl(const SynapseMessageReceivedEvent &);

        virtual void onValueChanged();

        virtual void dispatchValueChangeEvent();

        void initialize();

        AxonMessage toMessage() const;
    private:
        T *ptr = nullptr;
        uint8_t flags = 0;

        uint32_t clientID = 0;
        uint64_t serverID = 0;

        bool shouldUpdateSelf = false;

        SynapseInterface *synapse = nullptr;
        Socket connected = {};
        EventSystem::AxonEventManager events;
    };
}

#include "AxonNetworkPointer.ipp"
