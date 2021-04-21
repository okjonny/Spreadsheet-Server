//#include <boost/filesystem/operations.hpp>
//#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <mutex>
#include <string>

#ifndef SOCKSTATE_H 
#define SOCKSTATE_H

//using namespace boost::asio;
//using ip::tcp;


namespace network_util
{
    // ! Fix comments
    class socket_state
    {
        friend class networking;
        
        // member variables
        std::wstring data;
        std::string error_message;
        bool error_occured;
        static long next_id; //used to be static
        std::mutex data_lock;
        // boost::asio::mutable_buffer the_buffer; // TODO: change to boost buffer

        public:
            // member variables
            boost::asio::ip::tcp::socket *the_socket;
            long id;

            // methods
            socket_state(const network_util::socket_state& other);
            socket_state(std::function<network_util::socket_state(network_util::socket_state() &)> & to_call, boost::asio::ip::tcp::socket &s);
            std::function<network_util::socket_state(network_util::socket_state() &)> * on_network_action;
            std::string get_error_message() const;
            bool get_error_occured() const;

            socket_state & operator=(const network_util::socket_state & other);

            std::string get_data();
            void remove_data(int start, int length);
            void clear_data();

        private:
            void receive_callback(const boost::system::error_code& error, std::size_t num_bytes);
    };
}

#endif