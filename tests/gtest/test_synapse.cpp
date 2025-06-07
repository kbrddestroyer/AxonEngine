#include <gtest/gtest.h>
#include <networking/AxonLibrary.hpp>
#include "FakeNetworkController/FakeNetworkController.hpp"
#include <atomic>

bool hasVisited = false;

void onMessageReceived(const Networking::SynapseMessageReceivedEvent& event) {
    ASSERT_STREQ(static_cast<char*>(event.getMessage().getMessage()), "Hello World!");
    hasVisited = true;
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
