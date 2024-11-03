#pragma once

#ifndef AXON_SERVER
#define AXON_SERVER
#endif  // AXON_SERVER

#include <server/server_connection.h>


namespace Axon
{
	class AXON_EXPORT Server
	{
	private:
		Axon::Connection::ServerConnectionHandler* handler;

	public:
		Server();
		~Server();
	};
}