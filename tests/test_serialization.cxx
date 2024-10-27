#include <AxonTypes.h>

using namespace Axon;

int main()
{
    const char* message_data = "Test message data to serialize";

    Connection::UDPMessage message;
    Connection::UDPMessage::createUDPMessage(message, (void*)message_data, strlen(message_data) + 1, 19);

    Connection::UDPMessage deserialized;
    size_t serialized_size;
    
    char* serialized = serialize(message.data, message.size, message.tag, &serialized_size);
    deserialize(serialized, serialized_size, &deserialized.data, &deserialized.size, &deserialized.tag);

    free(serialized);

    return strcmp(message.data, deserialized.data) != -1;
}