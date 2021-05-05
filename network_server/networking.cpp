#include <iostream>
#include <functional>
#include "socket_state.h"
#include "networking.h"
#include <unistd.h>
#include <stdio.h>
#include <array>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <cstdlib>
#include <thread>
#include <unordered_map>
#include <server_controller.h>

#define PORT 1100

namespace network_util
{
    int networking::addrlen = sizeof(address);
    int networking::opt = 1;
    int networking::thread_id = 0;
    int networking::valread;
    char networking::buffer[4096];
    int networking::server_fd;
    struct sockaddr_in networking::address;
    std::thread networking::threads[100];
    std::mutex networking::mtx;
    socket_state networking::clients[100];


    int networking::start_server(std::function<void(socket_state &)> &to_call)
    {
        //std::cout << "Running..." << std::endl;
        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
//if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        // Forcefully attaching socket to the port 8080
        if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        accept_new_clients(to_call);
        return 0;
    }

    void networking::stop_server(std::string shutdown_message)
    {
        for (const socket_state &c : clients)
        {
            if(!c.get_error_occured())
                send(c.get_socket(), shutdown_message.c_str(), strlen(shutdown_message.c_str()), 0);
        }

        for(std::thread &t : threads)
        {
            if (t.joinable())
                t.detach();
//                t.join();

        }

        std::cout << "Shutting down..." << std::endl;
    }


    void networking::accept_new_clients(std::function<void(socket_state &)> &to_call)
    {
        // Accepting clients forever
        while (true)
        {
            socket_state c(to_call, thread_id);

            if (listen(server_fd, 3) < 0)
            {
                perror("listen");
                exit(EXIT_FAILURE);
            }
            if ((c.socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            if (threads[thread_id].joinable())
            {
                threads[thread_id].join();
            }
            std::cout << c.socket << std::endl;

            clients[thread_id].socket = c.socket;

            clients[thread_id].state_id = thread_id;
            clients[thread_id].on_network_action = to_call;
            threads[thread_id] = std::thread(&networking::receive_callback, thread_id);
            thread_id++;
        }
    }

    void networking::receive_callback(int thread_id)
    {
        //try {
        std::cout << "Operating on Threadid " << (long) thread_id << std::endl;
        while (true)
        {
//            for(spreadsheet s: ss::server_controller::get_spreadsheets())

            // mtx.lock();
            std::fill_n(clients[thread_id].buffer, 4096, 0);


            valread = read(clients[thread_id].socket, clients[thread_id].buffer, 4096);


            //std::cout << thread_id << ":" << clients[thread_id].buffer << std::endl;
            std::string s = clients[thread_id].buffer;
            clients[thread_id].data = s;

            std::cout << thread_id << "======================VALREAD: " << valread << std::endl;
//            int value = std::stoi(s);


//                try {



            if (valread < 1)
            {
                error_call(clients[thread_id]);
                std::cout << "ERROR" << std::endl;
                break;
            }

            clients[thread_id].on_network_action(clients[thread_id]);


//                }
//                catch(...)
//                {
//                    break;
//                }
//            value++;
//            std::string sToSend = std::to_string(value);
//            send(clients[thread_id].socket, sToSend.c_str(), strlen(sToSend.c_str()), 0);
            std::fill_n(clients[thread_id].buffer, 4096, 0);
            //   mtx.unlock();
        }

        //      }
/*        catch(...)
        {
            std::cout << thread_id << " disconnected" << std::endl;
        }*/

/*       if(threads[thread_id].joinable())
           threads[thread_id].join();
       */
        //networking::thread_id--;
    }

    void networking::error_call(socket_state &error_state)
    {
        error_state.error_occured = true;
        error_state.error_message = std::string("Disconnected DE CLIENT.");
        error_state.on_network_action(error_state);
    }
}