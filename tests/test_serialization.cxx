#include <AxonTypes.h>

using namespace Axon;

int main()
{
    const char* message_data = "Test message data to serialize";

    Connection::UDPMessage message((char*) message_data, strlen(message_data) * sizeof(char), 1025);

    Connection::UDPMessage deserialized;

    std::shared_ptr<char[]> serialized;
    size_t serialized_size;
    serialized = message.getSerializedData(serialized_size);
    deserialized.setDeserialized(serialized, serialized_size);

    if (
            message.tag != deserialized.tag ||
            strcmp(deserialized.data, message_data) != 0
            )
        return 1;

    return 0;
}