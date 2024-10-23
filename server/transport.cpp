#include "transport.h"


Axon::TransportReceiver::TransportReceiver(const char * addr, uint16_t port) : addr(addr), port(port)
{
    isRunning = false;
}

bool Axon::TransportReceiver::Running() const
{
    return isRunning;
}