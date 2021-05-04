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
#include <csignal>
#include <codecvt>
#include <locale>
#include "json_struct.cpp"
#include "writer.h"

using namespace network_util;
namespace ss
{

//    class Reader {
//    public:
//        Reader(std::shared_ptr<spreadsheet_file> sf) : _sf(sf)
//        {}
//
//        std::vector<std::string> read_from_file()
//        {
//            _sf->read();
//        }
//
//    private:
//        std::shared_ptr<spreadsheet_file> _sf;
//    };

    // Static Class Variables
    std::unordered_map<std::string, ss::spreadsheet> server_controller::current_spreadsheets;
    std::mutex server_controller::spreadsheet_mutex;


    server_controller::server_controller()
    = default;


    void server_controller::start_server()
    {
        // This begins an "event loop"
        std::function<void(socket_state &)> callback = receive_name;
        networking::start_server(callback);
        std::cout << "Server is running. Accepting new Players:" << std::endl;
    }


    void server_controller::receive_name(network_util::socket_state &state)
    {
        check_client_connection(state);

        state.username = remove_extra_characters(state.get_data());

        std::cout << "User Connected: " << state.get_username() << std::endl;

        //Send current spreadsheet separated by "\n"
        // add "\n\n" if no spreadsheets available OR on last spreadsheet
        std::vector<std::string> spreadsheets = get_existing_spreadsheets();
        std::string spreadsheets_list;

        int i = 0;
        for (auto const &element : spreadsheets)
        {
            if (i == spreadsheets.size() - 1)
                spreadsheets_list.append(element + "\n\n");
            else spreadsheets_list.append(element + "\n");
            i++;
        }

        if (spreadsheets_list.empty())
            spreadsheets_list = "\n\n";

        // send the list of spreadsheets to the client
        if (send(state.get_socket(), spreadsheets_list.c_str(), strlen(spreadsheets_list.c_str()), 0) == -1)
            std::cout << "client disconnected (1) :(" << std::endl;


        std::function<void(socket_state &)> callback = receive_spreadsheet_selection;
        state.on_network_action = callback;
    }

