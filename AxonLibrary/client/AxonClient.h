#pragma once

#include <Axon.h>
#include <client/client_connection.h>


namespace Axon
{
	class AXON_DECLSPEC Client
	{
	private:
		Axon::Connection::ClientConnectionHandler* handler;
	private:
		Client() = default;
	public:
		Client(char*, Axon::Connection::AXON_PORT);
		~Client();

		void SendUserMessage(Axon::Connection::Message& message);
	};
}