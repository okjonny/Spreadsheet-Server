// Hibban Butt, Lauren Schwenke, Jonny Palacios-Torres
#include "server_controller.h"
#include "networking.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <regex>
#include <sys/socket.h>
#include <unordered_map>
#include <string>
#include <memory>
#include <dirent.h>
#include "json_struct.cpp"
#include "writer.h"

using namespace network_util;

// Server Controller giving functionality for executing server commands
namespace ss
{
    // Static Class Variables
    std::unordered_map<std::string, ss::spreadsheet> server_controller::current_spreadsheets;
    std::mutex server_controller::spreadsheet_mutex;

    /**
     * Default constructor
     */
    server_controller::server_controller()
    = default;

    /**
     * Destructor
     */
    server_controller::~server_controller()
    = default;


    /**
     * Begins the server and allows clients to connect
     */
    void server_controller::start_server()
    {
        // This begins an "event loop"
        std::cout << "Server is running. Accepting new clients:" << std::endl;
        std::function<void(socket_state &)> callback = receive_name;
        networking::start_server(callback);
    }

    /**
     * Stops the server and creates a disconnect message
     */
    void server_controller::stop_server()
    {
        nlohmann::json j;
        server_shutdown shutdown("Server is shutting down.");
        shutdown.to_json(j, shutdown);
        networking::stop_server(to_string(j) + "\n");
    }

    /**
    * First callback that receives the name of a client
    * @param state
    */
    void server_controller::receive_name(network_util::socket_state &state)
    {

        if (is_disconnected(state))
            return;

        state.username = remove_extra_characters(state.get_data());

        std::cout << "JOINED: " << "---- " << state.get_username() << std::endl;

        //Send current spreadsheet separated by "\n"
        // add "\n\n" if no spreadsheets available OR on last spreadsheet
        std::vector<std::string> spreadsheets = get_existing_spreadsheets();
        std::string spreadsheets_list;

        spreadsheet_mutex.lock();

        int i = 0;
        for (auto const &element : spreadsheets)
        {
            if (i == spreadsheets.size() - 1)
                spreadsheets_list.append(element + "\n\n");
            else
                spreadsheets_list.append(element + "\n");
            i++;
        }

        if (spreadsheets_list.empty())
            spreadsheets_list = "\n\n";

        std::cout << "SENDING LIST: " << spreadsheets_list << std::endl;

        // send the list of spreadsheets to the client
        send(state.get_socket(), spreadsheets_list.c_str(), strlen(spreadsheets_list.c_str()), 0);
        spreadsheet_mutex.unlock();

        std::function<void(socket_state &)> callback = receive_spreadsheet_selection;
        state.on_network_action = callback;
    }

    /**
     * Callback for selecting a spreadsheet for a client
     * @param state
     */
    void server_controller::receive_spreadsheet_selection(network_util::socket_state &state)
    {

        if(is_disconnected(state))
            return;

        state.spreadsheet = remove_extra_characters(state.get_data());

        std::cout << state.get_username() << " SENT AS CHOICE OF SPREADSHEET: " << "---- " << state.spreadsheet << std::endl;

        //check if spreadsheet exists, otherwise create a new one
        if (current_spreadsheets.find(state.spreadsheet) != current_spreadsheets.end())
        {
            std::cout << state.get_username() << " SPREADSHEET EXISTS: " << "---- " << state.spreadsheet << std::endl;
            //std::lock_guard<std::mutex> lock(spreadsheet_mutex);
            spreadsheet_mutex.lock();
            current_spreadsheets[state.spreadsheet].users_connected.push_back(&state);
            spreadsheet_mutex.unlock();
        } else
        {
            //std::lock_guard<std::mutex> lock(spreadsheet_mutex);
            spreadsheet_mutex.lock();

            spreadsheet new_spreadsheet;
            // new_spreadsheet.add_user_to_spreadsheet(state);
            new_spreadsheet.users_connected.push_back(&state);
            current_spreadsheets.insert({state.spreadsheet, new_spreadsheet});
            spreadsheet_mutex.unlock();
            std::cout << state.get_username() << " CREATED NEW SPREADSHEET: " << "---- " << state.spreadsheet << std::endl;


        }

        // Read and Send existing spreadsheet data
        std::string file_path = "../files/" + state.spreadsheet;
        std::ifstream file(file_path.c_str());
        std::vector<std::string> contents;

        if (file.is_open())
        {
            std::string line;
            nlohmann::json data;

            while (std::getline(file, line))
            {
                contents.push_back(line.c_str());
            }
            file.close();
        }

        // append data from file to send to client
        std::string contents_of_spreadsheet;
        for (std::string s : contents)
            contents_of_spreadsheet += s + "\n"; // TODO ADDED A NEWLINE HERE

        contents_of_spreadsheet += std::to_string(state.get_id()) + "\n";

        spreadsheet_mutex.lock();
        send(state.get_socket(), contents_of_spreadsheet.c_str(), strlen(contents_of_spreadsheet.c_str()), 0);
        spreadsheet_mutex.unlock();

        std::cout << "SENT: " << contents_of_spreadsheet << std::endl;

        std::function<void(socket_state &)> callback = receive_cell_selection;
        state.on_network_action = callback;
    }

