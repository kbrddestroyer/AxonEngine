#pragma once
#include <Axon.h>


#if defined(UNIX_PLATFORM)

#include <AxonClient.h>

#include <netinet/in.h>
#include <cstdint>


namespace Axon::Backends::Unix
{
class UnixUDPClient : public Axon::Connection::ClientConnectionHandler {
    private:
        int32_t sockfd;
        sockaddr_in server;
    public:
        UnixUDPClient(char* hostname = "localhost", Axon::Connection::AXON_PORT port = 7777);

        ~UnixUDPClient();

        bool Initialize() override;
        void SendUDPMessage(char*, size_t) override;
    };
}
#endif
