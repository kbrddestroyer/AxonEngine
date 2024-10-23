#pragma once
#include <cstdint>


namespace Axon {
    class ServerConnectionHandler {
    protected:
#pragma region SERVER_CONFIGURATION
        uint16_t port;

        bool isRunning;
#pragma endregion
    public:
        explicit ServerConnectionHandler(uint16_t = 7777);

        virtual bool Startup() = 0;
        virtual void Listen() = 0;

        [[nodiscard]] bool Running() const;
    };
}