//
// Created by Aleksey Grudko on 24.10.24.
//

#include "client_connection.h"

namespace Axon {
    bool Client::ClientConnectionHandler::Startup() {
        return Initialize();
    }
} // Axon