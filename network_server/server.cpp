#include "networking.h"
#include "socket_state.h"
#include "server_controller.h"
#include <iostream>

using namespace network_util;

//namespace spreadsheet_server
//{
int main()
{
    int command;
    ss::server_controller spreadsheet;
    spreadsheet.start_server();
//
//    std::cin >> command;
//    if(command == 0){
//        spreadsheet.stop_server();
//        return 0;
//    }

    while (true)
    {

    }
}
//}