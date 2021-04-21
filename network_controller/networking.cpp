#include <boost/asio.hpp>
#include "networking.h"

namespace network_util
{
        /////////////////////////////////////////////////////////////////////////////////////////
        // Server-Side Code
        /////////////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Starts a TcpListener on the specified port and starts an event-loop to accept new clients.
        /// The event-loop is started with BeginAcceptSocket and uses AcceptNewClient as the callback.
        /// AcceptNewClient will continue the event-loop.
        /// </summary>
        /// <param name="toCall">The method to call when a new connection is made</param>
        /// <param name="port">The the port to listen on</param>
        static boost::asio::ip::tcp::acceptor start_server(std::function<network_util::socket_state()> to_call, int port)
        {
            boost::asio::ip::tcp::endpoint target(asio::ip::address_v4::any(), port);
            boost::asio::io_service service;

            // TODO: WHICH SOCKET TO PASS?
            boost::asio::ip::tcp::socket new_client(service, target);
            boost::asio::ip::tcp::acceptor listener(service, target.protocol());

            // try
            // {
            listener.listen();
            // passing in error??
            listener.async_accept(new_client, boost::bind(accept_new_client, to_call, listener, new_client));
            
            // }
            // catch (Exception) { }


            return listener;
        }

        /// <summary>
        /// To be used as the callback for accepting a new client that was initiated by StartServer, and 
        /// continues an event-loop to accept additional clients.
        ///
        /// Uses EndAcceptSocket to finalize the connection and create a new SocketState. The SocketState's
        /// OnNetworkAction should be set to the delegate that was passed to StartServer.
        /// Then invokes the OnNetworkAction delegate with the new SocketState so the user can take action. 
        /// 
        /// If anything goes wrong during the connection process (such as the server being stopped externally), 
        /// the OnNetworkAction delegate should be invoked with a new SocketState with its ErrorOccured flag set to true 
        /// and an appropriate message placed in its ErrorMessage field. The event-loop should not continue if
        /// an error occurs.
        ///
        /// If an error does not occur, after invoking OnNetworkAction with the new SocketState, an event-loop to accept 
        /// new clients should be continued by calling BeginAcceptSocket again with this method as the callback.
        /// </summary>
        /// <param name="ar">The object asynchronously passed via BeginAcceptSocket. It must contain a tuple with 
        /// 1) a delegate so the user can take action (a SocketState Action), and 2) the TcpListener</param>
        static void accept_new_client(std::function<network_util::socket_state()> to_call, boost::asio::ip::tcp::acceptor listener, boost::asio::ip::tcp::socket client) // const boost::system::error_code& error)
        {

            //Tuple<Action<SocketState>, TcpListener> arBreakdown = (Tuple<Action<SocketState>, TcpListener>)ar.AsyncState;
            //TcpListener listener = (TcpListener)arBreakdown.Item2;
            boost::asio::ip::tcp::acceptor acceptor = listen;
            
            //if(!error)
            try
            {
                // TODO: WHICH SOCKET TO USE??
                //Socket newClient = listener.EndAcceptSocket(ar);
                boost::asio::ip::tcp::socket new_client = client;
               // SocketState state = new SocketState(arBreakdown.Item1, newClient);
                socket_state state(to_call, new_client);
                state.on_network_action(state);
                acceptor.async_accept(new_client, boost::bind(accept_new_client, to_call, listener, new_client));
            }
            catch (Exception e)
            {
                SocketState errorState = new SocketState(arBreakdown.Item1, null);
                ErrorCall(errorState, e.Message);
            }
        }

        /// <summary>
        /// Stops the given TcpListener.
        /// </summary>
        static void stop_server(boost::asio::ip::tcp::acceptor listener)
        {
            try
            {
                listener.close();
            }
            catch (Exception) { }
        }

        /////////////////////////////////////////////////////////////////////////////////////////
        // Server and Client Common Code
        /////////////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Begins the asynchronous process of receiving data via BeginReceive, using ReceiveCallback 
        /// as the callback to finalize the receive and store data once it has arrived.
        /// The object passed to ReceiveCallback via the AsyncResult should be the SocketState.
        ///  
        /// If anything goes wrong during the receive process, the SocketState's ErrorOccured flag should 
        /// be set to true, and an appropriate message placed in ErrorMessage, then the SocketState's
        /// OnNetworkAction should be invoked. Between this method and ReceiveCallback, OnNetworkAction should only be 
        /// invoked once on error.
        /// 
        /// </summary>
        /// <param name="state">The SocketState to begin receiving</param>
        public static void get_data(socket_state state)
        {
            try
            {
                //state.the_socket.BeginReceive(state.buffer, 0, state.buffer.Length, SocketFlags.None, ReceiveCallback, state);
                state.the_socket.async_receive(state.buffer, 0, boost::bind(state.receive_callback, state));
            }
            catch (Exception e)
            {
                ErrorCall(state, e.Message);
            }
        }

       

        /// <summary>
        /// Begin the asynchronous process of sending data via BeginSend, using SendCallback to finalize the send process.
        /// 
        /// If the socket is closed, does not attempt to send.
        /// 
        /// If a send fails for any reason, this method ensures that the Socket is closed before returning.
        /// </summary>
        /// <param name="socket">The socket on which to send the data</param>
        /// <param name="data">The string to send</param>
        /// <returns>True if the send process was started, false if an error occurs or the socket is already closed</returns>
        public static bool send(boost::asio::ip::tcp::socket socket, std::wstring data)
        {
            if (socket.is_open())
            {
                try
                {
                    //byte[] messageBuffer = Encoding.UTF8.GetBytes(data);
                    //socket.BeginSend(messageBuffer, 0, messageBuffer.Length, SocketFlags.None, SendCallback, socket);
                    socket.async_send(boost::asio::buffer(data, data.size()), handler);

                    return true;
                }
                catch (Exception)
                {
                    socket.close();
                }
            }
            return false;
        }

        /// <summary>
        /// To be used as the callback for finalizing a send operation that was initiated by Send.
        ///
        /// Uses EndSend to finalize the send.
        /// 
        /// This method must not throw, even if an error occured during the Send operation.
        /// </summary>
        /// <param name="ar">
        /// This is the Socket (not SocketState) that is stored with the callback when
        /// the initial BeginSend is called.
        /// </param>
        private static void send_callback(const boost::system::error_code& error, size_t num_bytes)
        {
            //Socket client = (Socket)ar.AsyncState;

        //    try
          //  {
          //      client.EndSend(ar);
        //    }
         //   catch (Exception) { }
        }

        /// <summary>
        /// To be invoked whenever an error occurs in this library.
        /// 
        /// Changes a SocketState object's ErrorOccured flag to true,
        /// and its ErrorMessage to a provided errorMessage.
        /// 
        /// Finally invokes the delegate that the new SocketState holds.
        /// </summary>
        /// <param name="errorState">The SocketState with the error.</param>
        /// <param name="errorMessage">The message corresponding to the error as desired.</param>
        private static void error_call(socket_state error_state, std::string error_message)
        {
            error_state.error_occured = true;
            error_state.error_message = error_message;

            error_state.on_network_action(error_state);
        }
}