#pragma once
#include <AxonUtility.h>
#include <networking/synapse/BasicSynapse.hpp>


namespace Networking {
    class AXON_DECLSPEC AxonNetworkObject {
    public:
        AxonNetworkObject() = delete;
        AxonNetworkObject( SynapseInterface * );
        virtual ~AxonNetworkObject();

        WGETTER( bool ready() ) { return serverID != 0; }
        WGETTER( uint64_t id() ) { return serverID; }
    protected:
        void resolveNetworkID();
        void onIDResolved(const SynapseMessageReceivedEvent&);
    private:
        uint64_t serverID = 0;
        uint32_t clientID = 0;
    protected:
        SynapseInterface * synapse = nullptr;
    };
}
