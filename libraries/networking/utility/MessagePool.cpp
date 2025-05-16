#include "MessagePool.hpp"
#include <networking/networking-core/Synapse.hpp>


void Networking::MessagePoolBase::push(MessagePoolNode message) {
    poolMutex.lock();
    messagePool.push(std::make_shared<MessagePoolNode> (message));
    poolMutex.unlock();
}

Networking::MessagePoolNodePtr Networking::MessagePoolBase::pop() {
    poolMutex.lock();
    MessagePoolNodePtr message = messagePool.front();
    messagePool.pop();
    poolMutex.unlock();
    return message;
}

