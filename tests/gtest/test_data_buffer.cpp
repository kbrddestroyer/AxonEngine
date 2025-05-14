#include <gtest/gtest.h>
#include <networking/AxonNetwork.hpp>
#include <cstring>

TEST(TEST_BINARY_STREAM, TEST_COMMON)
{
    Networking::AxonBinaryStreamBase stream;
    const char* appendedData = "Hello World!";
    stream.append(appendedData, strlen(appendedData));

    ASSERT_EQ(stream.size(), strlen(appendedData));

    Networking::AxonBinaryStreamBase newStream = std::move(stream);
    ASSERT_EQ(stream.size(), 0);
    ASSERT_EQ(newStream.size(), strlen(appendedData));

    auto* newlyCreatedStream = new Networking::AxonBinaryStreamBase(newStream);
    ASSERT_EQ(newlyCreatedStream->size(), newStream.size());
    delete newlyCreatedStream;
    ASSERT_NE(newStream.size(), 0);
    ASSERT_NE(newStream.data(), nullptr);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}