#pragma once
#include "messages/AxonMessage.hpp"
#include <backends/backend.hpp>
#include <queue>
#include <map>
//#include <memory>
#include <mutex>
//#include <functional>
//#include <vector>
#include <AxonUtility.h>


namespace Networking {
    struct AXON_DECLSPEC MessagePoolNode
    {
        AxonMessage     message;
        Socket          destination;
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
        typedef std::shared_ptr<AxonMessage> AxonMessagePtr;
        class MessageMapComparator
        {
        public:
            bool operator() ( const AxonMessagePtr& l, const AxonMessagePtr& r ) const { return l->getPartID() < r->getPartID(); }
        };
        typedef std::priority_queue<AxonMessagePtr, std::vector<AxonMessagePtr>, MessageMapComparator> MessageMapNode;
    public:
        MessageMapBase() = default;

        GETTER size_t getPoolSize() const { return messagePool.size(); }

        AxonMessagePtr append(const AxonMessage& message) {
            auto it = messagePool.find(message.ID());
            if (it != messagePool.end())
            {
                if (!it->second.empty() && message.getPartID() == it->second.top()->getPartID() + 1)
                {
                    it->second.top()->append(message);
                    return it->second.top();
                }
                it->second.push(std::make_shared<AxonMessage>(message));
            }

            MessageMapNode& node = messagePool[message.ID()] = MessageMapNode();
            AxonMessagePtr ptr = std::make_shared<AxonMessage>(message);
            node.push(ptr);
            return ptr;
        }

        AxonMessagePtr collapse(uint64_t id) {
            auto it = messagePool.find(id);
            if (it != messagePool.end() && !it->second.empty())
            {
                AxonMessagePtr ptr = it->second.top();
                it->second.pop();
                while (!it->second.empty()) {
                    ptr->append(*it->second.top());
                    it->second.pop();
                }
                return ptr;
            }
            return {};
        }

        bool contains(uint16_t id) {
            return messagePool.find(id) != messagePool.end();
        }
    private:
        std::map<uint16_t, MessageMapNode> messagePool;
    };
}
