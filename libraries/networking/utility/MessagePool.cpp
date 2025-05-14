#include "MessagePool.hpp"
#include <networking/networking-core/Synapse.hpp>


void Networking::MessagePoolBase::push(MessagePoolNode message) {
    messagePool.push(std::make_shared<MessagePoolNode> (message));
}

Networking::MessagePoolNodePtr Networking::MessagePoolBase::pop() {
    MessagePoolNodePtr message = messagePool.front();


    messagePool.pop();
    return message;
}

