#pragma once
#include <cstdint>
#include <vector>
#include <map>
#include <WinSock2.h>

namespace Axon {
    namespace Connection
    {
        enum class ServerUDPDefaultTags
        {
            CONTROL = 0
        };

        struct ServerUDPMessage
        {
            // Package struct:
            // CLIENT_IP DATA_TAG [DATA] sizeof(DATA)

            IN_ADDR                 inet_addr;
            ServerUDPDefaultTags    tag;
            void*                   buffer;
            uint32_t                size;
        };

        struct ServerConnection
        {
            // Should contain useful information about user

            IN_ADDR ip_addr;
        };

        class ServerConnectionHandler {
        private:
            std::map<uint32_t, ServerConnection> mConnections;
        protected:
#pragma region SERVER_CONFIGURATION
            uint16_t port;

            bool isRunning;
#pragma endregion
        public:
            explicit ServerConnectionHandler(uint16_t = 7777);

            virtual bool Initialize() = 0;
            [[nodiscard]] bool Running() const;

            void Start();
        protected:
            virtual void Listen() = 0;
            virtual void SendMessage(ServerUDPMessage) = 0;

            void OnIncomingMessage(ServerUDPMessage);
        public:
            void OnIncomingConnection(ServerUDPMessage);
        };
    }
}