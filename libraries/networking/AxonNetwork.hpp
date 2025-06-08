#pragma once

#include <AxonUtility.h>
#include <networking/synapse/BasicSynapse.hpp>

#include <memory>

namespace Networking
{
    class AXON_DECLSPEC AxonNetworkManager {
    public:
        AxonNetworkManager() = default;

        template <class SynapseType>
        void initialize();

        virtual uint64_t getUniqueIDInNetwork() = 0;
    private:
        std::unique_ptr<SynapseInterface> synapse;
    };
}

#include "AxonNetwork.ipp"

/* AxonNetwork.hpp */
