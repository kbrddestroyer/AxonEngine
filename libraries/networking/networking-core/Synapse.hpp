#pragma once
#include <thread>
#include <atomic>
#include <string>

#include <AxonUtility.h>
#include <events/AxonEvent.hpp>
#include <backends/backend.hpp>
#include <networking/message/AxonMessage.hpp>
#include <networking/utility/MessagePool.hpp>

namespace Networking
{
	constexpr uint32_t SYNAPSE_MESSAGE_MAX_SIZE = 1024;

	enum ConnectionMode
	{
		UDP = SOCK_DGRAM,
		TCP = SOCK_STREAM
	};

	/**
	* Low-level connection info for convenient storage
	* TODO:
	* - Research replacement
	*/
	struct ConnectionInfo
	{
		std::string	hostname;
		uint32_t	port = 10432;
	};

	/**
	* TODO: Documenting
    	*/
    	class AxonNetworkingInternalError : public std::exception
	{
		uint8_t err;
	public:
		explicit AxonNetworkingInternalError(uint8_t err = 0) : err(err) {}
		GETTER  constexpr uint8_t code() const { return err; }
	};

	/**
	* Synapse message when message is recieved from remote host
	*
	* Functions:
	* getMessage() -> AxonMessage: get AxonMessage object
	* getFrom() -> SOCKADDR_IN_T: get sender network info
	*/
	class AXON_DECLSPEC SynapseMessageReceivedEvent final : public EventSystem::AxonEvent
	{
	public:
		SynapseMessageReceivedEvent(const AxonMessage& message, SOCKADDR_IN_T* from) : EventSystem::AxonEvent(), message(message)
		{
			this->from = from;
		}

		GETTER const AxonMessage& getMessage() const { return message; }
		GETTER SOCKADDR_IN_T* getFrom() const { return from; }
	private:
		const AxonMessage&	message;
		SOCKADDR_IN_T* from;
	};

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
