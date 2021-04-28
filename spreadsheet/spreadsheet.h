//
// Created by hibban on 4/26/21.
//
#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <vector>
#include <mutex>
#include "socket_state.h"
#include "cell.h"
#include <unordered_map>
#include "server_controller.h"

namespace spreadsheet_server
{
    class spreadsheet {
//        friend class spreadsheet_server::server_controller;
        std::string name;
        std::vector<long> users_connected;
        std::vector<std::string> commands_received;

    public:
        std::unordered_map<std::string, std::vector<std::string>> cells;

        spreadsheet();

        void add_user_to_spreadsheet(long s);

        std::vector<long> get_users_connected();

        std::unordered_map<std::string, std::vector<std::string>> get_cell_list();

        void add_cell(std::string name, cell c);

        std::vector<std::string> get_commands_received();

        void add_command(std::string);
    };
}

#endif //SPREADSHEET_H
