#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <string>

namespace websocket = boost::beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class WebSocketClient {
private:
    net::io_context ioc;
    tcp::resolver resolver_;
    websocket::stream<tcp::socket> ws_;

public:
    WebSocketClient();

    void connect(const std::string& host, const std::string& port, const std::string& path);

    void send(const std::string& message);

    std::string receive();
    void receiveAsync();

    void run();

    void close();
};