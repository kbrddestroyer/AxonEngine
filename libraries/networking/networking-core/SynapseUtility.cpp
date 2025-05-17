#include "SynapseUtility.hpp"
#include <common_macro.h>

namespace Networking {
    const char * AxonNetworkingInternalError::what() const {
        switch (err) {
            case SUCCESS:
                return "The code really has thrown SUCCESS exception. Check usage.";
            case ERR_WININIT_FAIL:
#if defined(WINDOWS_PLATFORM)
                return "C WSAStartup function failed.";
#else
                return "That shouln't be here. Check your code";
#endif
            case ERR_INVALID:
                return "Got invalid socket value";
            case ERR_GETADDRINFO_FAIL:
                return "getaddrinfo function failed";
            case ERR_COULD_NOT_BIND:
                return "Could not bind socket. The address is already in use.";
            case ERR_CONNECTION_ABORTED:
                return "Connection aborted";
            default:
                return "Unknown exception occurred";
        }
    }
}
