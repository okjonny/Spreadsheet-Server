#include "networking.h"
#include "socket_state.h"
#include "server_controller.h"

using namespace network_util;

//namespace spreadsheet_server
//{
int main()
{
    ss::server_controller spreadsheet;
    spreadsheet.start_server();

    //error shutdown --> errbuddy on the server SHUTDOWN METHOD
    while (true)
    {}
}
//}