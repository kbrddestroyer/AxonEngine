#include <MemoryGuard/MemoryGuard.hpp>
#include <networking/AxonLibrary.hpp>
#include <chrono>
#include <sstream>

int main() {
    Networking::AsyncSynapse<
            Networking::BerkeleyAxonNetworkController<Networking::UDP, Networking::SynapseMode::SERVER>
            > server(10432);
    Networking::AsyncSynapse<
            Networking::BerkeleyAxonNetworkController<Networking::UDP, Networking::SynapseMode::CLIENT>
            > client("localhost", 10432);
    server.start();
    client.start();

    ENABLE_MEMGUARD

    for (uint8_t i = 0; i < 4; i++)
    {
        DISABLE_MEMGUARD
        std::this_thread::sleep_for(std::chrono::milliseconds (100));
        std::stringstream sstream;
        sstream <<
                "Sending message on " << i;
        Networking::AxonMessage message_(const_cast<char*>(sstream.str().c_str()), sstream.str().length() + 1, 0, Networking::TAG_FLAGS::VALIDATE);
        ENABLE_MEMGUARD
        client.send(message_);
    }

    client.kill();
    server.kill();
}

MEMGUARD_REPORT
