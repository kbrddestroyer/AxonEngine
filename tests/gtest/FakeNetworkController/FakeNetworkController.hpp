#pragma once
#include <networking/synapse/BasicSynapse.hpp>
#include <queue>
#include <map>
#include <unordered_map>
#include <string>

namespace Networking {
    class AxonNetworkControllerBase;
}

namespace TestUtils {
    class FakeNetworkController;

    class FakeNetwork {
        FakeNetwork() = default;
    public:
        static FakeNetwork & Instance () {
            static FakeNetwork gInstance;
            return gInstance;
        }

        uint32_t create(const std::string &, uint32_t);
        uint32_t connect(const std::string &, uint32_t);

        void bind(uint32_t, FakeNetworkController *);

        void sendto(const Networking::SerializedAxonMessage&, uint32_t, uint32_t);
    private:
        static uint32_t getDesc() {
            static uint32_t desc = 0;
            return ++desc;
        }
    private:
        std::map<uint32_t, FakeNetworkController *> pool = {};
        std::map<std::string, std::unordered_map<uint32_t, uint32_t>> nodes = {};
    };

    class FakeNetworkController : public Networking::AxonNetworkControllerBase {
    public:
        FakeNetworkController() = delete;

        explicit FakeNetworkController(Networking::SynapseInterface * owner, uint32_t);
        explicit FakeNetworkController(Networking::SynapseInterface * owner, const Networking::ConnectionInfo&);

        void listen() override;
        void send(Networking::AxonMessage&) override;
        void sendTo(Networking::AxonMessage&, const Networking::NetworkNodeInfo&) override;


        GETTER Networking::SynapseInterface * owner() { return owningSynapse; }
    protected:
        void sendSerializedTo(const Networking::SerializedAxonMessage&, const Networking::NetworkNodeInfo&) override;

        uint32_t connectedNode;
        uint32_t self;
        FakeNetwork &instance;
    };
}

#include "FakeNetworkController.ipp"
