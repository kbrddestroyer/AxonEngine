#pragma once
#if defined(_WIN32)
#include <AxonClient.h>

#include <WinSock2.h>
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
		uint16_t		port;
		WSADATA			ws;
	private:
		WinUDPClient() = default;
	public:
		explicit WinUDPClient(char* hostname, uint16_t port = 7777);
		~WinUDPClient();
	
	protected:
		bool Initialize() override;
		void SendUserMessage(Axon::Connection::UDPMessage message) override;
	};
}
#endif
