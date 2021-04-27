//
// Created by jonny on 4/22/21.
//

#ifndef TEAMPOG_CS3505_SERVER_CONTROLLER_H
#define TEAMPOG_CS3505_SERVER_CONTROLLER_H


class spreadsheet;

#include <string>
#include <vector>
#include <unordered_map>
#include "../spreadsheet/spreadsheet.h"
#include "socket_state.h"

namespace spreadsheet_server
{
    class server_controller {
        static std::unordered_map<std::string, spreadsheet> current_spreadsheets;

    public:
        server_controller();

        void start_server();

        //static void first_contact(network_util::socket_state &socket);

        static void receive_name(network_util::socket_state &state);

        static void receive_spreadsheet_selection(network_util::socket_state &state);

        static void receive_cell_selection(network_util::socket_state &state);

        static std::unordered_map<std::string, spreadsheet> get_spreadsheets();

        //static void receive_edit_request(network_util::socket_state &state);


        static std::vector<std::string> process_data(network_util::socket_state &state);
    };
}


#endif //TEAMPOG_CS3505_SERVER_CONTROLLER_H
