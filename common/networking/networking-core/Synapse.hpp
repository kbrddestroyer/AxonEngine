#pragma once
#include <thread>
#include <atomic>

#include <AxonUtility.h>
#include <events/AxonEvent.hpp>
#include <networking/message/AxonMessage.hpp>
#include <backends/backend.hpp>

#include <string>


namespace Networking
{
	const uint32_t SYNAPSE_MESSAGE_MAX_SIZE = 1024;

	/**
	* Low-level connection info for convenient storage
	* TODO:
	* - Research replacement
	*/
	struct ConnectionInfo
	{
		std::string			hostname = "";
		uint32_t			port;
	};

	/**
	* TODO: Documenting
	*/
	class AxonNetworkingInternalError
	{
		uint8_t err;
	public:
		inline constexpr AxonNetworkingInternalError(uint8_t err = 0) : err(err) {}
		inline constexpr uint8_t code() const { return err; }
	};

	/**
	* Synapse message when message is recieved from remote host
	*
	* Functions:
	* getMessage() -> AxonMessage: get AxonMessage object
	* getFrom() -> SOCKADDR_IN_T: get sender network info
	*/
	class SynapseMessageReceivedEvent final : public EventSystem::AxonEvent
	{
	private:
		const AxonMessage&		message;
		SOCKADDR_IN_T* from;
	public:
		SynapseMessageReceivedEvent(const AxonMessage& message, SOCKADDR_IN_T* from) : EventSystem::AxonEvent(), message(message)
		{
			this->from = from;
		}

		inline const AxonMessage& getMessage() const { return message; }
		inline SOCKADDR_IN_T* getFrom() const { return from; }
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
	*	- Make async
	*/
	AXON_DECLSPEC class Synapse
	{
	private:
		EventSystem::AxonEventManager events;

		bool				isServer;

		ConnectionInfo		info;
		SOCKET_T socket;
		SOCKADDR_IN_T socket_info;

		/** Default creation is restriced */
		Synapse() = default;
	protected:
		std::atomic<bool>	isAlive = false;
	public:
		/** Initializes Synapse in server mode */
		Synapse(uint32_t);
		/** Initialize Synapse in client mode */
		Synapse(const ConnectionInfo&);
		~Synapse();

		inline bool alive() const { return isAlive.load(); }
		virtual void start();

		void send(const AxonMessage&);
		void sendTo(const AxonMessage&, const SOCKADDR_IN_T*);
		void listen();

		void onMessageReceived(const AxonMessage&, SOCKADDR_IN_T*);

		constexpr inline EventSystem::AxonEventManager& getEventManager() { return events; }
	};

	AXON_DECLSPEC class AsyncSynapse : public Synapse
	{
	private:
		std::thread proc;
	public:
		/** Initializes Synapse in server mode */
		inline AsyncSynapse(uint32_t port) : Synapse(port) {}
		/** Initialize Synapse in client mode */
		inline AsyncSynapse(const ConnectionInfo& info) : Synapse(info) {}
		
		~AsyncSynapse();

		void start() override;
		void kill();
	};
}

/* Synapse.hpp */
