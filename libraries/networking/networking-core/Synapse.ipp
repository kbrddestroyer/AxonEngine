#pragma once

template <Networking::ConnectionMode mode>
void Networking::Synapse<mode>::initializeFromConnectionMode() {
    if (isServer)
    {
        uint8_t returnCode = initialize_server<mode>(socket, info.port);
    }
}

template <Networking::ConnectionMode mode>
void Networking::AsyncSynapse<mode>::kill()
{
    isAlive = false;
    proc.join();
}

template <Networking::ConnectionMode mode>
Networking::Synapse<mode>::Synapse(uint32_t port)
{
    isServer = true;

    info.port = port;
    initializeFromConnectionMode();
}


template <Networking::ConnectionMode mode>
Networking::Synapse<mode>::Synapse(const ConnectionInfo& connection)
{
    isServer = false;
    info.hostname = connection.hostname;
    info.port = connection.port;

    initializeFromConnectionMode();
}


template <Networking::ConnectionMode mode>
Networking::Synapse<mode>::~Synapse()
{
    isAlive = false;
    finalize_udp(socket.socket);
}

template <Networking::ConnectionMode mode>
Networking::AsyncSynapse<mode>::~AsyncSynapse()
{
    kill();
}

