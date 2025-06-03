#include "messages/AxonMessage.hpp"
#include <MemoryGuard/MemoryGuard.hpp>

int main() {
    ENABLE_MEMGUARD

    Networking::AxonMessage msg1(nullptr, 0);
    Networking::AxonMessage msg2("Hello World", 12, 0, 1);

    Networking::SerializedAxonMessage serializedAxonMessage1 = msg1.getSerialized();
    Networking::SerializedAxonMessage serializedAxonMessage2(msg2);

    Networking::AxonMessage cpyMsg1(msg1);
    Networking::AxonMessage cpyMsg2 = msg2;

    Networking::SerializedAxonMessage serializedAxonMessageCpy1(serializedAxonMessage1);
    Networking::SerializedAxonMessage serializedAxonMessageCpy2 = serializedAxonMessage2;

    msg1.split(2);
}

MEMGUARD_REPORT
