#pragma once
#include "message/AxonMessage.hpp"

#include <events/AxonEvent.hpp>
#include <backends/backend.hpp>
#include <AxonUtility.h>
#include <string>


namespace Networking
{
	/**
	* Low-level connection info for convinient storage
	*/
	struct ConnectionInfo
	{
		std::string			hostname = "";
		uint32_t			port;
	};

	AXON_DECLSPEC class AxonNetworkingInternalError
	{
		uint8_t err;
		AxonNetworkingInternalError() = default;
	public:
		AxonNetworkingInternalError(uint8_t);

		uint8_t code();
	};

	/**
	* Synaps message when message is recieved from remote host
	*/
	AXON_DECLSPEC class SynapsMessageReceivedEvent : public EventSystem::AxonEvent
	{
	private:
		AxonMessage		message;
		SOCKADDR_IN_T*	from;
	
	public:
		SynapsMessageReceivedEvent(AxonMessage& message, SOCKADDR_IN_T* from) : EventSystem::AxonEvent()
		{
			this->message = message;
			this->from = from;
		}

		inline AxonMessage getMessage() { return message; }
		inline SOCKADDR_IN_T* getFrom() { return from; }
	};
	

	/**
	* Axon connection handler
	* Specifies connection handling to one remote host
	* 
	* EventSystem API:
	* WIP currently
	*/
	AXON_DECLSPEC class Synaps
	{
	private:
		EventSystem::AxonEventManager events;

		bool				isServer;
		bool				isAlive = false;
		ConnectionInfo		info;
		SOCKET_T socket;
		SOCKADDR_IN_T socket_info;

		/** Default creation is restriced */
		Synaps() = default;
	public:
		/** Initializes Synaps in server mode */
		Synaps(uint32_t);
		/** Initialize Synaps in client mode */
		Synaps(const ConnectionInfo&);
		~Synaps();

		void send(AxonMessage&);
		void listen();

		void onMessageReceived(AxonMessage&, SOCKADDR_IN_T*);

		constexpr inline EventSystem::AxonEventManager& getEventManager() { return events; }
	};

	/**
	* Networking core
	* Handles synaps connections
	*/
	AXON_DECLSPEC class AxonNetwork
	{
	public:
		AxonNetwork() = default;
	};
}

/* AxonNetwork.hpp */
