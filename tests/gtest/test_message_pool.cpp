#include <gtest/gtest.h>
#include <networking/AxonNetwork.hpp>


TEST(TEST_MESSAGE_POOL, TEST_REGULAR)
{
    Networking::MessagePoolBase pool;

    const char* initData = "Hello World!";

    Networking::AxonMessage message(const_cast<char*>(initData), strlen(initData), 0, 0);
    Networking::MessagePoolNode node = { message, {} };

    pool.push(node);
    ASSERT_EQ(pool.getPoolSize(), 1);

    pool.push(node);
    ASSERT_EQ(pool.getPoolSize(), 2);

    Networking::MessagePoolNodePtr pNode1 = pool.pop();
    Networking::MessagePoolNodePtr pNode2 = pool.pop();

    ASSERT_FALSE(pool.getPoolSize());
    ASSERT_NE(pNode1.get(), pNode2.get());
}
