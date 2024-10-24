#pragma once
#include <AxonEngine.h>

namespace Axon::Client {
    class ClientConnectionHandler {
    protected:
        Axon::Connection::AXON_PORT port;
        char* hostname;
        bool isRunning = false;
    private:
        ClientConnectionHandler() = default;
    public:
        ClientConnectionHandler(char* hostname, Axon::Connection::AXON_PORT port = 7777);
        ~ClientConnectionHandler() = default;
    protected:
        virtual bool Initialize() = 0;
        virtual void SendUserMessage(Axon::Connection::UDPMessage message) = 0;
    public:
        bool Startup();
    };
}
