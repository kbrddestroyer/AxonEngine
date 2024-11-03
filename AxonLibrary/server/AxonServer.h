#pragma once
#include <server/server_connection.h>


namespace Axon
{
	class AXON_DECLSPEC Server
	{
	private:
		Axon::Connection::ServerConnectionHandler* handler;

	public:
		Server();
		~Server();
	};
}