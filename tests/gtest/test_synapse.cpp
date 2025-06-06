#include <gtest/gtest.h>
#include <networking/AxonNetwork.hpp>
#include "FakeNetworkController/FakeNetworkController.hpp"
#include <atomic>
#include <mutex>

bool hasVisited = false;

void onMessageReceived(const Networking::SynapseMessageReceivedEvent& event) {
    static std::mutex f_mutex;
    f_mutex.lock();
    ASSERT_STREQ(static_cast<char*>(event.getMessage().getMessage()), "Hello World!");
    hasVisited = true;
    f_mutex.unlock();
}

TEST(TEST_SYNAPSE, TEST_SYNAPSE_COMMON_UDP) {
    Networking::AsyncSynapse<
            Networking::BerkeleyAxonNetworkController<Networking::UDP, Networking::SynapseMode::SERVER>
            > server(10435);
    Networking::AsyncSynapse<
            Networking::BerkeleyAxonNetworkController<Networking::UDP, Networking::SynapseMode::CLIENT>
    > client({"localhost", 10435});

    server.getEventManager().subscribe<Networking::SynapseMessageReceivedEvent>(onMessageReceived);
    server.start();
    client.start();

    Networking::AxonMessage msg("Hello World!", sizeof("Hello World!"));
    client.send(msg);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    ASSERT_TRUE(hasVisited);

    hasVisited = false;
}

TEST(TEST_SYNAPSE, TEST_SYNAPSE_COMMON_TCP) {
    Networking::AsyncSynapse<
            Networking::BerkeleyAxonNetworkController<Networking::TCP, Networking::SynapseMode::SERVER>
    > server(10435);
    Networking::AsyncSynapse<
            Networking::BerkeleyAxonNetworkController<Networking::TCP, Networking::SynapseMode::CLIENT>
    > client({"localhost", 10435});

    server.getEventManager().subscribe<Networking::SynapseMessageReceivedEvent>(onMessageReceived);
    server.start();
    client.start();
    Networking::AxonMessage msg("Hello World!", sizeof("Hello World!"), 2, 0);

    for (uint8_t i = 0; i < 3; i++)
        client.send(msg);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    ASSERT_TRUE(hasVisited);
    hasVisited = false;
 }

TEST(TEST_SYNAPSE, TEST_FAKE_NETWORK) {
    Networking::Synapse<
            TestUtils::FakeNetworkController
    > server(10435);
    Networking::Synapse<
            TestUtils::FakeNetworkController
    > client({"test-nodes-fake-host", 10435});

    server.getEventManager().subscribe<Networking::SynapseMessageReceivedEvent>(onMessageReceived);
    server.start();
    client.start();

    Networking::AxonMessage msg("Hello World!", sizeof("Hello World!"));
    client.send(msg);
    ASSERT_TRUE(hasVisited);
    hasVisited = false;
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
