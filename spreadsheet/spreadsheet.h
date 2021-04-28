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
#include "dependency_graph.h"

namespace ss
{
    class spreadsheet {
        friend class spreadsheet_server::server_controller;
        std::string name;
        std::vector<long> users_connected;
        std::vector<std::string> commands_received;

        std::unordered_map<std::string, std::vector<std::string>> nonempty_cells;


    public:

        spreadsheet();

        void add_user_to_spreadsheet(long s);

        std::vector<long> get_users_connected();

        std::unordered_map<std::string, std::vector<std::string>> get_cell_list();

        void add_cell(std::string name, cell c);

        std::vector<std::string> get_commands_received();

        void add_command(std::string);

        // SHOULD THESE BE PRIVATE HMMM
        std::string get_cell_contents(std::string cell);

        static bool is_valid_name(std::string name);

        std::vector<std::string> get_nonempty_cells();

        std::vector<std::string> set_contents_of_cell(std::string name, std::string contents);

        std::vector<std::string> set_cell_contents(std::string name, std::string contents);


    private:
        bool changed;

        dependency_graph dependencies;


    };
}

#endif //SPREADSHEET_H
