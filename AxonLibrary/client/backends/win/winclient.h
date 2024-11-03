#pragma once
#if defined(_WIN32)
#include <AxonClient.h>

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <cstdint>

#pragma comment(lib,"ws2_32.lib")

// In debug state

namespace Axon::Backends::Windows
{
	class WinUDPClient : public Axon::Client::ClientConnectionHandler
	{
	private:
		uint32_t		sockfd = INVALID_SOCKET;
		SOCKADDR_IN		server;
		SOCKET			client_socket;
		WSADATA			ws;
	private:
		WinUDPClient() = default;
	public:
		explicit WinUDPClient(char* hostname, Axon::Connection::AXON_PORT port = 7777);
		~WinUDPClient();
	
	protected:
		bool Initialize() override;
		void SendUDPMessage(char*, size_t) override;
	};
}
#endif
