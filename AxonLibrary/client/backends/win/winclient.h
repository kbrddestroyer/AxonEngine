#pragma once
#include <Axon.h>


#if defined(WINDOWS_PLATFORM)

#include <AxonClient.h>
#include <cstdint>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

// In debug state

namespace Axon::Backends::Windows
{
	class WinUDPClient : public Axon::Connection::ClientConnectionHandler
	{
	private:
		uint32_t		sockfd = INVALID_SOCKET;
		SOCKADDR_IN		server;
		SOCKET			client_socket;
		WSADATA			ws;
	private:
		WinUDPClient() = default;
	public:
		WinUDPClient(char* hostname, Axon::Connection::AXON_PORT port = 7777);
		~WinUDPClient();
	
	protected:
		bool Initialize() override;
		void SendUDPMessage(char*, size_t) override;
	};
}
#endif
