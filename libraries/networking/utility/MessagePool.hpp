#pragma once
#include <networking/message/AxonMessage.hpp>
#include <queue>
#include <map>
#include <memory>
#include <mutex>


namespace Networking {
    struct AXON_DECLSPEC MessagePoolNode
    {
        AxonMessage     message;
        SOCKADDR_IN_T   destination;
    };

    typedef std::shared_ptr<MessagePoolNode> MessagePoolNodePtr;

    class AXON_DECLSPEC MessagePoolBase {
    public:
        MessagePoolBase() = default;

        GETTER size_t getPoolSize() const { return messagePool.size(); }

        void push(const MessagePoolNode&);
        MessagePoolNodePtr pop();
    private:
        std::queue<MessagePoolNodePtr> messagePool;
        std::mutex poolMutex;
    };

    class AXON_DECLSPEC MessageMapBase {
    public:
        MessageMapBase() = default;

        GETTER size_t getPoolSize() const { return messagePool.size(); }

        std::shared_ptr<AxonMessage> push(const AxonMessage& message) {
            auto it = messagePool.find(message.ID());
            if (it != messagePool.end())
            {
                it->second->append(message);
                return it->second;
            }

            messagePool[message.ID()] = std::make_shared<AxonMessage>(message);
            return messagePool[message.ID()];
        }

        bool contains(uint16_t id) {
            return messagePool.find(id) != messagePool.end();
        }

    private:
        std::map<uint16_t, std::shared_ptr<AxonMessage>> messagePool;
    };
}
