#pragma once
#include <networking/message/AxonMessage.hpp>
#include <queue>
#include <memory>

namespace Networking {
    struct AXON_DECLSPEC MessagePoolNode
    {
        AxonMessage message;
        SOCKADDR_IN_T destination;
    };

    typedef std::shared_ptr<MessagePoolNode> MessagePoolNodePtr;

    class AXON_DECLSPEC MessagePoolBase {
    public:
        MessagePoolBase() = default;

        size_t getPoolSize() const { return messagePool.size(); }
        void push(MessagePoolNode);
        MessagePoolNodePtr pop();
    private:
        std::queue<MessagePoolNodePtr> messagePool;
    };
}
