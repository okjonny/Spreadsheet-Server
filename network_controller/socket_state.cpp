#include "socket_state.h"
//#include <boost/filesystem/operations.hpp>
//#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>
#include <mutex>

//using namespace boost::asio;
//using ip::tcp;

namespace networking_util
{
    //the_socket = null;
    //next_id = 0;

    //std::function<networking_util::socket_state()> &to_call,

    socket_state::socket_state(std::function<networking_util::socket_state()> &to_call, boost::asio::ip::tcp::socket &s)
    {
        on_network_action = to_call;
        the_socket = &s;
        next_id = 0;
        id = next_id++;
    }

    std::string socket_state::get_error_message() const
    {
        return error_message;
    }

    bool socket_state::get_error_occured() const
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
        data.str( std::string() );
        data.clear();
    }

}
