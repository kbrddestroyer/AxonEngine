#include "AxonEngine.h"


Axon::AxonError::AxonError(Axon::Error::AxonErrorCode err) : reason(err) {}

Axon::Error::AxonErrorCode Axon::AxonError::err() { return reason; }
