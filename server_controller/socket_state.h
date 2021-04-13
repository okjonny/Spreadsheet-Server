#include <boost/filesystem/operations.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>
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
            const ip::tcp::socket the_socket;
            const int buffer_size;
            const long id;

            // methods
            socket_state();
            std::function<networking_util::socket_state> on_network_action;
            std::string get_error_message;
            bool get_error_occured;
            void remove_data(int start, int length);
            void clear_data();

        private:
            // member variables
            std::byte buffer [buffer_size];
            std::basic_stringstream data;
            std::string error_message;

            static long next_id;
            std::mutex data_mutex;
    };
}
#endif