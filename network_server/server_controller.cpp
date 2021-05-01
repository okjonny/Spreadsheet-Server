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
namespace ss
{

    std::unordered_map<std::string, ss::spreadsheet> server_controller::current_spreadsheets;

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
        std::string command_to_send;

        std::cout << "REQUEST TYPE: " << data["requestType"] << std::endl;
        std::string cell_name = data["cellName"];

        if (data["requestType"] == "editCell")
        {
            cell c(data["cellName"], data["contents"]); // cell updated class
            c.to_json(j, c);

            // Check here if cell content would cause a circular dependency
            // if it does, send error message
            // otherwise, server makes the change and sends "cellUpdated" to all users including itself
            current_spreadsheets[state.spreadsheet].set_contents_of_cell(data["cellName"], data["contents"]);
            current_spreadsheets[state.spreadsheet].add_to_history(to_string(j) + "\n");
            command_to_send = to_string(j) + "\n";
        } else if (data["requestType"] == "selectCell")
        {

            // Selects a cell from the user and sends the information to all users in the current spreadsheet.
            //{"requestType":"selectCell","cellName":"A1"}\n

            selected_cell c(data["cellName"], state.get_id(), state.get_username());
            c.to_json(j, c);
            current_spreadsheets[state.spreadsheet].add_to_history((to_string(j) + "\n"));
            command_to_send = to_string(j) + "\n";

        } else if (data["requestType"] == "revertCell")
        {
            current_spreadsheets[state.spreadsheet].revert_cell_contents(data["cellName"]);
        } else if (data["requestType"] == "undo")
        {
            current_spreadsheets[state.spreadsheet].undo();
        }

        // BROADCAST CHANGES TO SPREADSHEETS
        std::string new_id = std::to_string(state.get_id()) + " made edit\n";
        for (long s : get_spreadsheets()[state.spreadsheet].get_users_connected())
        {
            if (s != state.get_socket())
                send(s, command_to_send.c_str(), strlen(command_to_send.c_str()), 0);
        }
    }


    std::vector<std::string> server_controller::process_data(socket_state &state)
    {
        std::string s = state.get_data();
        std::cout << "SERVER CONTROLLER: " << s << std::endl;

        std::vector<std::string> parts;
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