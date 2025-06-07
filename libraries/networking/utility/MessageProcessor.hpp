#pragma once

#include <networking/synapse/BasicSynapse.hpp>
#include <messages/AxonMessage.hpp>
#include <networking/utility/MessagePool.hpp>

namespace Networking {

#define ADD_MODULE(TYPENAME, NAME, ...) \
    std::unique_ptr<TYPENAME> NAME = std::make_unique<TYPENAME>(__VA_ARGS__)

class AXON_DECLSPEC MessageProcessor {
public:
    MessageProcessor() = delete;

    MessageProcessor(SynapseInterface *);

    MessageProcessor & operator= (const MessageProcessor &) = delete;
    MessageProcessor & operator= (MessageProcessor&&) = delete;

    void process(const AxonMessage &, const Socket &);
    void awaitValidation(uint64_t id) { pendingValidation.push_back(id); }

    MessagePoolNodePtr processPool() const { return pool->pop(); }
    void poolMessage(const MessagePoolNode &node) const { pool->push(node); }
private:
    SynapseInterface *owner;

    std::vector<uint64_t> pendingValidation;
    ADD_MODULE(MessageMapBase, mmap);
	ADD_MODULE(MessagePoolBase, pool);
};

} // Networking
