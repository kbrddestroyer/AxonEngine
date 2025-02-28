#pragma once
#include <thread>
#include <atomic>

#include <AxonUtility.h>
#include <events/AxonEvent.hpp>
#include <networking/message/AxonMessage.hpp>
#include <backends/backend.hpp>


namespace Networking
{
	const uint32_t SYNAPS_MESSAGE_MAX_SIZE = 1024;

	/**
	* Low-level connection info for convinient storage
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
	* Synaps message when message is recieved from remote host
	*
	* Functions:
	* getMessage() -> AxonMessage: get AxonMessage object
	* getFrom() -> SOCKADDR_IN_T: get sender network info
	*/
	class SynapsMessageReceivedEvent final : public EventSystem::AxonEvent
	{
	private:
		AxonMessage		message;
		SOCKADDR_IN_T* from;
	public:
		SynapsMessageReceivedEvent(AxonMessage& message, SOCKADDR_IN_T* from) : EventSystem::AxonEvent()
		{
			this->message = message;
			this->from = from;
		}

		inline AxonMessage getMessage() const { return message; }
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
	AXON_DECLSPEC class Synaps
	{
	private:
		EventSystem::AxonEventManager events;

		bool				isServer;

		ConnectionInfo		info;
		SOCKET_T socket;
		SOCKADDR_IN_T socket_info;

		/** Default creation is restriced */
		Synaps() = default;
	protected:
		std::atomic<bool>	isAlive = false;
	public:
		/** Initializes Synaps in server mode */
		Synaps(uint32_t);
		/** Initialize Synaps in client mode */
		Synaps(const ConnectionInfo&);
		~Synaps();

		inline bool alive() { return isAlive.load(); }
		virtual void start();

		void send(AxonMessage&);
		void sendTo(AxonMessage&, SOCKADDR_IN_T*);
		void listen();

		void onMessageReceived(AxonMessage&, SOCKADDR_IN_T*);

		constexpr inline EventSystem::AxonEventManager& getEventManager() { return events; }
	};

	AXON_DECLSPEC class AsyncSynaps : public Synaps
	{
	private:
		std::thread proc;
	public:
		/** Initializes Synaps in server mode */
		inline AsyncSynaps(uint32_t port) : Synaps(port) {}
		/** Initialize Synaps in client mode */
		inline AsyncSynaps(const ConnectionInfo& info) : Synaps(info) {}
		
		~AsyncSynaps();

		void start() override;
		void kill();
	};
}

/* Synaps.hpp */
