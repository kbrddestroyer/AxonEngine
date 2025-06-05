#pragma once
#include <networking/synapse/netcontroller/AxonNetworkController.hpp>
#include <networking/synapse/BasicSynapse.hpp>
#include <queue>
#include <map>
#include <unordered_map>
#include <string>

namespace TestUtils {
    class FakeNetwork {
    public:
        FakeNetwork() = default;

        static FakeNetwork & Instance () {
            static FakeNetwork gInstance;
            return gInstance;
        }

        uint32_t create(const std::string &, uint32_t);
        uint32_t connect(const std::string &, uint32_t);

        void sendto(const Networking::SerializedAxonMessage&, uint32_t, uint32_t);
        bool recv(uint32_t socket, Networking::SerializedAxonMessage &, uint32_t &);
    private:
        static uint32_t getDesc() {
            static uint32_t desc = 0;
            return ++desc;
        }
    public:
        struct MessageNode {
            Networking::SerializedAxonMessage msg;
            uint32_t from;
        };
    private:
        std::map<uint32_t, std::queue<MessageNode>> messagePool = {};
        std::map<std::string, std::unordered_map<uint32_t, uint32_t>> nodes = {};
    };

    class FakeNetworkController : public Networking::AxonNetworkControllerBase {
    public:
        FakeNetworkController() = delete;

        explicit FakeNetworkController(Networking::SynapseInterface * owner, uint32_t);
        explicit FakeNetworkController(Networking::SynapseInterface * owner, const Networking::ConnectionInfo&);

        void start() override;
        void listen() override;
        void send(Networking::AxonMessage&) override;
        void sendTo(Networking::AxonMessage&, const Networking::NetworkNodeInfo&) override;
    protected:
        void sendSerializedTo(const Networking::SerializedAxonMessage&, const Networking::NetworkNodeInfo&) override;
    protected:
        static uint32_t getID() {
            static uint32_t gID = 0;
            return ++gID;
        }

        uint32_t connectedNode;
        uint32_t self;
        FakeNetwork &instance;
    };
}
