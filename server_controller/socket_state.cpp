#include "socket_state.h"
#include <boost/filesystem/operations.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>
#include <mutex>

using namespace boost::asio;

namespace networking_util
{
    friend class networking;

    const tcp::socket the_socket = null;
    const int buffer_size = 4096;
    const long id;

    socket_state::socket_state(std::function<networking_util::socket_state> toCall, tcp::socket s)
    {
        on_network_action = to_call;
        the_socket = s;
        id = next_id++;
    }

    std::string socket_state::get_error_message()
    {
        return error_message;
    }

    bool socket_state::get_error_occured()
    {
        return error_occured;
    }

    std::string socket_state::get_data()
    {
        std::string retval;
        {
            std::lock_guard<std::mutex> guard(data_mutex);
            // DO something here
        }
        return retval;
    }

    void socket_state::remove_data(int start, int length)
    {
        // still need to implement
    }

    void socket_state::clear_data()
    {

    }

    static long next_id = 0;
    std::mutex data_mutex;
}
