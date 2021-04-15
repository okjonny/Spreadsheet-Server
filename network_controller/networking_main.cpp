#include <iostream>
#include "socket_state.h"
#include <boost/asio.hpp>

network_util::socket_state on_network_action2();
network_util::socket_state on_network_action();

int main()
{
    boost::asio::io_context context;
    boost::asio::ip::tcp::socket the_socket(context); 

    network_util::socket_state boomer(on_network_action, the_socket);

    std::cout << "I have the right \"includes\" :)" << std::endl;
    std::cout << "Compiled Fine." << std::endl;

    return 0;
}


network_util::socket_state on_network_action2()
{
    boost::asio::io_context context;
    boost::asio::ip::tcp::socket socket(context); 
    network_util::socket_state new_socket(on_network_action, socket);
    return new_socket;
}

network_util::socket_state on_network_action()
{
    boost::asio::io_context context;
    boost::asio::ip::tcp::socket socket(context); 
    network_util::socket_state new_socket(on_network_action, socket);
    return new_socket;
}