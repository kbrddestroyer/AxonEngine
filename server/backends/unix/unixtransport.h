#pragma once

#if defined(__unix__) || __APPLE__ || __APPLE_CC__

#include <AxonEngine.h>
#include <netinet/in.h>

namespace Axon {
    namespace Backends {
        namespace Unix {
            class UnixUDPConnectionHandler {

            };
        }
    }
}
#endif
