#include <mutex>
#include <string>
#include <functional>


#ifndef SOCKSTATE_H
#define SOCKSTATE_H

namespace network_util
{
    class socket_state {

        friend class networking;

        std::string data;
        int state_id;
        std::string error_message;
        bool error_occured;
        std::mutex data_lock;

        long socket;

    public:
        std::string spreadsheet;
        char buffer[4096];
        std::string username;
        std::function<void(socket_state &)> on_network_action;

        socket_state();

        socket_state(const network_util::socket_state &);

        socket_state &operator=(const socket_state &other);

        socket_state(std::function<void(socket_state &)> &to_call, int state_id);

        std::string get_error_message() const;

        bool get_error_occured() const;

        std::string get_data();

        std::string get_username();

        int get_id() const;

        long get_socket() const;

        void remove_data(int start, int length);

        void clear_data();
    };
}

#endif
