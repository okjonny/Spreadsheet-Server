#include "server.h"

int main(int argc, char const *argv[]) {
    network_util::server main_server;
    main_server.setup_server();
    while (true) {}
}
