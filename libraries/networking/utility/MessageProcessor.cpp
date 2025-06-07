#include "MessageProcessor.hpp"

namespace Networking {
    MessageProcessor::MessageProcessor(SynapseInterface *owner) : owner(owner) {
    }

    void MessageProcessor::process(const AxonMessage &message, const Socket &from) {
        if (message.hasFlag(VALIDATE))
        {
            AxonMessage reply = AxonMessage(message, 0);
            owner->sendTo(reply, from);
        }
        if (message.hasFlag(ACKNOWLEDGE) && !message.hasFlag(PARTIAL))
        {
            if (const auto it = std::find(
                pendingValidation.begin(),
                pendingValidation.end(), message.ID()); it != pendingValidation.end())
                pendingValidation.erase(it);
        }
        if (mmap->contains(message.ID()) || message.hasFlag(PARTIAL))
        {
            mmap->append(message);

            if (!message.hasFlag(PARTIAL))
            {
                const std::shared_ptr<AxonMessage> res = mmap->collapse(message.ID());
                if (!res)
                    return;

                SynapseMessageReceivedEvent event_(*res, from);
                this->owner->getEventManager().invoke(&event_);
            }
            return;
        }
    }
} // Networking