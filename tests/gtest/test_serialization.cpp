#include <gtest/gtest.h>
#include <serialization/serialization.hpp>
#include <networking/message/AxonMessage.hpp>

TEST(TEST_SERIALIZATION, TEST_SERIALIZATION_GENERAL)
{
    const char* message = "Hello World!";
    void* buffer;
    TAG_T tag = 25;
    size_t total = 0;

    const char* serialized = const_cast<const char*>(serialize(const_cast<char *>(message), strlen(message), tag, &total));

    ASSERT_TRUE(total);

    size_t deserialized_size = 0;
    uint8_t ret = deserialize((char*)serialized, total, (void**) &buffer, &deserialized_size, &tag);

    ASSERT_EQ(ret, 0);
    ASSERT_EQ(tag, 25);

    char* strbuf = static_cast<char*>(buffer);

    strbuf[deserialized_size] = 0;
    ASSERT_EQ(strcmp(strbuf, message), 0);
    ASSERT_EQ(strlen(message), deserialized_size);
}

TEST(TEST_SERIALIZATION, TEST_MESSAGE) {
    // Case from client-server example
    const char* message = "Hello World!";
    Networking::AxonMessage message_( const_cast<char*> ( message ) , strlen(message) + 1, 1, 1);

    Networking::SerializedAxonMessage serialized = message_.getSerialized();

    Networking::AxonMessage deserialized(serialized);

    ASSERT_EQ(deserialized.getFlags(), message_.getFlags());
    ASSERT_EQ(deserialized.getPartID(), message_.getPartID());
    ASSERT_EQ(deserialized.getSize(), message_.getSize());
}

TEST(TEST_SERIALIZATION, TEST_MESSAGE_TAG) {
    // Case from client-server example
    const char* message = "Hello World!";
    Networking::AxonMessage message_( const_cast<char*> ( message ) , strlen(message) + 1, 0);

    message_.setPartID(1);
    message_.setFlags(Networking::TAG_FLAGS::FINISH | Networking::TAG_FLAGS::ACKNOWLEDGE);

    Networking::SerializedAxonMessage serialized = message_.getSerialized();
    Networking::AxonMessage cpyMessage(serialized.getBits(), serialized.getSize());

    ASSERT_TRUE(~cpyMessage.getFlags() ^ (Networking::TAG_FLAGS::ACKNOWLEDGE | Networking::TAG_FLAGS::FINISH));
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
