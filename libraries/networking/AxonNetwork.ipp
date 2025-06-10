template<class Synapse>
void Networking::AxonNetwork::initialize() {
    static_assert(std::is_base_of_v<SynapseInterface, Synapse>);

    synapse = std::make_unique<Synapse>();
}

template<class Object, typename... Args>
Networking::AxonNetworkObject * Networking::AxonNetwork::createObject(Args&&... args) {
    static_assert(std::is_base_of_v<AxonNetworkObject, Object>);

    AxonNetworkObject *ob = new Object(std::forward<Args>(args)...);

    objectRegistry.push_back(std::unique_ptr<AxonNetworkObject>( ob ));
    return ob;
}