
#include "ws_client.hpp"

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

void WebSocketClient::close()
{
    ws_.close(websocket::close_code::normal);
}