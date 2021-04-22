#include "socket_state.h"
#include <mutex>
#include <locale>
#include <codecvt>

namespace network_util {
    //the_socket = null;
    //std::function<networking_util::socket_state()> &to_call,

//    socket_state::socket_state(const network_util::socket_state & other)
//    {
//        *this = other;
//    }

    socket_state::socket_state(int _socket) {
        socket = _socket;
    }

//    socket_state & socket_state::operator=(const socket_state & other)
//    {
//        //Fill in with copying shtuff
//        return *this;
//    }

    std::string socket_state::get_error_message() const {
        return error_message;
    }

    bool socket_state::get_error_occured() const {
        return error_occured;
    }

    std::string socket_state::get_data() {
        std::string retval;
        {
            std::lock_guard<std::mutex> guard(data_lock);
            retval = data;
        }
        return retval;

        // Stackoverflow
        // {
        // std::lock_guard<std::mutex> guard(data_lock);
        // using convert_typeX = std::codecvt_utf8<wchar_t>;
        // std::wstring_convert<convert_typeX, wchar_t> converterX;
        // return converterX.to_bytes(data);
        // }
    }

    void socket_state::remove_data(int start, int length) {
        {
            std::lock_guard<std::mutex> guard(data_lock);
            data.erase(start, length);
        }
    }

    void socket_state::clear_data() {
        {
            std::lock_guard<std::mutex> guard(data_lock);
            data.clear();
        }
    }
}
