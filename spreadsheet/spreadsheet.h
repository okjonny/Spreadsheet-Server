//
// Created by hibban on 4/26/21.
//
#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <vector>
#include <mutex>
#include "socket_state.h"

namespace spreadsheet_server
{
    class spreadsheet {
        std::string name;
        std::vector<long> users_connected;
        std::vector<std::string> commands_received;
//        std::mutex spreadsheet_lock;
    public:
        spreadsheet();

        void add_user_to_spreadsheet(long s);

        std::vector<long> get_users_connected();

        std::vector<std::string> get_commands_received();

        void add_command(std::string);
    };
}

#endif //SPREADSHEET_H
