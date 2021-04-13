#include "socket_state.h"
#include<boost/asio.hpp>
#include <mutex>

#ifndef SOCKSTATE_H
#define SOCKSTATE_H

using namespace boost::asio;

namespace networking_util
{
    // ! Fix comments
    class socket_state
    {
        friend class networking;

        public:
            // member variables
            const tcp::socket the_socket;
            const int buffer_size;
            const long id;

            // methods
            socket_state();
            std::function<socket_state> on_network_action;
            std::string get_error_message;
            bool get_error_occured;
            void remove_data(int start, int length);
            void clear_data();

        private:
            //
            // member variables

            std::byte[] buffer;
            std::basic_stringstream data;
            std::string error_message;

            static long next_id;
            std::mutex data_mutex;

    }
}