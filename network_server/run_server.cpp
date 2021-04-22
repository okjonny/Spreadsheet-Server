#include "server.h"

int main(int argc, char const *argv[]) {
    network_util::server::setup_server();
    while (true) {}
}
