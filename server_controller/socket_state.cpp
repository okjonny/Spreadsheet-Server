#include "socket_state.h"
#include<boost/asio.hpp>
#include <mutex>

#ifndef SOCKSTATE_H
#define SOCKSTATE_H

using namespace boost::asio;

namespace networking_util
{
    class socket_state
    {
        friend class networking;

        public:
            const tcp::socket the_socket;
            const int buffer_size;

            const long id;
            std::function<socket_state> on_network_action;

            std::string error_message;
            bool error_occured;

        private:
            std::byte[] buffer;
            std::basic_stringstream data;

            static long next_id;
            std::mutex data_mutex;
    }