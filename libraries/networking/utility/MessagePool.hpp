#pragma once
#include <networking/message/AxonMessage.hpp>
#include <stack>

namespace Networking {
    template <typename metadata>
    class MessagePoolBase {
    public:
        MessagePoolBase() = default;

        size_t getPoolSize() const { return messagePool.size(); }
        void push(const AxonMessage&);
        AxonMessage pop();
    private:
        std::stack<AxonMessage> messagePool;
    };
}
