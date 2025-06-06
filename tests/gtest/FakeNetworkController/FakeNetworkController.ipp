#pragma once

inline void TestUtils::FakeNetwork::sendto(const Networking::SerializedAxonMessage &serialized, uint32_t socket, uint32_t from) {
    g_mutex.lock();
    messagePool[socket].push({std::make_shared<Networking::SerializedAxonMessage>(serialized), from});
    g_mutex.unlock();
}

inline bool TestUtils::FakeNetwork::recv(uint32_t socket, Networking::SerializedAxonMessage &buffer, uint32_t &from) {
    g_mutex.lock();
    if (messagePool[socket].empty()) {
        g_mutex.unlock();
        return false;
    }
    MessageNode node = messagePool[socket].front();

    buffer = *node.msg;
    from = node.from;

    messagePool[socket].pop();
    g_mutex.unlock();
    return true;
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

inline void TestUtils::FakeNetworkController::listen() {
    while (this->isAlive()) {
        Networking::SerializedAxonMessage message(nullptr, 0);
        uint32_t from = 0;
        if (instance.recv(connectedNode, message, from)) {
            owningSynapse->processIncomingMessage(
                    message, {static_cast<SOCKET_T>(connectedNode), {}}
                    );
        }
    }
}

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
    }

inline TestUtils::FakeNetworkController::FakeNetworkController(Networking::SynapseInterface *owner,
                                                        const Networking::ConnectionInfo &info) :
        AxonNetworkControllerBase(owner),
        instance(FakeNetwork::Instance()) {
    self = instance.create("test-nodes-fake-clients", info.port);
    connectedNode = instance.connect(info.hostname, info.port);
}
