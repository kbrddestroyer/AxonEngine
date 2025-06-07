#pragma once

inline void TestUtils::FakeNetwork::sendto(const Networking::SerializedAxonMessage &serialized, uint32_t socket, uint32_t from) {
    pool[socket]->owner()->processIncomingMessage(serialized, { static_cast<SOCKET_T>( socket ), {} });
}

inline uint32_t TestUtils::FakeNetwork::create(
        const std::string &hostname,
        uint32_t port
        ) {
    uint32_t desc = getDesc();
    this->nodes[hostname][port] = desc;
    return desc;
}

inline uint32_t TestUtils::FakeNetwork::connect(const std::string &hostname, uint32_t port) {
    return this->nodes[hostname][port];
}

void TestUtils::FakeNetwork::bind(uint32_t desc, TestUtils::FakeNetworkController *controller) {
    pool[desc] = controller;
}

inline void TestUtils::FakeNetworkController::listen() {}

inline void TestUtils::FakeNetworkController::send(Networking::AxonMessage &message) {
    this->sendTo(message, {static_cast<SOCKET_T>(connectedNode), {}});
}

inline void TestUtils::FakeNetworkController::sendTo(Networking::AxonMessage &message, const Networking::NetworkNodeInfo &node) {
    this->sendSerializedTo(message.getSerialized(), node);
}

inline void TestUtils::FakeNetworkController::sendSerializedTo(const Networking::SerializedAxonMessage &serialized,
                                                        const Networking::NetworkNodeInfo &node) {
    instance.sendto(serialized, node.socket, self);
}

inline TestUtils::FakeNetworkController::FakeNetworkController(Networking::SynapseInterface *owner, uint32_t port) :
    AxonNetworkControllerBase(owner),
    instance(FakeNetwork::Instance())
    {
        connectedNode = self = instance.create("test-nodes-fake-host", port);
        instance.bind(connectedNode, this);
    }

inline TestUtils::FakeNetworkController::FakeNetworkController(Networking::SynapseInterface *owner,
                                                        const Networking::ConnectionInfo &info) :
        AxonNetworkControllerBase(owner),
        instance(FakeNetwork::Instance()) {
    self = instance.create("test-nodes-fake-clients", info.port);
    connectedNode = instance.connect(info.hostname, info.port);
    instance.bind(self, this);
}
