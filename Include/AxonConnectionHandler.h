#pragma once
#include "Axon.h"
#include <AxonEngine.h>
#include <AxonTypes.h>


class AXON_EXPORT ConnectionHandler
{
private:
    ConnectionHandler() = default;
public:
    ~ConnectionHandler() = default;
protected:
    virtual bool Initialize() = 0;
    virtual void SendUDPMessage(char*, size_t) = 0;
public:
    virtual bool SendUserMessage(const Axon::Connection::UDPMessage& message) = 0;
    virtual bool Startup() = 0;
};