    /**
     *
     * @param state
     */
    void server_controller::receive_spreadsheet_selection(network_util::socket_state &state)
    {

        check_client_connection(state);

        state.spreadsheet = remove_extra_characters(state.get_data());

        std::cout << "Spreadsheet selected: " << state.spreadsheet << std::endl;

        //check if spreadsheet exists, otherwise create a new one
        if (current_spreadsheets.find(state.spreadsheet) != current_spreadsheets.end())
        {
            std::cout << "contains spreadsheet!\n";
            std::lock_guard<std::mutex> lock(spreadsheet_mutex);
//            spreadsheet_mutex.lock();
            current_spreadsheets[state.spreadsheet].add_user_to_spreadsheet(state.get_socket());
//            spreadsheet_mutex.unlock();
        } else
        {
            std::lock_guard<std::mutex> lock(spreadsheet_mutex);
            spreadsheet new_spreadsheet;
//            spreadsheet_mutex.lock();
            new_spreadsheet.add_user_to_spreadsheet(state.get_socket());
            current_spreadsheets.insert({state.spreadsheet, new_spreadsheet});
//            spreadsheet_mutex.unlock();
            std::cout << "New spreadsheet made: " << state.spreadsheet << std::endl;
        }

        // Read and Send existing spreadsheet data
        std::string file_path = "../files/" + state.spreadsheet;
        std::ifstream file(file_path.c_str());
        std::vector<std::string> contents;

        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                contents.push_back(line.c_str()); // should it be c_str here??? -lauren
            }
            file.close();
        }

        // append data from file to send to client
        std::string contents_of_spreadsheet;
        for (std::string s : contents)
            contents_of_spreadsheet += s;

        contents_of_spreadsheet += std::to_string(state.get_socket()) + "\n";
        // send the message to client as one long message
        if (send(state.get_socket(), contents_of_spreadsheet.c_str(), strlen(contents_of_spreadsheet.c_str()), 0) == -1)
            std::cout << "client disconnected (2) :(" << std::endl; // What do we really want here?

        std::function<void(socket_state &)> callback = receive_cell_selection;
        state.on_network_action = callback;
    }


    void server_controller::receive_cell_selection(network_util::socket_state &state)
    {
        check_client_connection(state);

        bool is_garbage_data = false;
        bool is_error_message = false;
//        try
//        {
        // !BUG: Sometimes data = json::parse throws RARELY(?) some reason
        // Not fully received message?
        std::vector<std::string> commands = process_data(state);
        std::string dank_weed = commands[0];
        nlohmann::json data = nlohmann::json::parse(dank_weed);
        nlohmann::json struct_to_json;
        std::string command_to_send_client;

//            if (data["requestType"] == "editCell")
//            {
//                cell_updated c(data["cellName"], data["contents"]); // cell updated class
//                c.to_json(struct_to_json, c);
//                try
//                {
//                    current_spreadsheets[state.spreadsheet].set_contents_of_cell(data["cellName"], data["contents"]);
//                    writer1.write_to_file(to_string(struct_to_json));
//                }
//                catch (const std::runtime_error &e)
//                {
//                    invalid_request c(data["cellName"], e.what());
//                    c.to_json(struct_to_json, c);
//                    is_error_message = true;
//                }
//            }
        if (data["requestType"] == "selectCell")
        {
            selected_cell c(data["cellName"], state.get_id(), state.get_username());
            c.to_json(struct_to_json, c);
        } else
        {
            try
            { struct_to_json = updating_content(state, data); }
            catch (std::runtime_error &e)
            {
                is_error_message = true;
            }
        }
//            else if (data["requestType"] == "revertCell")
//            {
//                try
//                {
//                    current_spreadsheets[state.spreadsheet].revert_cell_contents(data["cellName"]);
//                    cell_updated c(data["cellName"],
//                                   current_spreadsheets[state.spreadsheet].get_cell_contents(data["cellName"]));
//                    c.to_json(struct_to_json, c);
//                    writer1.write_to_file(to_string(struct_to_json));
//                }
//                catch (std::runtime_error &e)
//                {
//                    invalid_request c(data["cellName"], e.what());
//                    c.to_json(struct_to_json, c);
//                    is_error_message = true;
//                }
//            }
//            else if (data["requestType"] == "undo")
//            {
//                std::pair<std::string, std::string> undo_contents = {"", ""};
//                try
//                {
//                    undo_contents = current_spreadsheets[state.spreadsheet].get_undo_contents();
//                    current_spreadsheets[state.spreadsheet].undo();
//                    cell_updated c(undo_contents.first, current_spreadsheets[state.spreadsheet].get_cell_contents(
//                            undo_contents.first)); // cell updated class
//                    c.to_json(struct_to_json, c);
//                    writer1.write_to_file(to_string(struct_to_json));
//                }
//                catch (std::runtime_error &e)
//                {
//                    invalid_request c(undo_contents.first, e.what());
//                    c.to_json(struct_to_json, c);
//                    is_error_message = true;
//                }
//            }
//
//            else // messages that aren't valid json
//            {
//                is_garbage_data = true;
////                state.clear_data();
//            }

        command_to_send_client = to_string(struct_to_json) + "\n";

        std::cout << "Server to Client Command: " << command_to_send_client << std::endl;


        // TODO: BROADCAST TO EVERYONE EXCEPT THE CLIENT THAT JUST DISCONNECTED, STATE.GET_ID????
        std::string new_id = std::to_string(state.get_id()) + " made edit\n";

        for (long s : get_spreadsheets()[state.spreadsheet].get_users_connected())
        {
            // Only Send error occurred messages to the client who sent it
            if (is_error_message && s == state.get_socket())
            {
                send(s, command_to_send_client.c_str(), strlen(command_to_send_client.c_str()), 0);
                break;
            } else if (!is_error_message && !is_garbage_data)
                if (send(s, command_to_send_client.c_str(), strlen(command_to_send_client.c_str()), 0) == -1)
                {
                    sigset_t set;
                    sigfillset(&set);
                    sigaddset(&set, SIGPIPE);
                    int retcode = sigprocmask(SIG_BLOCK, &set, NULL);
                    if (retcode == -1)
                        std::cout << "ignored signal??" << std::endl;
                    std::cout << "client disconnected (3) :(" << std::endl;
                    throw std::runtime_error("This boomer disconnect like bitconnnneeeect.");
                }
        }
        if (struct_to_json["messageType"] == "cellUpdated")
        {
            //////////////////////
        }
//        }
//        catch (...)
//        {
//            throw std::runtime_error("THROWING FROM SELECT CELL SELECTION");
//        }
    }


    //
    // PRIVATE HELPER METHODS
    //

    nlohmann::json server_controller::updating_content(socket_state &state, nlohmann::json data)
    {
        auto synchronized_file = std::make_shared<spreadsheet_file>(state.spreadsheet);
        writer writer1(synchronized_file);
        std::pair<std::string, std::string> undo_contents = {"", ""};
        nlohmann::json struct_to_json;
        try
        {
            if (data["requestType"] == "undo")
            {
                undo_contents = current_spreadsheets[state.spreadsheet].get_undo_contents();
                current_spreadsheets[state.spreadsheet].undo();
                cell_updated c(undo_contents.first, current_spreadsheets[state.spreadsheet].get_cell_contents(
                        undo_contents.first)); // cell updated class
                c.to_json(struct_to_json, c);
            } else if (data["requestType"] == "revertCell")
            {
                current_spreadsheets[state.spreadsheet].revert_cell_contents(data["cellName"]);
                cell_updated c(data["cellName"],
                               current_spreadsheets[state.spreadsheet].get_cell_contents(data["cellName"]));
                c.to_json(struct_to_json, c);
            } else if (data["requestType" == "editCell"])
            {
                current_spreadsheets[state.spreadsheet].set_contents_of_cell(data["cellName"], data["contents"]);
                cell_updated c(data["cellName"], data["contents"]); // cell updated class
                c.to_json(struct_to_json, c);
            }
            writer1.write_to_file(to_string(struct_to_json));
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
            parts.push_back(s.substr(0, pos));
            s.erase(0, pos + delimiter.length());
        }

        return parts;
    }

    std::string server_controller::unicode_to_utf8(const std::wstring &wstr)
    {
        std::string ret;
        try
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t> > wcv;
            ret = wcv.to_bytes(wstr);
        } catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
        return ret;
    }

    std::unordered_map<std::string, spreadsheet> server_controller::get_spreadsheets()
    { return current_spreadsheets; }


    std::string server_controller::remove_extra_characters(std::string s)
    {
        std::regex newlines_re("\r\n+|\r|\n");
        std::string result = std::regex_replace(s, newlines_re, "");
        std::cout << result << std::endl;
        return result;
    }

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

    void server_controller::check_client_connection(socket_state &state)
    {
        if (state.get_error_occured())
        {
            for (long s : get_spreadsheets()[state.spreadsheet].get_users_connected())
            {
                nlohmann::json j;
                std::string to_send;
                disconnected d(state.get_id());
                d.to_json(j, d);
                to_send = to_string(j) + R"(\n)";

                if (s != state.get_socket())
                    send(s, std::string(to_send).c_str(), strlen(to_send.c_str()), 0);
            }

            return;
        }
    }

}