    /**
    * Callback after the server has already sent a spreadsheet to the client and is awaiting commands.
    * @param state
    */
    void server_controller::receive_cell_selection(network_util::socket_state &state)
    {
        if (is_disconnected(state))
            return;

        bool is_error_message = false;
        std::string request = process_data(state);

        spreadsheet_mutex.lock();

        nlohmann::json data;

        try {
            data = nlohmann::json::parse(
                    request.c_str()); // can we just assume client is sending the correct things pls
        }
        catch (...) {
        }
        nlohmann::json struct_to_json;
        std::string command_to_send_client;

        if (data["requestType"] == "selectCell")
        {
            selected_cell c(data["cellName"], state.get_id(), state.get_username());
            c.to_json(struct_to_json, c);
            std::cout << "SELECTED CELL ----------------" << std::endl;
        } else
        {
            try
            {
                struct_to_json = updating_content(state, data);
                std::cout << "CONSTRUCTED JSON TO SEND ----------------" << std::endl;
            }
            catch (std::runtime_error &e)
            {
                is_error_message = true;
            }
        }
        command_to_send_client = to_string(struct_to_json) + "\n";

        // TODO: BROADCAST TO EVERYONE EXCEPT THE CLIENT THAT JUST DISCONNECTED, STATE.GET_ID????
        std::string new_id = std::to_string(state.get_id()) + " made edit\n";

        if (struct_to_json["messageType"] == "cellUpdated")
        {
            for (network_util::socket_state *s : current_spreadsheets[state.spreadsheet].users_connected)
            {
                if (!s->get_error_occured())
                {
                    send(s->get_socket(), command_to_send_client.c_str(),
                         strlen(command_to_send_client.c_str()), 0);

                    std::cout << "SENT: " << command_to_send_client << std::endl;
                }
            }
        } else if (struct_to_json["messageType"] == "cellSelected")
        {
            for (network_util::socket_state *s : current_spreadsheets[state.spreadsheet].users_connected)
            {
                if (!s->get_error_occured() && s -> get_socket() != state.get_socket()){
                    send(s->get_socket(), command_to_send_client.c_str(), strlen(command_to_send_client.c_str()), 0);

                    std::cout << "SENT: " << command_to_send_client << std::endl;
                }

            }
        } else if (struct_to_json["messageType"] == "requestError")
        {
            send(state.get_socket(), command_to_send_client.c_str(), strlen(command_to_send_client.c_str()), 0);
            std::cout << "SENT: " << command_to_send_client << std::endl;
        }


        spreadsheet_mutex.unlock();
    }

    //
    // PRIVATE HELPER METHODS
    //

