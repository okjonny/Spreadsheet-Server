//
// Created by jonny on 4/22/21.
//
#ifndef TEAMPOG_CS3505_SERVER_CONTROLLER_H
#define TEAMPOG_CS3505_SERVER_CONTROLLER_H

#include "socket_state.h"
#include <vector>

namespace spreadsheet_server
{
    class server_controller {

    public:
        server_controller();

        void start_server();

        //static void first_contact(network_util::socket_state &socket);

        static void receive_name(network_util::socket_state &state);

        static void receive_selection(network_util::socket_state &state);

        static void send_selection(network_util::socket_state &state);

        static std::vector<std::string> process_data(network_util::socket_state &state);
    };
}


#endif //TEAMPOG_CS3505_SERVER_CONTROLLER_H
