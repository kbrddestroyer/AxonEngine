#pragma once
#include <cstdint>


namespace Axon {
    class TransportReceiver {
    protected:
#pragma region SERVER_CONFIGURATION
        const char *addr;
        uint16_t port;

        bool isRunning;
#pragma endregion
    public:
        explicit TransportReceiver(const char * = "localhost", uint16_t = 7777);

        virtual bool Startup() = 0;
        virtual void Listen() = 0;

        [[nodiscard]] bool Running() const;
    };
}