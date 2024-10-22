#pragma once

#ifdef _WIN32 || _WIN64
#include <WinSock2.h>
#include <cstdint>


#pragma comment(lib,"ws2_32.lib") 

namespace Axon
{
	namespace Windows
	{
		/// <summary>
		/// Windows server socket implementation
		/// Uses UDP protocol
		/// </summary>
		class WinUDPSocket
		{
		private:
#pragma region SERVER_DATA
			WSADATA			ws;
			SOCKET			client_socket;
			SOCKADDR_IN		server;
#pragma endregion
#pragma region SERVER_CONFIGURATION
			const char* addr;
			std::uint16_t port;
#pragma endregion
		public:
			WinUDPSocket(const char* = "localhost", uint16_t = 7777);
			~WinUDPSocket();

			bool Startup();
		};
	}
}
#endif