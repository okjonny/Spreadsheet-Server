#include "networking.h"
#include "socket_state.h"
using namespace network_util;

namespace spreadsheet_server {
    network_util::socket_state boomer(network_util::socket_state s) {
        return s;
    }

    int main(int argc, char const *argv[]) {
        std::function<socket_state(socket_state)> boom = boomer;
        network_util::networking::start_server(boom);
        while (true) {}
    }
}