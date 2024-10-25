#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(__NT__)
#include <AxonTypes.h>
#include <AxonServer.h>

#include <WinSock2.h>
#include <cstdint>

#pragma comment(lib,"ws2_32.lib") 

namespace Axon::Backends::Windows
{
	/// <summary>
	/// Windows server socket implementation
	/// Uses UDP protocol
	/// </summary>
	class WinUDPConnectionHandler : public Axon::Connection::ServerConnectionHandler
	{
	private:
#pragma region SERVER_DATA
		WSADATA			ws;
		SOCKET			server_socket;
		SOCKET			client_socket;
		SOCKADDR_IN		server;
#pragma endregion
#pragma region SERVER_CONFIGURATION
		Axon::Connection::AXON_PORT		port;
		bool			isRunning;
#pragma endregion
	public:
		explicit WinUDPConnectionHandler(Axon::Connection::AXON_PORT port = 7777);
		~WinUDPConnectionHandler();

		bool SendUserMessage(Axon::Connection::ServerUDPMessage);
	protected:
		bool Initialize();
		void Listen();
	};
}
#endif