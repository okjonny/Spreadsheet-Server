//
// Created by hibban on 4/25/21.
//

#include "spreadsheet.h"
#include <string>

namespace spreadsheet_server
{
    struct cell {
        std::string contents;
    };

    spreadsheet::spreadsheet()
    {
    }

    void spreadsheet::add_user_to_spreadsheet(long s)
    {
        users_connected.push_back(s);
    }

    std::vector<long> spreadsheet::get_users_connected()
    {
        return users_connected;
    }


    std::vector<std::string> spreadsheet::get_commands_received()
    {
        return commands_received;
    }

    void spreadsheet::add_command(std::string s)
    {
        commands_received.push_back(s);
    }
//{}(network_util::socket_state s);
//string GetCellContents(string name)
//{
//    name = Normalize(name);
//    NameCheck(name);
//
//    if(NonemptyCells.TryGetValue(name, out Cell c))
//    return c.Contents;
//    return "";
//}
}