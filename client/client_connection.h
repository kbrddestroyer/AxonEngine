#pragma once

#include <AxonEngine.h>

namespace Axon::Client {
    class ClientConnectionHandler {
    protected:
        Axon::Connection::AXON_PORT port;
        char* hostname;
        bool isRunning = false;
    protected:
        virtual bool Initialize() = 0;

        virtual void SendMessage() = 0;
    public:
        bool Startup();
    };
}
