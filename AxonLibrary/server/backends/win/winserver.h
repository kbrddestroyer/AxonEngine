#pragma once
#include <Axon.h>

#if defined(WINDOWS_PLATFORM)
#include <AxonServer.h>

#include <WinSock2.h>
#include <cstdint>
#include <map>

#include <server/server_constants.h>

#pragma comment(lib,"ws2_32.lib")


namespace Axon::Backends::Windows
{
	/// <summary>
	/// Windows server socket implementation
	/// Uses UDP protocol
	/// </summary>
	class AXON_DECLSPEC WinUDPConnectionHandler : public Axon::Connection::ServerConnectionHandler
	{
	private:
#pragma region SERVER_DATA
		WSADATA			ws;
		SOCKET			server_socket;
		SOCKADDR_IN		server;

		std::map<uint64_t, SOCKADDR_IN> connections;
#pragma endregion
#pragma region SERVER_CONFIGURATION
		Axon::Connection::AXON_PORT		port;
		bool			isRunning;
#pragma endregion
	public:
		explicit WinUDPConnectionHandler(Axon::Connection::AXON_PORT port = 7777);
		~WinUDPConnectionHandler();

		bool SendUserMessage(char*, size_t, uint64_t);
	protected:
		bool Initialize();
		void Listen();
	};
}
#endif