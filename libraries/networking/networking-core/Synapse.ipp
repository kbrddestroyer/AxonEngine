#pragma once


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

