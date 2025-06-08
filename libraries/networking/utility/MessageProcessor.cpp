#include "MessageProcessor.hpp"
#include <cassert>

#include <algorithm>

namespace Networking {
    MessageProcessor::MessageProcessor(SynapseInterface *owner) : owner(owner) {
    }

    void MessageProcessor::process(const AxonMessage &message, const Socket &from) {
        if (message.hasFlag(VALIDATE))
        {
            AxonMessage reply(message, 0);
            owner->sendTo(reply, from);
        }
        if (message.hasFlag(NETOBJ_INI)) {
            RequestUniqueIDProto proto = * static_cast<RequestUniqueIDProto *>( message.getMessage() );

            assert (!message.hasFlag(VALIDATE));
            assert (message.getSize() == sizeof(RequestUniqueIDProto));

            proto.serverSideID = getObjectID();

            AxonMessage reply(&proto, sizeof(proto), 0, NETOBJ_REPL);

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

        SynapseMessageReceivedEvent event_(message, from);
        this->owner->getEventManager().invoke(&event_);
    }
} // Networking