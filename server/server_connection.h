#pragma once
#include <AxonTypes.h>

#include <cstdint>
#include <vector>
#include <map>

#if defined(_WIN32)
#include <WinSock2.h>
#elif defined(__unix__) || __APPLE__
#include <netinet/in.h>
#endif


namespace Axon::Connection
{
    enum class ServerUDPDefaultTags
    {
        CONTROL = 0
    };

    struct ServerUDPMessage
    {
        // Package struct:
        // CLIENT_IP DATA_TAG [DATA] sizeof(DATA)

        uint64_t connectionID;
        UDPMessage payload;
    };

    struct ServerConnection
    {
        // Should contain useful information about user

        uint32_t ip_addr;
    };

    class ServerConnectionHandler {
    private:
        std::map<uint32_t, ServerConnection> mConnections;
    protected:
#pragma region SERVER_CONFIGURATION
        Axon::Connection::AXON_PORT port;

        bool isRunning;
#pragma endregion
    public:
        explicit ServerConnectionHandler(uint16_t = 7777);

        virtual ~ServerConnectionHandler();
        [[nodiscard]] bool Running() const;

        void Start();
    protected:
        virtual bool Initialize() = 0;
        virtual void Listen() = 0;
        virtual bool SendUserMessage(ServerUDPMessage) = 0;

        void OnIncomingMessage(ServerUDPMessage);
    public:
        void OnIncomingConnection(ServerUDPMessage);
    };
}
