#include <gtest/gtest.h>
#include <serialization/serialization.hpp>
#include <networking/message/AxonMessage.hpp>

#include <cstring>

TEST(TEST_SERIALIZATION, TEST_SERIALIZATION_GENERAL)
{
    const char* message = "Hello World!";
    void* buffer;
    TAG_T tag = 25;
    size64_t total = 0;

    const char* serialized = const_cast<const char*>(serialize(const_cast<char *>(message), strlen(message), tag, &total));

    TAG_T extracted;
    size64_t size;

    ASSERT_EQ(extractMetadata(
            serialized, total, &size, &extracted
            ), 0);

    ASSERT_EQ(extracted, tag);
    ASSERT_EQ(size, strlen(message));

    ASSERT_TRUE(total);

    size64_t deserialized_size = 0;
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
    Networking::AxonMessage message_( const_cast<char*> ( message ) , strlen(message), 1, 1);

    Networking::SerializedAxonMessage serialized = message_.getSerialized();

    Networking::AxonMessage deserialized(serialized);
    ASSERT_EQ(deserialized.getSize(), message_.getSize());
}

TEST(TEST_SERIALIZATION, TEST_MESSAGE_TAG) {
    // Case from client-server example
    const char* message = "Hello World!";
    Networking::AxonMessage message_( const_cast<char*> ( message ) , strlen(message), 0);

    message_.setPartID(1);
    message_.setFlags(Networking::TAG_FLAGS::ACKNOWLEDGE);
    Networking::SerializedAxonMessage serialized = message_.getSerialized();

    ASSERT_TRUE(message_.hasFlag(Networking::TAG_FLAGS::ACKNOWLEDGE));
    Networking::AxonMessage cpyMessage(serialized);
    ASSERT_TRUE(cpyMessage.hasFlag(Networking::TAG_FLAGS::ACKNOWLEDGE));
}


TEST(TEST_SERIALIZATION, TEST_MESSAGE_SPLIT) {
    const char* message = "Hello World!";
    Networking::AxonMessage message_( const_cast<char*> ( message ), strlen(message), 0);

    Networking::AxonMessage::UniqueAxonMessagePtr ptr = message_.split(5);

    ASSERT_TRUE(ptr.get());
    ASSERT_EQ(ptr->getSize(), 7);
    ASSERT_EQ(message_.getSize(), 5);

    ASSERT_STREQ(static_cast<char*>(message_.getMessage()), "Hello World!");

    ASSERT_TRUE(message_.hasFlag(Networking::PARTIAL));
    ASSERT_FALSE(ptr->hasFlag(Networking::PARTIAL));

    ASSERT_EQ(message_.ID(), ptr->ID());
    ASSERT_EQ(message_.getPartID() + 1, ptr->getPartID());

    message_.~AxonMessage();
    ASSERT_STREQ(static_cast<char*>(ptr->getMessage()), " World!");
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
