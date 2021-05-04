//
// Created by jonny on 4/22/21.
//

#ifndef TEAMPOG_CS3505_SERVER_CONTROLLER_H
#define TEAMPOG_CS3505_SERVER_CONTROLLER_H


//class spreadsheet;
#include <mutex>
#include <string>
#include <vector>
#include <unordered_map>
#include "../spreadsheet/spreadsheet.h"
#include "socket_state.h"
#include <fstream>

class spreadsheet;

namespace ss
{
    class server_controller {
    private:
        friend class spreadsheet;

        /// map of spreadsheet names (keys) to spreadsheet objects (values)
        static std::unordered_map<std::string, spreadsheet> current_spreadsheets;
        static std::mutex spreadsheet_mutex;

        static std::string remove_extra_characters(std::string s);

        static std::vector<std::string> get_existing_spreadsheets();

        static void check_client_connection(network_util::socket_state &state);

    public:
        server_controller();

        void start_server();

        static std::string unicode_to_utf8(const std::wstring &wstr);

        static void receive_name(network_util::socket_state &state);

        static void receive_spreadsheet_selection(network_util::socket_state &state);

        static void receive_cell_selection(network_util::socket_state &state);

        static std::unordered_map<std::string, spreadsheet> get_spreadsheets();

        static std::vector<std::string> process_data(network_util::socket_state &state);
    };
}


#endif //TEAMPOG_CS3505_SERVER_CONTROLLER_H
