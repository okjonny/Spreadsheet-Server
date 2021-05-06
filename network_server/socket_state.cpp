#include "socket_state.h"
#include <mutex>
#include <codecvt>

namespace network_util
{
    /**
     * Empty socket state constructor
     */
    socket_state::socket_state()
    {}

    /**
     * Socket state object constructor
     * @param to_call
     * @param id
     */
    socket_state::socket_state(std::function<void(socket_state &)> &to_call, int id)
    {
        on_network_action = to_call;
        state_id = id;
    }

    /**
     * Retruns the error message
     * @return error message
     */
    std::string socket_state::get_error_message() const
    {
        return error_message;
    }

    /**
     * Returns true or false if an error occured
     * @return
     */
    bool socket_state::get_error_occured() const
    {
        return error_occured;
    }

    /**
     * Returns the id of the socket
     * @return
     */
    long socket_state::get_socket() const
    {
        return socket;
    }

    /**
     * The id that is given from the spreadsheet
     * @return
     */
    int socket_state::get_id() const
    {
        return state_id;
    }

    /**
     * Retrieves the data from the socket
     * @return
     */
    std::string socket_state::get_data()
    {
        std::string retval;
        {
            std::lock_guard<std::mutex> guard(data_lock);
            retval = data;
        }
        return retval;
    }


    /**
     * Retrieves the username of the spreadsheet
     * @return
     */
    std::string socket_state::get_username()
    {
        std::string retval;
        {
            std::lock_guard<std::mutex> guard(data_lock);
            retval = username;
        }
        return retval;
    }

    /**
     * Removes the data from the buffer
     * @param start - starting index of string
     * @param length - end of string wanting to remove
     */
    void socket_state::remove_data(int start, int length)
    {
        {
            std::lock_guard<std::mutex> guard(data_lock);
            data.erase(start, length);
        }
    }

    /**
     * Clears the data from the data
     */
    void socket_state::clear_data()
    {
        {
            std::lock_guard<std::mutex> guard(data_lock);
            data.clear();
        }
    }

    /**
     * Setting socket state = other
     * @param other - socket to equal to
     * @return this
     */
    socket_state &socket_state::operator=(const socket_state &other)
    {
        return *this;
    }

    /**
     * Make a copy
     * @param other - socket to set equal to
     */
    socket_state::socket_state(const network_util::socket_state &other)
    {
        *this = other;
    }

}
