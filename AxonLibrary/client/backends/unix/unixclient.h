#pragma once
#if defined(__unix__) || __APPLE__

#include <AxonEngine.h>
#include <AxonClient.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <cstdint>

namespace Axon::Backends::Unix
{
class UnixUDPClient : public Axon::Client::ClientConnectionHandler {
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
