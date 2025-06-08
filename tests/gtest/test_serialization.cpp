#include <gtest/gtest.h>
#include <serialization/serialization.hpp>
#include "messages/AxonMessage.hpp"

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

TEST(TEST_SERIALIZATION, TEST_SERIALIZATION_ZERO_DATA) {
    size64_t total;
    char* serialized = serialize(nullptr, 0, 0, &total);

    ASSERT_EQ(total, 0);
    ASSERT_FALSE(serialized);

    serialized = serialize(nullptr, 0, 2, &total);

    ASSERT_EQ(total, 2);
    ASSERT_TRUE(serialized);

    char* deserialized;
    size64_t size;
    TAG_T tag;

    deserialize(serialized, total, reinterpret_cast<void **>(&deserialized), &size, &tag);
    ASSERT_EQ(size, 0);
    ASSERT_EQ(tag, 2);

    delete[] serialized;
}

TEST(TEST_SERIALIZATION, TEST_SERIALIZATION_API_ZERO_DATA) {
    Networking::AxonMessage message(nullptr, 0);
    message.addFlag(Networking::ACKNOWLEDGE);
    Networking::AxonMessage cpyMessage(message.getSerialized());

    ASSERT_EQ(cpyMessage.getFlagSet(), Networking::ACKNOWLEDGE);
    ASSERT_EQ(cpyMessage.getPartID(), 0);

    Networking::AxonMessage message_(nullptr, 0);
    message.addFlag(Networking::ACKNOWLEDGE);
    Networking::AxonMessage cpyMessage_(message.getSerialized());

    ASSERT_EQ(cpyMessage_.getFlagSet(), Networking::ACKNOWLEDGE);
    ASSERT_EQ(cpyMessage_.getPartID(), 0);
}

TEST(TEST_SERIALIZATION, TEST_MESSAGE) {
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
    Networking::AxonMessage message_( const_cast<char*> ( message ), strlen(message) + 1, 0);

    ASSERT_STREQ(static_cast<char*>(message_.getMessage()), "Hello World!");
    Networking::AxonMessage::UniqueAxonMessagePtr ptr = message_.split(5);

    ASSERT_TRUE(ptr.get());
    ASSERT_EQ(ptr->getSize(), 8);
    ASSERT_EQ(message_.getSize(), 5);

    ASSERT_STREQ(static_cast<char*>(message_.getMessage()), "Hello World!");

    ASSERT_TRUE(message_.hasFlag(Networking::PARTIAL));
    ASSERT_FALSE(ptr->hasFlag(Networking::PARTIAL));

    ASSERT_EQ(message_.ID(), ptr->ID());
    ASSERT_EQ(message_.getPartID() + 1, ptr->getPartID());

    message_.~AxonMessage();
    ASSERT_STREQ(static_cast<char*>(ptr->getMessage()), " World!");
}
