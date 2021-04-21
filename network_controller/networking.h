//#include <boost/filesystem/operations.hpp>
//#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>
#include <mutex>
#include <string>

#ifndef NET_H 
#define NET_H

//using namespace boost::asio;
//using ip::tcp;


namespace network_util
{
    // ! Fix comments
    class networking
    {
        public:
            //Server
            static boost::asio::ip::tcp::acceptor start_server(std::function<network_util::socket_state()> to_call, int port);
            static void stop_server(boost::asio::ip::tcp::acceptor listener);

            //General networking stuff
            static void get_data(socket_state state);
            static bool send(boost::asio::ip::tcp::socket &socket, std::string data);


        private:
            static void send_callback(boost::asio::async_result ar);
            static void accept_new_client(std::function<network_util::socket_state()> to_call, boost::asio::ip::tcp::acceptor listener, boost::asio::ip::tcp::socket client);
            static void error_call(socket_state error_state, std::string error_message);
    };
}

#endif