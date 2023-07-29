
#include <iostream>
#include <sstream>
#include <vector>

#include "firmata.h"

#include <boost/beast.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <thread>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

t_firmata *firmata;
int i = 0;

// for string delimiter
std::vector<std::string> split(std::string s, std::string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

struct control_payload
{
    int l_y;
    int l_x;

    int r_y;
    int r_x;
};

enum Direction
{
    FORWARD,
    REVERSE
};

struct dc_motor_value
{
    int power;
    bool active;
    Direction direction;
};

struct hbridge_value
{
    dc_motor_value motor1;
    dc_motor_value motor2;
};

std::ostream &operator<<(std::ostream &lhs, Direction e)
{
    switch (e)
    {
    case FORWARD:
        lhs << "FORWARD";
        break;
    case REVERSE:
        lhs << "REVERSE";
        break;
    }
    return lhs;
}

std::ostream &operator<<(std::ostream &os, dc_motor_value const &arg)
{
    os << "active = " << arg.active << "power = " << arg.power << " direction = " << arg.direction;
    return os;
}

std::ostream &operator<<(std::ostream &os, hbridge_value const &arg)
{
    os << "motor throttle = " << arg.motor1 << " motor steering = " << arg.motor2;
    return os;
}

dc_motor_value get_dc_motor_value(int v)
{

    // 100 et -100,  -100
    auto value = dc_motor_value{};

    int zero_diff = 0;

    if (v < -5)
    {
        value.active = true;
        value.direction = Direction::FORWARD;
        zero_diff = v * -1;
    }
    else if (v > 5)
    {
        value.active = true;
        value.direction = Direction::REVERSE;
        zero_diff = v;
    }
    else
    {
        value.active = false;
    }

    value.power = (zero_diff * 255) / 100;

    return value;
}

hbridge_value get_hbridge_value(control_payload const *control_payload)
{

    return hbridge_value{
        get_dc_motor_value(control_payload->l_y),
        get_dc_motor_value(control_payload->l_x)};
}


hbridge_value *value = NULL;
std::mutex mtx;

void do_session(tcp::socket socket)
{
    try
    {
        // Construct the stream by moving in the socket
        websocket::stream<tcp::socket> ws{std::move(socket)};

        // Set a decorator to change the Server of the handshake
        ws.set_option(websocket::stream_base::decorator(
            [](websocket::response_type &res)
            {
                res.set(http::field::server,
                        std::string(BOOST_BEAST_VERSION_STRING) +
                            " websocket-server-sync");
            }));

        // Accept the websocket handshake
        ws.accept();

        for (;;)
        {
            // This buffer will hold the incoming message
            beast::flat_buffer buffer;

            // Read a message
            ws.read(buffer);

            boost::asio::const_buffer data_buffer = buffer.data();
            std::string result(boost::asio::buffer_cast<const char *>(data_buffer), boost::asio::buffer_size(data_buffer));

            //ws.text(ws.got_text());
            //ws.write(buffer.data());

            const auto delimiter = ",";
            const auto payload = result;

            const auto v = split(payload, delimiter);

            if (v.size() != 4)
            {
                continue;
            }

            std::vector<int> newV;
            newV.reserve(v.size()); //  avoids unnecessary reallocations
            std::transform(v.begin(), v.end(),
                           std::back_inserter(newV),
                           [](std::string str)
                           { return std::atoi(str.c_str()); });

            const auto control_struct = control_payload{newV.at(1), newV.at(0), newV.at(3), newV.at(2)};
            const auto hbridge = get_hbridge_value(&control_struct);


            mtx.lock();

            value = (hbridge_value *)malloc(sizeof(hbridge_value));
            value->motor1.active = hbridge.motor1.active;
            value->motor1.direction = hbridge.motor1.direction;
            value->motor1.power = hbridge.motor1.power;

            mtx.unlock();

        }
    }
    catch (beast::system_error const &se)
    {
        // This indicates that the session was closed
        if (se.code() != websocket::error::closed)
            std::cerr << "Error: " << se.code().message() << std::endl;
    }
    catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main()
{

    //std::thread(&thread_motor).detach();

    try
    {

        auto const address = net::ip::make_address("0.0.0.0");
        auto const port = static_cast<unsigned short>(std::atoi("9002"));

        // The io_context is required for all I/O
        net::io_context ioc{1};

        // The acceptor receives incoming connections
        tcp::acceptor acceptor{ioc, {address, port}};
        for (;;)
        {
            // This will receive the new connection
            tcp::socket socket{ioc};

            // Block until we get a connection
            acceptor.accept(socket);

            // Launch the session, transferring ownership of the socket
            std::thread(
                &do_session,
                std::move(socket))
                .detach();

             firmata = firmata_new("/dev/ttyACM0"); // init Firmata

    while (!firmata->isReady) // Wait until device is up
        firmata_pull(firmata);

    firmata_pinMode(firmata, 7, MODE_OUTPUT); // set pin 13 (led on most arduino) to out
    firmata_pinMode(firmata, 8, MODE_OUTPUT); // set pin 13 (led on most arduino) to out

    while (true)
    {
        mtx.lock();

        if (value != NULL)
        {

            if (value->motor1.active == true) {

                if (value->motor1.direction == FORWARD)
                {

                    firmata_digitalWrite(firmata, 7, LOW);  // light led
                    firmata_digitalWrite(firmata, 8, HIGH); // light led
                    std::cout << "forward" << std::endl;

                }
                else
                {

                    firmata_digitalWrite(firmata, 7, HIGH); // light led
                    firmata_digitalWrite(firmata, 8, LOW);  // light led
                    std::cout << "reverse" << std::endl;

                }


            } else {
                firmata_digitalWrite(firmata, 7, HIGH); // light led
                firmata_digitalWrite(firmata, 8, HIGH);  // light led
                std::cout << "stopping" << std::endl;
            }
            
            delete value;
            value = NULL;
        }

        mtx.unlock();

        sleep(2);
    }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}