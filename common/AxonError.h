#pragma once

namespace Axon {
    namespace Error
    {
        enum AxonErrorCode
        {
            INTERNAL_ERROR,
            LISTEN_SOCKET_ERROR
        };
    }

    class AxonError
    {
    protected:
        Error::AxonErrorCode reason;
    public:
        explicit AxonError(Error::AxonErrorCode = Error::AxonErrorCode::INTERNAL_ERROR);

        [[nodiscard]] Error::AxonErrorCode err() const;
    };
}
