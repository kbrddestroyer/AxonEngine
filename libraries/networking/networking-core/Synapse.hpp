#pragma once
#include <thread>
#include <atomic>
#include <string>

#include <AxonUtility.h>
#include <events/AxonEvent.hpp>
#include <backends/backend.hpp>
#include <networking/message/AxonMessage.hpp>
#include <networking/utility/MessagePool.hpp>
#include <message/AxonMessage.hpp>
#include <utility/MessagePool.hpp>

#include <networking-core/SynapseUtility.hpp>
#include <networking-core/SynapseEvents.hpp>

namespace Networking
{
	/**
	* Axon connection handler
	* Specifies connection handling to one remote host
	*
	* EventSystem API:
	* WIP currently
	*
	* TODO:
	*	- Documenting
	*/
	template <ConnectionMode> class AXON_DECLSPEC Synapse
	{
		const size_t MAX_MESSAGE = 1024;
	public:
		/** Default creation is restricted */
		Synapse() = delete;
		/** Initializes Synapse in server mode */
		explicit Synapse(uint32_t);
		/** Initialize Synapse in client mode */
		explicit Synapse(const ConnectionInfo&);

		virtual ~Synapse();

		bool alive() const { return isAlive.load(); };

		virtual void start();
		virtual void send(const AxonMessage&);
		virtual void sendTo(const SerializedAxonMessage&, const SOCKADDR_IN_T*) const;
		virtual void listen();
        virtual void update();
		virtual void onMessageReceived(const AxonMessage&, SOCKADDR_IN_T*);

		EventSystem::AxonEventManager& getEventManager() { return events; }
        void sendPooled(const AxonMessage&, const SOCKADDR_IN_T* = nullptr) const;
    protected:
		EventSystem::AxonEventManager events;
		std::unique_ptr<MessagePoolBase> pool = std::make_unique<MessagePoolBase>();
		std::atomic<bool> isAlive = false;
	private:
		bool			    isServer;
		ConnectionInfo		info;
		Socket              socket;
	};

	template <ConnectionMode mode>
	class AXON_DECLSPEC AsyncSynapse : public Synapse<mode>
	{
	public:
		/** Initializes Synapse in server mode */
		explicit AsyncSynapse(uint32_t port) : Synapse<mode>(port) {}

		/** Initialize Synapse in client mode */
		explicit AsyncSynapse(const ConnectionInfo& info) : Synapse<mode>(info) {}

		~AsyncSynapse() override;

		void start() final;
		void kill();
	private:
		std::thread proc;
	};
}

#include "Synapse.ipp"

/* Synapse.hpp */
