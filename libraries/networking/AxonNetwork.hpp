#pragma once
#include <string>
#include <vector>

#include <AxonUtility.h>
#include <networking/networking-core/Synapse.hpp>
#include <backends/backend.hpp>

namespace Networking
{
	/**
	* Networking core
	* Handles multiple synapse connections
	*/
	AXON_DECLSPEC class AxonNetwork
	{
        Synapse<ConnectionMode::UDP> synapse;
	public:
		AxonNetwork() = default;
	};
}

/* AxonNetwork.hpp */
