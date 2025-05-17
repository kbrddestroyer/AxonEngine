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
	template <ConnectionMode, SynapseMode> class AXON_DECLSPEC BasicSynapse
	{
		const size_t MAX_MESSAGE = 1024;
	public:
		/** Default creation is restricted */
		BasicSynapse() = delete;
		/** Initializes Synapse in server mode */
		explicit BasicSynapse(uint32_t);
		/** Initialize Synapse in client mode */
		explicit BasicSynapse(const ConnectionInfo&);

		virtual ~BasicSynapse();

		bool alive() const { return isAlive.load(); };

		virtual void start();
		virtual void send(const AxonMessage&);
		virtual void sendTo(const SerializedAxonMessage&, const SOCKADDR_IN_T*) const;

		// This function should be instanced for each connection type
		virtual void listen() {}

		// --- INTERFACE FUNCTIONS --- //

        virtual void update() {}
		virtual void onMessageReceived(const AxonMessage&, SOCKADDR_IN_T*) {};
    protected:
		std::atomic<bool>	isAlive = false;
		ConnectionInfo		connectionInfo;
		Socket              socketInfo;
	};


	template <ConnectionMode conn, SynapseMode mode>
	class AXON_DECLSPEC Synapse : public BasicSynapse<conn, mode> {
	public:
#pragma region CONSTRUCTING
		/** Initializes Synapse in server mode */
		explicit Synapse(uint32_t port) : BasicSynapse<conn, mode>(port) {}
		/** Initialize Synapse in client mode */
		explicit Synapse(const ConnectionInfo &info) : BasicSynapse<conn, mode>(info) {}

		~Synapse() override = default;
#pragma endregion

#pragma region INTERFACE

		void update() override;
		void onMessageReceived(const AxonMessage&, SOCKADDR_IN_T*) override;

		EventSystem::AxonEventManager& getEventManager() { return events; }
		void sendPooled(const AxonMessage&, const SOCKADDR_IN_T* = nullptr) const;
#pragma endregion
	protected:
		EventSystem::AxonEventManager events;
		std::unique_ptr<MessagePoolBase> pool = std::make_unique<MessagePoolBase>();
	};

	template <ConnectionMode conn, SynapseMode mode>
	class AXON_DECLSPEC AsyncSynapse : public Synapse<conn, mode>
	{
	public:
		/** Initializes Synapse in server mode */
		explicit AsyncSynapse(uint32_t port) : Synapse<conn, mode>(port) {}

		/** Initialize Synapse in client mode */
		explicit AsyncSynapse(const ConnectionInfo& info) : Synapse<conn, mode>(info) {}

		~AsyncSynapse() override;

		void start() final;
		void kill();
	private:
		std::thread proc;
	};
}

#include "Synapse.ipp"

/* Synapse.hpp */
