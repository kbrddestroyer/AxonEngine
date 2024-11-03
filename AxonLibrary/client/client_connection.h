/*
* WARNING: Dynamic ip resolving algorythm is delegated to backend
* Please, implement getaddrinfo() usage
*/

#pragma once
#include <AxonMasterInclude.h>


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
        virtual void SendUDPMessage(char*, size_t) = 0;
    public:
        bool SendUserMessage(const Axon::Connection::UDPMessage& message);
        bool Startup();
    };
}
