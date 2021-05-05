#include "socket_state.h"
#include <mutex>
#include <codecvt>

namespace network_util
{
    socket_state::socket_state()
    {}

    socket_state::socket_state(std::function<void(socket_state &)> &to_call, int id)
    {
        on_network_action = to_call;
        state_id = id;
    }

    std::string socket_state::get_error_message() const
    {
        return error_message;
    }

    bool socket_state::get_error_occured() const
    {
        return error_occured;
    }

    long socket_state::get_socket() const
    {
        return socket;
    }

    int socket_state::get_id() const
    {
        return state_id;
    }

    std::string socket_state::get_data()
    {
        std::string retval;
        {
            std::lock_guard<std::mutex> guard(data_lock);
            retval = data;
        }
        return retval;
    }


    std::string socket_state::get_username()
    {
        std::string retval;
        {
            std::lock_guard<std::mutex> guard(data_lock);
            retval = username;
        }
        return retval;
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

    socket_state &socket_state::operator=(const socket_state &other)
    {
        //Fill in with copying shtuff
        return *this;
    }

    socket_state::socket_state(const network_util::socket_state &other)
    {
        *this = other;
    }

}
