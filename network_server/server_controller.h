//
// Created by jonny on 4/22/21.
//
#ifndef TEAMPOG_CS3505_SERVER_CONTROLLER_H
#define TEAMPOG_CS3505_SERVER_CONTROLLER_H
#include "socket_state.h"
namespace spreadsheet_server {
    class server_controller {
    public:
        void start_server();

        void first_contact(network_util::socket_state socket);
    };
}


#endif //TEAMPOG_CS3505_SERVER_CONTROLLER_H
