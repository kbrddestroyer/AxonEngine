#include <gtest/gtest.h>

TEST(BasicChecks, BasicCheck)
{
    EXPECT_EQ(2, 2);
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
