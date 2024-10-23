#pragma once
#include <AxonEngine.h>

#include <WinSock2.h>
#include <cstdint>

namespace Axon
{
	namespace Backends
	{
		namespace Windows
		{
			class WinUDPClient
			{
			private:
				uint32_t sockfd;
				SOCKADDR_IN server;
				SOCKET client_socket;
				uint16_t port;
				WSADATA	ws;
			public:
				WinUDPClient(uint16_t port = 7777) : port(port) {}

				bool Startup();
				void SendTo();
			};
		}
	}
}
