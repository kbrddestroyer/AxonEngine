#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(__NT__)
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
			SOCKET			server_socket;
			SOCKET			client_socket;
			SOCKADDR_IN		server;
#pragma endregion
#pragma region SERVER_CONFIGURATION
			const char* addr;
			std::uint16_t port;

			bool isRunning;
#pragma endregion
		public:
			WinUDPSocket(const char* = "localhost", uint16_t = 7777);
			~WinUDPSocket();

			bool Startup();
			void Listen();
		};

		class WinUDPClientConnection
		{

		};
	}
}
#endif