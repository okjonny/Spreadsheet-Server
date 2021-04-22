//#include <unistd.h>
//#include <cstdio>
//#include <cstring>
//#include <string>
//#include <cstdlib>
#include <iostream>
#include <functional>
#include "socket_state.h"
#include "server.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <cstdlib>
#include <thread>

#define PORT 1100

namespace network_util {
    int server::addrlen = sizeof(address);
    int server::opt = 1;
    int server::thread_id = 0;
    int server::valread;
    char server::buffer[4096];
    int server::server_fd;
    struct sockaddr_in server::address;
    std::thread server::threads[100];
    std::mutex server::mtx;
    socket_state server::clients[100];

//    server::server() {
//        addrlen = sizeof(address);
//        opt = 1;
//        thread_id = 0;
//        buffer[4096] = {0};
//    }

    int server::setup_server() {
        std::cout << "Running..." << std::endl;
        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        // Forcefully attaching socket to the port 8080
        if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        listen_for_new_clients();
        return 0;
    }

    void server::listen_for_new_clients() {
        while (true) {
            socket_state c;
            if (listen(server_fd, 3) < 0) {
                perror("listen");
                exit(EXIT_FAILURE);
            }
            if ((c.socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            if (threads[thread_id].joinable()) {
                threads[thread_id].join();
            }
//            threads[thread_id] = std::thread([this] { CountUp(thread_id); });
//            threads[thread_id] = std::thread(std::bind(&server::CountUp, this, thread_id));
            threads[thread_id] = std::thread(&server::CountUp, thread_id);

            clients[thread_id].socket = c.socket;
            thread_id++;
        }
    }

    void server::CountUp(int thread_id) {
        std::cout << "Operating on Threadid " << (long) thread_id << std::endl;
        while (true) {
            mtx.lock();
            std::fill_n(buffer, 4096, 0);
            valread = read(clients[thread_id].socket, buffer, 4096);
            std::cout << thread_id << ":" << buffer << std::endl;
            std::string s = buffer;
//            int value = std::stoi(s);
            if (s == "Disconnect") {
                mtx.unlock();
                break;
            }
//            value++;
//            std::string sToSend = std::to_string(value);
//            send(clients[thread_id].socket, sToSend.c_str(), strlen(sToSend.c_str()), 0);
            std::fill_n(buffer, 4096, 0);
            mtx.unlock();
        }
        server::thread_id--;
    }
}