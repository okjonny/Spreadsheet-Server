#include "networking.h"
#include "socket_state.h"
#include "server_controller.h"
#include <iostream>

using namespace network_util;

//namespace spreadsheet_server
//{
int main()
{
    ss::server_controller spreadsheet;

    // Credits: StackOverflow
    // https://stackoverflow.com/questions/49462557/how-to-terminate-a-listening-tcp-server-with-keyboard-input-in-c
    std::thread t{
            [&spreadsheet] {
                std::string s;
                while (std::cin >> s)
                    if (s == "exit")
                    {
                        spreadsheet.stop_server();
                        std::cout << "exit command given on stdin\n";
                        exit(0);
                    }
            }
    };
    t.detach();

    spreadsheet.start_server();
}