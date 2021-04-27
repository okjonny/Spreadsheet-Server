#include "server_controller.h"
#include "networking.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <regex>
#include <sys/socket.h>
#include <unordered_map>
#include <string>
//#include "utf8.h"

using namespace network_util;
namespace spreadsheet_server
{

    std::unordered_map<std::string, spreadsheet> server_controller::current_spreadsheets;

    // CLIENT STRUCTS
    struct select_cell {
        std::string requestType;
        std::string cellName;

        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(select_cell, requestType, cellName)
    };

    // SERVER TO CLIENT STRUCTS
    struct cell_selected {
        std::string messageType;
        std::string cellName;
        std::string selector;
        std::string selectorName;

        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(cell_selected, messageType, cellName, selector, selectorName)
    };

/// <summary>
/// Start accepting Tcp sockets connections from clients
/// </summary>
    server_controller::server_controller()
    = default;

    void server_controller::start_server()
    {

        // This begins an "event loop"
        std::function<void(socket_state &)> callback = receive_name;
        networking::start_server(callback);
        std::cout << "Server is running. Accepting new Players:" << std::endl;
    }


    /**
     *
     * @param state - The socket_state representing the new client
     */
    void server_controller::receive_name(network_util::socket_state &state)
    {
//        if (state.ErrorOccured)
//            return;

// {"requestType":"selectCell", "cellName":"A1"}\n
        state.username = state.get_data();
        std::cout << "User Connected: " << state.get_username() << std::endl;

        //Send current spreadsheet separated by /n or \n\n if no spreadsheets are available
        std::string list;
        int i = 0;


        for (auto const &element : current_spreadsheets)
        {
            if (i == current_spreadsheets.size() - 1)
                list.append(element.first + "\n\n");
            else list.append(element.first + "\n");
            i++;
        }

        if (list.empty())
            list = "\n\n";

        send(state.get_socket(), list.c_str(), strlen(list.c_str()), 0);

        std::function<void(socket_state &)> callback = receive_spreadsheet_selection;
        state.on_network_action = callback;
    }

    /**
     *
     * @param state
     */
    void server_controller::receive_spreadsheet_selection(network_util::socket_state &state)
    {
        std::unordered_map<std::string, std::string> cells; //= {{"A1","1+3"}};

//        if (state.ErrorOccured)
//            return;

        std::string selection = state.get_data();
        state.spreadsheet = selection;

        std::cout << "Spreadsheet selected: " << selection << std::endl;

        //check if spreadsheet exists, create otherwise
        if (current_spreadsheets.find(selection) != current_spreadsheets.end())
        {
            std::cout << "map contains spreadsheet!\n";
            current_spreadsheets[selection].add_user_to_spreadsheet(state.get_socket());
        }
            // fix spreadsheet() we need to give it a name
        else
        {
            spreadsheet new_spreadsheet;
            new_spreadsheet.add_user_to_spreadsheet(state.get_socket());
//            if (selection.find('\n'))
//                return;
//            else
            current_spreadsheets.insert({selection, new_spreadsheet});


            std::cout << "New spreadsheet made: " << selection << std::endl;
        }

        //---------------CANNOT PROCESS EDIT REQUESTS HERE, MAKE SURE TO FIND SOLUTION, LOCK??------------
        //if spreadsheetlist.contains(selection)
        //  Spreadsheet s = spreadsheetList[selection];
        //{messageType:"cellUpdated", cellName: “<cell name>”,contents: “<contents>”}

        for (auto &c : cells)
        {
            std::string cell_update =
                    std::string("{\"messageType\":\"cellUpdated\",cellName:") + "\"" + c.first + "\"" +
                    ",\"contents\": \"" +
                    c.second + "\"}\n";
            send(state.get_socket(), cell_update.c_str(), strlen(cell_update.c_str()), 0);
        }

        std::function<void(socket_state &)> callback = receive_cell_selection;
        state.on_network_action = callback;
    }


    void server_controller::receive_cell_selection(network_util::socket_state &state)
    {
//        if (state.ErrorOccured)
//            return;
        std::cout << "IN RECEIVE+++++++ " << std::endl;

        std::vector<std::string> commands = process_data(state);
        current_spreadsheets[state.spreadsheet].add_command(state.get_data());

        nlohmann::json tester = nlohmann::json::parse(commands[0]);

        std::cout << "REQUEST TYPE: " << tester["requestType"] << std::endl;

        if (tester["requestType"] == "editCell")
        {
            std::string cell_update =
                    std::string("{\"messageType\":\"cellUpdated\",cellName:") + "\"" + std::string(tester["cellName"]) +
                    "\"" + ",\"contents\": \"" +
                    std::string(tester["contents"]) + "\"}\n";
            send(state.get_socket(), cell_update.c_str(), strlen(cell_update.c_str()), 0);
        } else if (tester["requestType"] == "selectCell")
        {
            nlohmann::json j;
            select_cell c;
            c.to_json(j, c);
            std::cout << "JSOOOOOON: " << j << std::endl;

        } else if (tester["requestType"] == "revertCell")
        {
            std::cout << "revert revert skrtttt" << std::endl;
        } else if (tester["requestType"] == "undo")
        {
            std::cout << "undo bro" << std::endl;
        }


        // {"requestType": "selectCell", "cellName": “A1”, "contents":“42”}\n
        // User receives their own
        std::string new_id = std::to_string(state.get_id()) + " made edit\n";
        for (long s : get_spreadsheets()[state.spreadsheet].get_users_connected())
        {
            for (std::string &m : get_spreadsheets()[state.spreadsheet].get_commands_received())
            {
                if (s != state.get_id())
                    send(s, m.c_str(), strlen(m.c_str()), 0);
            }
        }
    }


    std::vector<std::string> server_controller::process_data(socket_state &state)
    {
        std::string s = state.get_data();
//        std::stringstream s(boomer);
        std::cout << "SERVER CONTROLLER: " << s << std::endl;

        std::vector<std::string> parts;

        /*    // SPLIT:
            std::regex rgx(R"(\\n)");
            std::sregex_token_iterator iter(total_data.begin(), total_data.end(), rgx, -1);
            std::sregex_token_iterator end;
            while (iter != end)
            {
                parts.push_back(*iter);
                ++iter;
            }

            // Loop until we have processed all messages.
            // We may have received more than one.
            std::vector<std::string> new_messages;
            for (std::string p:parts)
            {
                if (p.length() == 0)
                    continue;
                // The regex splitter will include the last string even if it doesn't end with a '\n',
                // So we need to ignore it if this happens.
                if (p[p.length() - 1] != '\n')
                    break;

                // build a list of messages to send to the view
                new_messages.push_back(p);
                // Then remove it from the SocketState's growable buffer
                state.remove_data(0, p.length());
            }*/

        std::string delimiter = "\\n";

        size_t pos = 0;
        std::string token;
        while ((pos = s.find(delimiter)) != std::string::npos)
        {
            //token = s.substr(0, pos);
            //std::cout << token << std::endl;
            parts.push_back(s.substr(0, pos));
            s.erase(0, pos + delimiter.length());
        }

        return parts;
    }

    std::unordered_map<std::string, spreadsheet> server_controller::get_spreadsheets()
    { return current_spreadsheets; }

}