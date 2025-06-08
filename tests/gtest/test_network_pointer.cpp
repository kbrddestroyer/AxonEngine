#include <gtest/gtest.h>
#include <AxonLibrary.hpp>
#include "FakeNetworkController/FakeNetworkController.hpp"
#include "utility/AxonNetworkPointer.hpp"

TEST(TEST_NETWORK_POINTER, TEST_GET_SERVER_ID) {
    Networking::Synapse<TestUtils::FakeNetworkController> server(10432);
    Networking::Synapse<TestUtils::FakeNetworkController> client({"test-nodes-fake-host", 10432});

    server.start();
    client.start();
    uint8_t data = 0;
    Networking::AxonNetworkPtr<uint8_t> ptr(&client, &data);

    ASSERT_TRUE(ptr.ready());
}
