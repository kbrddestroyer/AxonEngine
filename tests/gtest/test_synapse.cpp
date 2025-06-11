#include <gtest/gtest.h>
#include <networking/AxonLibrary.hpp>
#include "FakeNetworkController/FakeNetworkController.hpp"
#include <atomic>

bool hasVisited = false;

namespace {
    void onMessageReceived(const Networking::SynapseMessageReceivedEvent& event) {
        ASSERT_STREQ(static_cast<char*>(event.getMessage().getMessage()), "Hello World!");
        hasVisited = true;
    }

    void onMessageReceivedWithUniqueID(const Networking::SynapseMessageReceivedEvent& event) {
        static uint64_t lastTaken = 0;

        if (!event.getMessage().hasFlag(Networking::NETOBJ_REPL)) return;

        ASSERT_EQ(event.getMessage().getSize(), sizeof(Networking::MessageProcessor::RequestUniqueIDProto));

        Networking::MessageProcessor::RequestUniqueIDProto repl = * static_cast< Networking::MessageProcessor::RequestUniqueIDProto * > ( event.getMessage().getMessage() );
        ASSERT_NE( repl.serverSideID, lastTaken );
        lastTaken = repl.serverSideID;
        hasVisited = true;
    }
}

TEST(TEST_SYNAPSE, TEST_FAKE_NETWORK) {
    Networking::Synapse<
            TestUtils::FakeNetworkController
    > server(10435);
    Networking::Synapse<
            TestUtils::FakeNetworkController
    > client("test-nodes-fake-host", 10435);

    server.getEventManager().subscribe<Networking::SynapseMessageReceivedEvent>(onMessageReceived);
    server.start();
    client.start();

    Networking::AxonMessage msg("Hello World!", strlen("Hello World!"));
    client.send(msg);
    ASSERT_TRUE(hasVisited);
    hasVisited = false;
}

TEST(TEST_SYNAPSE, TEST_GET_SERVER_ID) {
    Networking::Synapse<TestUtils::FakeNetworkController> server(10432);
    Networking::Synapse<TestUtils::FakeNetworkController> client("test-nodes-fake-host", 10432);
    client.getEventManager().subscribe<Networking::SynapseMessageReceivedEvent>(onMessageReceivedWithUniqueID);

    server.start();
    client.start();

    Networking::MessageProcessor::RequestUniqueIDProto repl = { 0, 0 };
    Networking::AxonMessage message( &repl, sizeof(repl), 0, Networking::NETOBJ_INI );

    client.send(message);
    client.send(message);
    client.send(message);

    ASSERT_TRUE(hasVisited);
}

