#include "socket_state.h"
//#include <boost/filesystem/operations.hpp>
//#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>
#include <boost/locale.hpp>
#include <mutex>
#include <locale>
#include <codecvt>
//using namespace boost::asio;
//using ip::tcp;

namespace network_util
{
    //the_socket = null;
    long socket_state::next_id = 0;
    boost::asio::mutable_buffer the_buffer(); // TODO: change to boost buffer
    //std::function<networking_util::socket_state()> &to_call,

    socket_state::socket_state(const network_util::socket_state & other)
    {
        *this = other;
    }

    socket_state::socket_state(std::function<network_util::socket_state(network_util::socket_state()&)> & to_call, boost::asio::ip::tcp::socket &s)
    {
        on_network_action = & to_call;
        the_socket = &s;
        id = next_id++;
    }

    socket_state & socket_state::operator=(const socket_state & other)
    {
        //Fill in with copying shtuff
        return *this;
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
        // std::string retval;
        // {
        //     std::lock_guard<std::mutex> guard(data_lock);
        //     retval = data;
        // }
        // return retval;

        // Stackoverflow
        {
        std::lock_guard<std::mutex> guard(data_lock);
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;
        return converterX.to_bytes(data);
        }
    }

    void socket_state::remove_data(int start, int length)
    {
        {
            std::lock_guard<std::mutex> guard(data_lock);
            data.erase(start, length);
        }
    }

    void socket_state::clear_data()
    {
        {
            std::lock_guard<std::mutex> guard(data_lock);
            data.clear();
        }
    }
}
