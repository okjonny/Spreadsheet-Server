#include "networking.h"
#include "socket_state.h"
#include "server_controller.h"

using namespace network_util;

//namespace spreadsheet_server
//{
int main()
{
    spreadsheet_server::server_controller spreadsheet;
    spreadsheet.start_server();
//
//        std::function<void (socket_state & )> boom = boomer;
//        network_util::networking::start_server(boom);

    while (true)
    {}
}
//}