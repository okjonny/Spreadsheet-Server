#include "socket_state.h"
#include <mutex>
#include <codecvt>

namespace network_util {

    socket_state::socket_state() {}

    socket_state::socket_state(int _socket) {
        socket = _socket;
    }

    std::string socket_state::get_error_message() const {
        return error_message;
    }

    bool socket_state::get_error_occured() const {
        return error_occured;
    }

    int socket_state::get_socket_id() const {
        return socket;
    }

    std::string socket_state::get_data() {
        std::string retval;
        {
            std::lock_guard<std::mutex> guard(data_lock);
            retval = data;
        }
        return retval;
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
