#pragma once
#include "message/AxonMessage.hpp"
#include <events/AxonEvent.hpp>
#include <backends/backend.hpp>

#include <string>


namespace Networking
{
	/**
	* Low-level connection info for convinient storage
	*/
	struct ConnectionInfo
	{
		std::string			hostname;
		uint32_t			port;
	};

	class AxonNetworkingInternalError
	{
		uint8_t err;
		AxonNetworkingInternalError() = default;
	public:
		AxonNetworkingInternalError(uint8_t);

		uint8_t code();
	};

	/**
	* Axon connection handler
	* Specifies connection handling to one remote host
	*/
	class Synaps
	{
	private:
		bool				isServer;
		ConnectionInfo		info;

		SOCKET_T*			local;
		SOCKADDR_IN_T*		remote;

		/** Default creation is restriced */
		Synaps() = default;
	public:
		/** Initializes Synaps in server mode */
		Synaps(uint32_t);
		/** Initialize Synaps in client mode */
		Synaps(const ConnectionInfo&);

		void send(const AxonMessage&);
		void listen();

		void onMessageReceived(const AxonMessage&, SOCKADDR_IN_T*);
	};

	/**
	* Networking core
	* Handles synaps connections
	*/
	class AxonNetwork
	{
	public:
		AxonNetwork();


	};
}

/* AxonNetwork.hpp */
