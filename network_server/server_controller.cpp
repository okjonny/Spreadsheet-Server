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
#include <sstream>
//#include <experimental/filesystem>

using namespace network_util;
namespace ss
{

    //
//Created by Lauren Schwenke on 5/1/21.
//


    class spreadsheet_file {
    public:
        std::ofstream file;
        std::ifstream input_file;

        spreadsheet_file(const std::string &path) : _path(path)
        {
        }

        void write(const std::string &dataToWrite)
        {
            file.open(_path, std::ios_base::app);
            // Never should happen
//            if (file.bad() || !file.is_open())

//            std::lock_guard<std::mutex> lock(_writerMutex);
            file << dataToWrite;
            file.close();
        }

        std::vector<std::string> read()
        {
            input_file.open(_path);
//            std::lock_guard<std::mutex> lock(_writerMutex);
            std::vector<std::string> contents;
            std::string line;

            if (input_file.fail())
                return contents;

            while (!file.eof())
            {
                input_file >> line;
                contents.push_back(line);
            }
            input_file.close();
            return contents;
        }


    private:
        std::string _path;
        std::mutex _writerMutex;
    };

    class Writer {
    public:
        Writer(std::shared_ptr<spreadsheet_file> sf) : _sf(sf)
        {}

        void write_to_file(const std::string &message)
        {
            _sf->write(message + "\n");
        }

    private:
        std::shared_ptr<spreadsheet_file> _sf;

    };

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


    std::unordered_map<std::string, ss::spreadsheet> server_controller::current_spreadsheets;
    std::fstream server_controller::file;

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
            // messageType = "selected";
            messageType = "cellSelected";
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

    struct invalid_request {
        std::string messageType;
        std::string cellName;
        std::string message;

        invalid_request(std::string cell, std::string error_message)
        {
            messageType = "requestError";
            cellName = cell;
            message = error_message;
        }

        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(invalid_request, messageType, cellName, message)
    };

    struct server_shutdown {
        std::string messageType;
        std::string message;

        server_shutdown(std::string error_message)
        {
            messageType = "serverError";
            message = error_message;
        }

        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(server_shutdown, messageType, message)
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

        // Read from file and print working spreadsheets
//        std::string path = "/";
//
//        for (const auto & file : std::experimental::filesystem::directory_iterator(path))
//            std::cout << file.path() << std::endl;

        DIR* dir;
        struct dirent* diread;
        std::unordered_set<std::string> spreadsheetz;

        // TODO: fix this hardcoding
        if ((dir = opendir("/Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/network_server/spreadsheetz")) != nullptr) {
            while ((diread = readdir(dir)) != nullptr) {
                if ( !strcmp(diread->d_name, ".") || !strcmp(diread->d_name, "..") )
                {
                    // do nothing (straight logic)
                } else {
                    spreadsheetz.insert(diread->d_name);
                }

            }
            closedir (dir);

        } else {
            //perror ("opendir");
            std::cout << "MM PROBABLY THROW AN ERROR HERE" << std::endl;
        }

        std::string spreadsheets_list;

        //for (auto ssz : spreadsheetz)
        int i = 0;
        for (std::string ssz : spreadsheetz)
        {
            if (i == spreadsheetz.size() - 1)
                spreadsheets_list.append(ssz) + "\n\n"; // idk why it's not actually printing a new line here
            else
                spreadsheets_list.append(ssz) + "\n";

            i++;
        }



        //Send current spreadsheet separated by /n or \n\n if no spreadsheets are available
        // TODO COMMENTED OUT
//        int i = 0;
//        for (auto const &element : current_spreadsheets)
//        {
//            if (i == current_spreadsheets.size() - 1)
//                spreadsheets_list.append(element.first + "\n\n");
//            else spreadsheets_list.append(element.first + "\n");
//            i++;
//        }

        if (spreadsheets_list.empty())
            spreadsheets_list = "\n\n";

        // send the list of spreadsheets to the client
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
        std::string crap = state.get_data();
        std::regex newlines_re("\r\n+|\r|\n");
        auto selection = std::regex_replace(crap, newlines_re, "");
        state.spreadsheet = selection;
//        std::string selection = state.get_data();
//        state.spreadsheet = selection;

        std::cout << "Spreadsheet selected: " << selection << std::endl;

        // Go through current folder and look for existing files
//        using std::experimental::filesystem::directory_iterator;

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
//
        // Read from file if it exists
//        auto synchronizedFile = std::make_shared<spreadsheet_file>(state.spreadsheet);
//        Reader reader(synchronizedFile);
//        std::vector<std::string> contents = reader.read_from_file();

