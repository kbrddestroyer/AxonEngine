#pragma once
#include <WinSock2.h>
#include <cstdint>

#pragma comment(lib,"ws2_32.lib")

// In debug state

namespace Axon
{
	namespace Backends
	{
		namespace Windows
		{
			class WinUDPClient
			{
			private:
				uint32_t sockfd = INVALID_SOCKET;
				SOCKADDR_IN server;
				SOCKET client_socket;
				uint16_t port;
				WSADATA	ws;
			public:
				explicit WinUDPClient(uint16_t port = 7777);
				~WinUDPClient();

				bool Startup();
				void SendTo();
			};
		}
	}
}
