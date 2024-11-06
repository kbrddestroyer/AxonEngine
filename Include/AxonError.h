#pragma once
#include <cstdlib>
#include <exception>

namespace Axon {
    namespace Error
    {
        enum AxonErrorCode
        {
            INTERNAL_ERROR,
            SOCKET_BIND_ERROR,
            HOSTNAME_NO_INFO,
            LISTEN_SOCKET_ERROR
        };
    }

    class AxonError : public std::exception
    {
    protected:
        Error::AxonErrorCode reason;
    public:
        explicit AxonError(Error::AxonErrorCode = Error::AxonErrorCode::INTERNAL_ERROR);

        [[nodiscard]] constexpr Error::AxonErrorCode err() const;
    };
}