        // TODO COMMENTED OUT

        // why tf does this not work
        std::string file_path = "/Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/network_server/spreadsheetz/" + crap;
        std::cout << file_path << std::endl;
        //std::ifstream file(file_path.c_str());
        std::ifstream file("../spreadsheetz/apple.txt"); // this ain't working unless its the whole path bruh
        std::vector<std::string> contents;

        //file.open(file_path.c_str());
        if (file.is_open()) {

            std::string line;
            while (std::getline(file, line)) {
                // using printf() in all tests for consistency
                contents.push_back(line.c_str()); // should it be c_str here???
            }
            file.close();
        }

        // send contents of the file to client
        for (std::string s : contents)
        {
            send(state.get_socket(), s.c_str(), strlen(s.c_str()), 0);
        }
//        for (auto &c : cells)
//        {
//            std::string cell_update =
//                    std::string("{\"messageType\":\"cellUpdated\",cellName:") + "\"" + c.first + "\"" +
//                    ",\"contents\": \"" +
//                    c.second + "\"}\n";
//            send(state.get_socket(), cell_update.c_str(), strlen(cell_update.c_str()), 0);
//        }
        std::function<void(socket_state &)> callback = receive_cell_selection;
        state.on_network_action = callback;
    }

//    std::wstring ExePath() {
//        TCHAR buffer[MAX_PATH] = { 0 };
//        GetModuleFileName( NULL, buffer, MAX_PATH );
//        std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
//        return std::wstring(buffer).substr(0, pos);
//    }


    void server_controller::receive_cell_selection(network_util::socket_state &state)
    {
//        if (state.ErrorOccured)
//            return;

        auto synchronizedFile = std::make_shared<spreadsheet_file>(state.spreadsheet);
        Writer writer1(synchronizedFile);
        std::vector<std::string> commands = process_data(state);
        nlohmann::json data = nlohmann::json::parse(commands[0]);

        nlohmann::json j;
        std::string command_to_send;

        std::cout << "REQUEST TYPE: " << data["requestType"] << std::endl;
//        std::string cell_name = data["cellName"];

        if (data["requestType"] == "editCell")
        {
            cell_updated c(data["cellName"], data["contents"]); // cell updated class

            c.to_json(j, c);

            // Check here if cell content would cause a circular dependency
            // if it does, send error message
            // otherwise, server makes the change and sends "cellUpdated" to all users including itself
            try
            {
                current_spreadsheets[state.spreadsheet].set_contents_of_cell(data["cellName"], data["contents"]);
                writer1.write_to_file(to_string(j));
            }
            catch (const std::runtime_error &e)
            {
                invalid_request c(data["cellName"], e.what());
                c.to_json(j, c);
                //current_spreadsheets[state.spreadsheet].add_to_history((to_string(j) + "\n"));
                //command_to_send = to_string(j) + "\n";
            }

            //current_spreadsheets[state.spreadsheet].add_to_history(to_string(j) + "\n");
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
            try
            {
                current_spreadsheets[state.spreadsheet].revert_cell_contents(data["cellName"]);
                cell_updated c(data["cellName"],
                               current_spreadsheets[state.spreadsheet].get_cell_contents(data["cellName"]));
                c.to_json(j, c);
                writer1.write_to_file(to_string(j));
            }
            catch (std::runtime_error &e)
            {
                invalid_request c(data["cellName"], e.what());
                c.to_json(j, c);
            }
        } else if (data["requestType"] == "undo")
        {
            std::pair<std::string, std::string> undo_contents = {"", ""};
            try
            {
                undo_contents = current_spreadsheets[state.spreadsheet].get_undo_contents();
                current_spreadsheets[state.spreadsheet].undo();
                cell_updated c(undo_contents.first, undo_contents.second); // cell updated class
                c.to_json(j, c);
                writer1.write_to_file(to_string(j));
            }
            catch (std::runtime_error &e)
            {
                invalid_request c(undo_contents.first, e.what());
                c.to_json(j, c);
            }
        }

        command_to_send = to_string(j) + "\n";

        // TODO: have to lock file when writing to it
        // Create the synchronized file
        // Create the writers using the same synchronized file


        // BROADCAST CHANGES TO SPREADSHEETS
        std::string new_id = std::to_string(state.get_id()) + " made edit\n";
        for (long s : get_spreadsheets()[state.spreadsheet].get_users_connected())
        {
            //if (s != state.get_socket())
            send(s, command_to_send.c_str(), strlen(command_to_send.c_str()), 0);
        }
        if (j["messageType"] == "cellUpdated")
        {
            //////////////////////
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