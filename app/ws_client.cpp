
#include "ws_client.hpp"

#include <ostream>

WebSocketClient::WebSocketClient() : resolver_(ioc), ws_(ioc)
{
}

void WebSocketClient::connect(const std::string &host, const std::string &port, const std::string &path)
{
    // Resolve the host
    auto const results = resolver_.resolve(host, port);
    // Make the connection on the IP address we get from a lookup
    auto ep = net::connect(ws_.next_layer(), results);

    // Perform the websocket handshake
    ws_.handshake(host, path);
}

void WebSocketClient::send(const std::string &message)
{
    //ws_.async_write(net::buffer(message));
    ws_.write(net::buffer(message));
}

std::string WebSocketClient::receive()
{
    boost::beast::multi_buffer buffer;
    ws_.read(buffer);
    return boost::beast::buffers_to_string(buffer.data());
}

void WebSocketClient::receiveAsync() {
    boost::beast::flat_buffer buffer;
    ws_.async_read(buffer,
    [&buffer](boost::system::error_code ec, std::size_t bytes_transferred) {
              if (ec)
      {
         // Handle the error (e.g., close the WebSocket or log the error)
         std::cerr << "failed to error_code" << std::endl;
      }
      else
      {
         // Use the data in `buffer`
         std::cout << "reading from buffer" << std::endl;

      }
    }
    );
}

void WebSocketClient::run() {
    ioc.run();
}


void WebSocketClient::close()
{
    ws_.close(websocket::close_code::normal);
}