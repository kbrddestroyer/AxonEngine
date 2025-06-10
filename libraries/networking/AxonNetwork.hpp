#pragma once

#include <AxonUtility.h>
#include <networking/AxonNetworkObject.hpp>
#include <networking/synapse/BasicSynapse.hpp>

#include <memory>
#include <vector>
#include <initializer_list>

namespace Networking
{
    class AXON_DECLSPEC AxonNetwork {
    public:
        AxonNetwork() = default;

        template <class Synapse>
        void initialize();

        template <class Object, typename... Args>
        AxonNetworkObject * createObject(Args&&... args);
        void registerObject(AxonNetworkObject *);

        static AxonNetwork & instance();
    private:
        std::unique_ptr<SynapseInterface> synapse;
        std::vector<std::unique_ptr<AxonNetworkObject>> objectRegistry;
    };
}

#include "AxonNetwork.ipp"

/* AxonNetwork.hpp */
