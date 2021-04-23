#include <mutex>
#include <string>
#include <functional>
#include "server_controller.h"
//#include "server_controller.h"

#ifndef SOCKSTATE_H
#define SOCKSTATE_H
//using namespace spreadsheet_server;
namespace network_util
{
    class socket_state {

        friend class networking;
        friend class server_controller;

        std::string data;
        int socket;
        int id;
        std::string error_message;
        bool error_occured;
        std::mutex data_lock;

    public:
        char buffer[4096];
        std::function<void(socket_state &)> on_network_action;

        socket_state();

        socket_state(const network_util::socket_state &);

        socket_state &operator=(const socket_state &other);

        socket_state(std::function<void(socket_state &)> &to_call, int socket);

        std::string get_error_message() const;

        bool get_error_occured() const;

        std::string get_data();

        int get_socket_id() const;

        void remove_data(int start, int length);

        void clear_data();
    };
}

#endif
