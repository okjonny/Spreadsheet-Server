#include "server_controller.h"
#include "networking.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <regex>
#include <sys/socket.h>
#include <unordered_map>
#include <string>
#include "../spreadsheet/cell.h"
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


    struct undo_cell {
        std::string requestType;

        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(undo_cell, requestType)
    };

    struct redo_cell {
        std::string requestType;
        std::string cellName;

        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(redo_cell, requestType, cellName)
    };

    // SERVER TO CLIENT STRUCTS
    struct selected_cell {
        std::string messageType;
        std::string cellName;
        int selector;
        std::string selectorName;

        selected_cell(std::string cell, int _selector, std::string name)
        {
            messageType = "selected";
            cellName = cell;
            selector = _selector;
            selectorName = name;
        }

        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(selected_cell, messageType, cellName, selector, selectorName)

    };

    struct cell_updated {
        std::string messageType;
        std::string cellName;
        std::string contents;

        cell_updated(std::string name, std::string cell_contents)
        {
            messageType = "cellUpdated";
            cellName = name;
            contents = cell_contents;
        }

        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(cell_updated, messageType, cellName, contents)
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
        std::string user = state.get_data();
        std::regex newlines_re("\r\n+|\r|\n");
        auto result = std::regex_replace(user, newlines_re, "");
        state.username = result;

        std::cout << "User Connected: " << state.get_username() << std::endl;

        //Send current spreadsheet separated by /n or \n\n if no spreadsheets are available
        std::string spreadsheets_list;
        int i = 0;
        for (auto const &element : current_spreadsheets)
        {
            if (i == current_spreadsheets.size() - 1)
                spreadsheets_list.append(element.first + "\n\n");
            else spreadsheets_list.append(element.first + "\n");
            i++;
        }

        if (spreadsheets_list.empty())
            spreadsheets_list = "\n\n";

        send(state.get_socket(), spreadsheets_list.c_str(), strlen(spreadsheets_list.c_str()), 0);

        std::function<void(socket_state &)> callback = receive_spreadsheet_selection;
        state.on_network_action = callback;
    }

    /**
     *
     * @param state
     */
    void server_controller::receive_spreadsheet_selection(network_util::socket_state &state)
    {
        std::unordered_map<std::string, std::string> cells;

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

        std::vector<std::string> commands = process_data(state);
        nlohmann::json data = nlohmann::json::parse(commands[0]);

        nlohmann::json j;
        spreadsheet s = current_spreadsheets[state.spreadsheet];

        std::cout << "REQUEST TYPE: " << data["requestType"] << std::endl;
        std::string cell_name = data["cellName"];

        if (data["requestType"] == "editCell")
        {
            // CHECK FOR FORMULA STUFF HERE:
//            cell_updated c(data["cellName"], data["contents"]);
            cell c(data["cellName"], data["contents"]);
            c.to_json(j, c);

            //check if cell exists, create otherwise
            if (s.get_cell_list().find(data["cellName"]) != s.get_cell_list().end())
            {
                std::cout << "spreadsheet contains cell!\n";
                std::cout << "UPDATED CELL" << std::endl;
                s.get_cell_list().at(data["cellName"]).push_back(data["contents"]);
            }
                // fix spreadsheet() we need to give it a name
            else
            {
                std::vector<std::string> new_list;
                new_list.push_back(data["contents"]);
//                s.get_cell_list().insert({data["cellName"], new_list});
                std::cout << "ADDED CELL" << std::endl;
                current_spreadsheets[state.spreadsheet].cells.insert({data["cellName"], new_list});
//                std::cout << "SIZE: " << s.get_cell_list().at(data["cellName"]).size() << std::endl;
            }
            current_spreadsheets[state.spreadsheet].add_command(to_string(j) + "\n");
        } else if (data["requestType"] == "selectCell")
        {
            // Selects a cell from the user and sends the information to all users in the current spreadsheet.
            //{"requestType":"selectCell","cellName":"A1"}\n

            selected_cell c(data["cellName"], state.get_id(), state.get_username());
            c.to_json(j, c);
            current_spreadsheets[state.spreadsheet].add_command((to_string(j) + "\n"));
        } else if (data["requestType"] == "revertCell")
        {
            std::cout << "revert revert skrtttt" << std::endl;
        } else if (data["requestType"] == "undo")
        {
            //undo
        }

        // BROADCAST CHANGES TO SPREADSHEETS
        std::string new_id = std::to_string(state.get_id()) + " made edit\n";
        for (long s : get_spreadsheets()[state.spreadsheet].get_users_connected())
        {
            for (std::string &m : get_spreadsheets()[state.spreadsheet].get_commands_received())
            {
                if (s != state.get_socket())
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