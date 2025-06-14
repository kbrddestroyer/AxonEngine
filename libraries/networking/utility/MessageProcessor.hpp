#pragma once

#include <networking/synapse/BasicSynapse.hpp>
#include <messages/AxonMessage.hpp>
#include <networking/utility/MessagePool.hpp>

namespace Networking {

#define ADD_MODULE(TYPENAME, NAME, ...) \
    std::unique_ptr<TYPENAME> NAME = std::make_unique<TYPENAME>(__VA_ARGS__)

class AXON_DECLSPEC MessageProcessor {
public:
    struct RequestUniqueIDProto {
        uint32_t clientSideID = 0;
        uint64_t serverSideID = 0;

        static uint32_t generateID() {
            static uint32_t uniqueID = 0;
            return ++uniqueID;
        }
    };

    MessageProcessor() = delete;

    explicit MessageProcessor(SynapseInterface *);

    MessageProcessor & operator= (const MessageProcessor &) = delete;
    MessageProcessor & operator= (MessageProcessor&&) = delete;

    void process(const AxonMessage &, const Socket &);
    void awaitValidation(uint64_t id) { pendingValidation.push_back(id); }

    GETTER MessagePoolNodePtr processPool() const { return pool->pop(); }
    void poolMessage(const MessagePoolNode &node) const { pool->push(node); }
protected:
    GETTER static uint64_t getObjectID() {
        static uint64_t lastID = 0;
        return ++lastID;
    }
private:
    SynapseInterface *owner;

    std::vector<uint64_t> pendingValidation;
    ADD_MODULE(MessageMapBase, mmap);
	ADD_MODULE(MessagePoolBase, pool);
};

} // Networking
