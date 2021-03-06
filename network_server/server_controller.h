#ifndef TEAMPOG_CS3505_SERVER_CONTROLLER_H
#define TEAMPOG_CS3505_SERVER_CONTROLLER_H

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
    /**
     * Used to create our server_controller methods
     */
    class server_controller {
    private:
        friend class spreadsheet;

        static std::unordered_map<std::string, spreadsheet> current_spreadsheets;

        static std::mutex spreadsheet_mutex;

        /// helper method that removes extra characters from the client's input
        static std::string remove_extra_characters(std::string s);

        static std::vector<std::string> get_existing_spreadsheets();

        static bool is_disconnected(network_util::socket_state &state);

        static nlohmann::json updating_content(network_util::socket_state &s, nlohmann::json selection);

        static std::string process_data(network_util::socket_state &state);

    public:

        server_controller();

        ~server_controller();

        void start_server();

        void stop_server();

        static std::string unicode_to_utf8(const std::wstring &wstr);

        static void receive_name(network_util::socket_state &state);

        static void receive_spreadsheet_selection(network_util::socket_state &state);

        static void receive_cell_selection(network_util::socket_state &state);

        static std::unordered_map<std::string, spreadsheet> get_spreadsheets();

    };
}


#endif //TEAMPOG_CS3505_SERVER_CONTROLLER_H
