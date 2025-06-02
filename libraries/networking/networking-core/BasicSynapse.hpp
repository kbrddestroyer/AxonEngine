/**
 * BasicSynapse is a class that contains backend calls and basic message handling. It has no advanced functionality
 * such as event dispatching, thread managing, etc.
 *
 * It was created to separate low-level implementations from high level Synapse interface.
 * This class still can be used to create custom Synapse. Just override onMessageReceived and update methods.
 */

#pragma once
#include <atomic>


namespace Networking {
    /**
    * Basic connection handler
    * Specifies one-to-one connection for client and one-to-many for server.
    *
    * @tparam conn connection mode (TCP|UDP)
    * @tparam mode synapse mode (CLIENT|SERVER)
    */
    template <ConnectionMode conn, SynapseMode mode> class AXON_DECLSPEC BasicSynapse
    {
    public:
        /** Default creation is restricted */
        BasicSynapse() = delete;
        /** Initializes Synapse in server mode */
        explicit BasicSynapse(uint32_t);
        /** Initialize Synapse in client mode */
        explicit BasicSynapse(const ConnectionInfo&);

        virtual ~BasicSynapse();

        GETTER bool alive() const { return isAlive.load(); };
        virtual void kill() { isAlive.store(false); }

        virtual void start();
        virtual void send(AxonMessage&);
        virtual void sendTo(AxonMessage&, const SOCKADDR_IN_T*);
        virtual void sendTo(const SerializedAxonMessage&, const SOCKADDR_IN_T*) const;

        // This function should be instanced for each connection type
        virtual void listen() {}
        virtual void update() {}
        virtual void onMessageReceived(const AxonMessage&, SOCKADDR_IN_T*) {};

        void processIncomingMessage(SerializedAxonMessage, SOCKADDR_IN_T*);
    protected:
        std::atomic<bool>	isAlive = false;  // TODO: move this from here
        ConnectionInfo		connectionInfo;
        Socket              socketInfo;
    };
}

#include "BasicSynapse.ipp"

// BasicSynapse.hpp
