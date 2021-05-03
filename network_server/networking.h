#include <sys/socket.h>
#include <netinet/in.h>
#include <mutex>
#include <thread>
#include "socket_state.h"

#ifndef TEAMPOG_CS3505_SERVER_H
#define TEAMPOG_CS3505_SERVER_H

namespace network_util
{
    class networking {
        static char buffer[4096];
        static int valread;
        static int server_fd;
        static struct sockaddr_in address;
        static int opt;
        static int addrlen;
        static std::thread threads[100];
        static int thread_id;
        static std::mutex mtx;
        static socket_state clients[100];

    public:
        static int start_server(std::function<void(socket_state & )> &to_call);

        static void receive_callback(int thread_id);

        static void accept_new_clients(std::function<void(socket_state & )> &to_call);
//        static void broadcast_spreadsheets();
    private:
        static void error_call(socket_state &error_state);
    };
}
#endif //TEAMPOG_CS3505_SERVER_H
