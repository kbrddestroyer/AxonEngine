#pragma once
#if defined(__unix__) || __APPLE__

#include <AxonEngine.h>
#include <AxonClient.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstdint>

namespace Axon::Backends::Unix
{
class UnixUDPClient : public Axon::Client::ClientConnectionHandler {
    private:
        int32_t sockfd;
        sockaddr_in server;
    public:
        explicit UnixUDPClient(char* hostname = "localhost", Axon::Connection::AXON_PORT port = 7777);

        bool Initialize() override;
        bool SendUserMessage(Axon::Connection::UDPMessage message) override;
    };
}
#endif
