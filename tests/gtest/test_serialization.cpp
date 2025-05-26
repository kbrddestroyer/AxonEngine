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

    TAG_T extracted;
    size64_t size;

    ASSERT_EQ(extractMetadata(
            serialized, total, &size, &extracted
            ), 0);

    ASSERT_EQ(extracted, tag);
    ASSERT_EQ(size, strlen(message));

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
    ASSERT_EQ(deserialized.getSize(), message_.getSize());
}

TEST(TEST_SERIALIZATION, TEST_MESSAGE_TAG) {
    // Case from client-server example
    const char* message = "Hello World!";
    Networking::AxonMessage message_( const_cast<char*> ( message ) , strlen(message) + 1, 0);

    Networking::SerializedAxonMessage serialized = message_.getSerialized();

    serialized.setPartID(1);
    serialized.setFlags(Networking::TAG_FLAGS::ACKNOWLEDGE);

    ASSERT_TRUE(serialized.hasFlag(Networking::TAG_FLAGS::ACKNOWLEDGE));
    Networking::SerializedAxonMessage cpyMessage(serialized.getBits(), serialized.getSize());
    ASSERT_TRUE(cpyMessage.hasFlag(Networking::TAG_FLAGS::ACKNOWLEDGE));
}

TEST(TEST_SERIALIZATION, TEST_MESSAGE_SPLIT) {
    char buffer[SYNAPSE_MESSAGE_SIZE_MAX + 5] = {};

    for (uint16_t i = 0; i < SYNAPSE_MESSAGE_SIZE_MAX + 5; i++) {
        buffer[i] = static_cast<char>(i % 256);
    }

    Networking::SerializedAxonMessage serialized( const_cast<char*> ( buffer ) , SYNAPSE_MESSAGE_SIZE_MAX + 5);
    std::unique_ptr<Networking::SerializedAxonMessage> serializedPtr = serialized.split();

    ASSERT_EQ(serialized.getSize(), SYNAPSE_MESSAGE_SIZE_MAX);
    ASSERT_EQ(serializedPtr->getSize(), 5);

    ASSERT_EQ(serialized.getPartID(), 0);
    ASSERT_EQ(serializedPtr->getPartID(), 1);

    ASSERT_EQ(serialized.getUniqueID(), serializedPtr->getUniqueID());

    ASSERT_TRUE(serializedPtr->getOwning());
    ASSERT_FALSE(serialized.getOwning());

    ASSERT_TRUE(serialized.hasFlag(Networking::PARTIAL));
    ASSERT_FALSE(serializedPtr->hasFlag(Networking::PARTIAL));
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
