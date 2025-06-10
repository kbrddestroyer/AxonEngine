#include "AxonNetwork.hpp"

void Networking::AxonNetwork::registerObject(Networking::AxonNetworkObject *object) {
    objectRegistry.push_back(std::unique_ptr<AxonNetworkObject>(object));
}

Networking::AxonNetwork & Networking::AxonNetwork::instance() {
    static AxonNetwork g_instance;

    return g_instance;
}
