#pragma once

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

    uint8_t result = initialize_server<mode>(socket, port);

    if (result != SUCCESS) {
        throw AxonNetworkingInternalError(result);
    }
}


template <Networking::ConnectionMode mode>
Networking::Synapse<mode>::Synapse(const ConnectionInfo& connection)
{
    isServer = false;
    info.hostname = connection.hostname;
    info.port = connection.port;

    uint8_t result = initialize_client<mode>(socket, connection.hostname.c_str(), connection.port);

    if (result != SUCCESS) {
        throw AxonNetworkingInternalError(result);
    }
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

