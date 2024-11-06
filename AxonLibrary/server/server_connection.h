#pragma once
#include <AxonTypes.h>
#include <Axon.h>

#include <cstdint>
#include <map>

#if defined(WINDOWS_PLATFORM)
#include <WinSock2.h>
#elif defined(UNIX_PLATFORM)
#include <netinet/in.h>
#endif

namespace Axon::Connection
{
    /// <summary>
    /// Default control tags
    /// </summary>
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

    class AXON_DECLSPEC ServerConnectionHandler {
    private:
        std::map<uint32_t, ServerConnection> mConnections;
    protected:
        Axon::Connection::AXON_PORT port;

        bool isRunning;
    public:
        explicit ServerConnectionHandler(uint16_t = 7777);

        virtual ~ServerConnectionHandler();
        [[nodiscard]] bool Running() const;

        void Startup() noexcept;
    protected:
        virtual bool Initialize() = 0;
        virtual void Listen() = 0;
        virtual bool SendUserMessage(char*, size_t, uint64_t) = 0;

        void NotifyOnIncomingMessage(char*, size_t);
        void OnIncomingMessage(const ServerUDPMessage&);
    public:
        bool SendUDPMessage(const ServerUDPMessage& message);

        static ServerConnectionHandler* createServerHandler(Axon::Connection::AXON_PORT);
    };
}
