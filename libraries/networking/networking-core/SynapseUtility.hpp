#pragma once
#include <string>

#include <backends/backend.hpp>
#include <AxonUtility.h>

namespace Networking {
    /**
     *  Utility enum, that allows user to specify connection protocol in more human-readable manner.
     */
    enum ConnectionMode
    {
        UDP = SOCK_DGRAM,
        TCP = SOCK_STREAM
    };

    /**
     *  Utility enum that specifies Synapse behaviour
     */
    enum class SynapseMode {
        SERVER, ///< Server mode. Can handle multiple connections
        CLIENT  ///< Client mode. Works with singular server connection only
    };

    /**
    * Low-level connection info for convenient storage
    * TODO:
    * - Research replacement
    */
    struct ConnectionInfo
    {
        std::string	hostname;
        uint32_t	port = 10432;
    };

    /**
     *  Utility error class, that makes Synapse exception handling easier.
     *  <br>
     *  Usually, error code will be the same, as C backend function return value.
     */
    class AXON_DECLSPEC AxonNetworkingInternalError final : public std::exception
    {
        const uint8_t err;
    public:
        explicit AxonNetworkingInternalError(const uint8_t err = 0) : err(err) {}
        GETTER  constexpr uint8_t code() const { return err; }
        GETTER const char* what() const noexcept override;
    };
}
