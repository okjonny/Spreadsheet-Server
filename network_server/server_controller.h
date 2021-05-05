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
#include "utf8.h"

class spreadsheet;

namespace ss
{
    class server_controller {
    private:
        friend class spreadsheet;

        /// map of spreadsheet names (keys) to spreadsheet objects (values)
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

        static std::string unicode_to_utf8(const std::wstring &wstr);

        static void receive_name(network_util::socket_state &state);

        static void receive_spreadsheet_selection(network_util::socket_state &state);

        static void receive_cell_selection(network_util::socket_state &state);

        static std::unordered_map<std::string, spreadsheet> get_spreadsheets();

//        inline void decode_utf8(const std::string& bytes, std::wstring& wstr);
//
//        inline void encode_utf8(const std::wstring& wstr, std::string& bytes);
    };
}


#endif //TEAMPOG_CS3505_SERVER_CONTROLLER_H