    /**
     * Updating the content of cells depending on data selection
     * @param state - the socket_state to update from
     * @param data - the json to update cell from
     * @return - json that needs to be sent
     */
    nlohmann::json server_controller::updating_content(socket_state &state, nlohmann::json data)
    {
        auto synchronized_file = std::make_shared<spreadsheet_file>(state.spreadsheet);
        writer writer1(synchronized_file);
        std::pair<std::string, std::string> undo_contents = {"", ""};
        nlohmann::json struct_to_json;
        std::string update;

        try
        {
            if (data["requestType"] == "undo")
            {
                undo_contents = current_spreadsheets[state.spreadsheet].get_undo_contents();
                current_spreadsheets[state.spreadsheet].undo();
                cell_updated c(undo_contents.first, current_spreadsheets[state.spreadsheet].get_cell_contents(
                        undo_contents.first)); // cell updated class TODO this is returning null bc "there aren't any undos" killl
                c.to_json(struct_to_json, c);
                update = to_string(struct_to_json);
            } else if (data["requestType"] == "revertCell")
            {
                current_spreadsheets[state.spreadsheet].revert_cell_contents(data["cellName"]);
                cell_updated c(data["cellName"],
                               current_spreadsheets[state.spreadsheet].get_cell_contents(data["cellName"]));
                c.to_json(struct_to_json, c);
                update = to_string(struct_to_json);
            } else if (data["requestType"] == "editCell")
            {
                current_spreadsheets[state.spreadsheet].set_contents_of_cell(data["cellName"], data["contents"]);
                cell_updated c(data["cellName"], data["contents"]); // cell updated class
                c.to_json(struct_to_json, c);
                update = to_string(struct_to_json);
            } else
            {
                update = std::string();
            }

            if(!update.empty())
                writer1.write_to_file(update);
        }
        catch (std::runtime_error &e)
        {
            if (data["requestType"] == "undo")
            {
                invalid_request c(undo_contents.first, e.what());
                c.to_json(struct_to_json, c);
            } else
            {
                invalid_request c(data["cellName"], e.what());
                c.to_json(struct_to_json, c);
            }
        }
        return struct_to_json;
    }

    /**
     * Erases the data from the state
     * @param state - the socket state to update from
     * @return - string to udpate
     */
    std::string server_controller::process_data(socket_state &state)
    {
        std::string s = state.get_data();

        s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
        return s;
    }

    /**
     * Removes the extra characters from an entered string
     * @param s - the string that contains the \r\n
     * @return string - without the enter or escape characters
     */
    std::string server_controller::remove_extra_characters(std::string s)
    {
        std::regex newlines_re("\r\n+|\r|\n");
        std::string result = std::regex_replace(s, newlines_re, "");
        std::cout << result << std::endl;
        return result;
    }

    /**
     * Reads the folder with all files to display to user
     * @return
     */
    std::vector<std::string> server_controller::get_existing_spreadsheets()
    {
        // !TODO: LIST SPREADHSHEET NAMES IN ALPHABETICAL ORDER
        DIR *dir;
        struct dirent *diread;
        std::vector<std::string> spreadsheet_list;

        if ((dir = opendir("../files")) != nullptr)
        {
            while ((diread = readdir(dir)) != nullptr)
            {
                // Ignore misc. files
                if (!strcmp(diread->d_name, ".") || !strcmp(diread->d_name, ".."))
                {
                } else
                {
                    spreadsheet_list.push_back(std::string(diread->d_name));
                }
            }
            closedir(dir);
        } else
        {
            std::cout << "MM PROBABLY THROW AN ERROR HERE" << std::endl;
        }
        return spreadsheet_list;
    }

    /**
     * Informs clients that a user on the same spreadsheet has disconnected.
     * @param state
     */
    bool server_controller::is_disconnected(socket_state &state)
    {
        if (state.get_error_occured())
        {
            spreadsheet_mutex.lock();
            for (int i = 0; i < current_spreadsheets[state.spreadsheet].get_users_connected().size(); i++)
            {
                nlohmann::json j;
                std::string to_send;
                disconnected d(state.get_id());
                d.to_json(j, d);
                to_send = to_string(j) + "\n";

                if (current_spreadsheets[state.spreadsheet].users_connected[i]->get_socket() != state.get_socket() &&
                    !current_spreadsheets[state.spreadsheet].users_connected[i]->get_error_occured())
                    send(current_spreadsheets[state.spreadsheet].users_connected[i]->get_socket(),
                         std::string(to_send).c_str(), strlen(to_send.c_str()), 0);
            }

            spreadsheet_mutex.unlock();

            return true;
        }
        return false;
    }
}

