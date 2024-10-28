#pragma once

#if defined(__unix__) || __APPLE__ || __APPLE_CC__
#include <AxonEngine.h>
#include <AxonServer.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace Axon {
    namespace Backends {
        namespace Unix {
            class UnixUDPConnectionHandler : public Axon::Connection::ServerConnectionHandler {
            private:
#pragma region SOCKET
                sockaddr_in server, client;
                int32_t sockfd;
#pragma endregion
            public:
                explicit UnixUDPConnectionHandler(Axon::Connection::AXON_PORT port = 7777);

            protected:
                bool Initialize() override;
                void Listen() override;
                bool SendUserMessage(char*, size_t, uint64_t) override;
            };
        }
    }
}
#endif
