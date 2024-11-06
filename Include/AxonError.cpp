#include "AxonError.h"


Axon::AxonError::AxonError(Axon::Error::AxonErrorCode err) : reason(err) {}

constexpr Axon::Error::AxonErrorCode Axon::AxonError::err() const { return reason; }
