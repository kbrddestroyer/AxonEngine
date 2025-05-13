#include "MessagePool.hpp"


void Networking::MessagePoolBase::push(const AxonMessage& message) {
    messagePool.push(std::move(message));
}

Networking::AxonMessage Networking::MessagePoolBase::pop() {
    AxonMessage message = messagePool.top();
    messagePool.pop();

    return message;
}
