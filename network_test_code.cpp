#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;
string read_(tcp::socket & socket) {
       boost::asio::streambuf buf;
       boost::asio::read_until( socket, buf, "\n" );
       string data = boost::asio::buffer_cast<const char*>(buf.data());
       return data;
}
void send(tcp::socket & socket, const string& message) {
       const string msg = message + "\n";
       boost::asio::write( socket, boost::asio::buffer(message) );
}

int main() {
      boost::asio::io_service ioservice;
//listen for new connection
      tcp::acceptor acceptor(ioservice, tcp::endpoint(tcp::v4(), 1234 ));
//socket creation 
      tcp::socket socket(ioservice);
//waiting for connection
      acceptor.accept(socket);
//read operation
      string message = read(socket);
      cout << message << endl;
//write operation
      send(socket_, "Hello From Server!");
      cout << "Servent sent Hello message to Client!" << endl;
   return 0;
}