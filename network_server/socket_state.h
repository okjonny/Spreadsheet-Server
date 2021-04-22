#include <mutex>
#include <string>

#ifndef SOCKSTATE_H
#define SOCKSTATE_H

namespace network_util {
    // ! Fix comments
    class socket_state {
        friend class server;

        std::string data;
        int socket;
        std::string error_message;
        bool error_occured;
        std::mutex data_lock;
        char buffer[4096];

    public:
        socket_state();

        socket_state(int _socket);
        std::string get_error_message() const;
        bool get_error_occured() const;

        std::string get_data();

        int get_socket_id() const;

        void remove_data(int start, int length);
        void clear_data();
    };
}

#endif
