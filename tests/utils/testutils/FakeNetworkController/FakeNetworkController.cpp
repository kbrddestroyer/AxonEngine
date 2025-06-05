#include "FakeNetworkController.hpp"
#include <networking/synapse/netcontroller/AxonNetworkController.hpp>

void TestUtils::FakeNetwork::sendto(const Networking::SerializedAxonMessage &serialized, uint32_t socket, uint32_t from) {
    messagePool[socket].push({serialized, from});
}

bool TestUtils::FakeNetwork::recv(uint32_t socket, Networking::SerializedAxonMessage &buffer, uint32_t &from) {
    if (messagePool[socket].empty())
        return false;
    MessageNode node = messagePool[socket].front();

    buffer = node.msg;
    from = node.from;

    messagePool[socket].pop();
    return true;
}

uint32_t TestUtils::FakeNetwork::create(
        const std::string &hostname,
        uint32_t port
        ) {
    uint32_t desc = getDesc();
    this->nodes[hostname][port] = desc;
    return desc;
}

uint32_t TestUtils::FakeNetwork::connect(const std::string &hostname, uint32_t port) {
    return this->nodes[hostname][port];
}

void TestUtils::FakeNetworkController::start() {
    listen();
}

void TestUtils::FakeNetworkController::listen() {
    while (this->isAlive()) {
        Networking::SerializedAxonMessage message(nullptr, 0);
        uint32_t from = 0;
        if (instance.recv(connectedNode, message, from)) {
            owningSynapse->processIncomingMessage(
                    message, {(int32_t) from, {}}
                    );
        }
    }
}

void TestUtils::FakeNetworkController::send(Networking::AxonMessage &message) {
    this->sendTo(message, {(int32_t) connectedNode, {}});
}

void TestUtils::FakeNetworkController::sendTo(Networking::AxonMessage &message, const Networking::NetworkNodeInfo &node) {
    this->sendSerializedTo(message.getSerialized(), node);
}

void TestUtils::FakeNetworkController::sendSerializedTo(const Networking::SerializedAxonMessage &serialized,
                                                        const Networking::NetworkNodeInfo &node) {
    instance.sendto(serialized, node.socket, self);
}

TestUtils::FakeNetworkController::FakeNetworkController(Networking::SynapseInterface *owner, uint32_t port) :
    Networking::AxonNetworkControllerBase(owner),
    instance(FakeNetwork::Instance())
    {
        connectedNode = self = instance.create("test-nodes-fake-host", port);
    }

TestUtils::FakeNetworkController::FakeNetworkController(Networking::SynapseInterface *owner,
                                                        const Networking::ConnectionInfo &info) :
        Networking::AxonNetworkControllerBase(owner),
        instance(FakeNetwork::Instance()) {
    self = instance.create("test-nodes-fake-clients", info.port);
    connectedNode = instance.connect(info.hostname, info.port);
}
