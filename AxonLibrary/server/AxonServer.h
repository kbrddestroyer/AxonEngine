#pragma once

#ifndef AXON_SERVER
#define AXON_SERVER
#endif  // AXON_SERVER

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