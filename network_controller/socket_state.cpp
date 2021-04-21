#include "socket_state.h"
//#include <boost/filesystem/operations.hpp>
//#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>
#include <boost/locale.hpp>
#include <mutex>
#include <locale>
#include <codecvt>
//using namespace boost::asio;
//using ip::tcp;

namespace network_util
{
    //the_socket = null;
    long socket_state::next_id = 0;
    boost::asio::mutable_buffer the_buffer(); // TODO: change to boost buffer
    //std::function<networking_util::socket_state()> &to_call,

    socket_state::socket_state(const network_util::socket_state & other)
    {
        *this = other;
    }

    socket_state::socket_state(std::function<network_util::socket_state(network_util::socket_state()&)> & to_call, boost::asio::ip::tcp::socket &s)
    {
        on_network_action = & to_call;
        the_socket = &s;
        id = next_id++;
    }

    socket_state & socket_state::operator=(const socket_state & other)
    {
        //Fill in with copying shtuff
        return *this;
    }

    std::string socket_state::get_error_message() const
    {
        return error_message;
    }

    bool socket_state::get_error_occured() const
    {
        return error_occured;
    }

    std::string socket_state::get_data()
    {
        // std::string retval;
        // {
        //     std::lock_guard<std::mutex> guard(data_lock);
        //     retval = data;
        // }
        // return retval;

        // Stackoverflow
        {
        std::lock_guard<std::mutex> guard(data_lock);
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;
        return converterX.to_bytes(data);
        }
    }

    void socket_state::remove_data(int start, int length)
    {
        {
            std::lock_guard<std::mutex> guard(data_lock);
            data.erase(start, length);
        }
    }

    void socket_state::clear_data()
    {
        {
            std::lock_guard<std::mutex> guard(data_lock);
            data.clear();
        }
    }

     /// <summary>
        /// To be used as the callback for finalizing a receive operation that was initiated by GetData.
        /// 
        /// Uses EndReceive to finalize the receive.
        ///
        /// As stated in the GetData documentation, if an error occurs during the receive process,
        /// either this method or GetData (not both) should indicate the error appropriately.
        /// 
        /// If data is successfully received:
        ///  (1) Read the characters as UTF8 and put them in the SocketState's unprocessed data buffer (its string builder).
        ///      This must be done in a thread-safe manner with respect to the SocketState methods that access or modify its 
        ///      string builder.
        ///  (2) Call the saved delegate (OnNetworkAction) allowing the user to deal with this data.
        /// </summary>
        /// <param name="ar"> 
        /// This contains the SocketState that is stored with the callback when the initial BeginReceive is called.
        /// </param>
        void socket_state::receive_callback(const boost::system::error_code& error, std::size_t num_bytes) // only takes in buffer and num bytes received
        {
            // try
            // {
                //TODO: boost - number of bytes are returned in the parameter
             // int num_bytes = state.TheSocket.EndReceive(ar);

                //If the remote socket is cleanly shutdown, it is an error.
                if (num_bytes == 0)
                {
                    // TODO: ERRORSSSSS
                    //ErrorCall(state, "Remote socket is cleanly shutdown.");
                    //return;
                }

                //std::string new_data = Encoding.UTF8.GetString(state.buffer, 0, numBytes);
                std::string new_data(boost::asio::buffers_begin(the_buffer.data()), boost::asio::buffers_begin(the_buffer.data()) + num_bytes);

                std::wstring encoded_data = boost::locale::conv::to_utf<wchar_t>(new_data,"UTF-8", boost::locale::conv::default_method); 

                //Buffering the data
                {
                    std::lock_guard<std::mutex> guard(data_lock);
                    data += encoded_data;
                }

                on_network_action(this);
            // }
            // TODO
            // catch (Exception e)
            // {
            //     ErrorCall(state, e.Message);
            // }
        }
}
