#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <iostream>
#include <sstream>
#include <vector>

#include "firmata.h"


typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

t_firmata *firmata;
int i = 0;


// for string delimiter
std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}


struct control_payload {
    int l_y;
    int l_x;

    int r_y;
    int r_x;
};

enum Direction {
    FORWARD,
    REVERSE
};

struct dc_motor_value {
    int power;
    bool active;
    Direction direction;
};

struct hbridge_value {
    dc_motor_value motor1;
    dc_motor_value motor2;
};

std::ostream& operator<<(std::ostream& lhs, Direction e) {
    switch(e) {
    case FORWARD: lhs << "FORWARD"; break;
    case REVERSE: lhs << "REVERSE"; break;
    }
    return lhs;
}

std::ostream& operator<<(std::ostream& os, dc_motor_value const& arg) {
    os << "active = " << arg.active << "power = " << arg.power << " direction = " << arg.direction;
    return os;
}

std::ostream& operator<<(std::ostream& os, hbridge_value const& arg) {
    os << "motor throttle = " << arg.motor1 << " motor steering = " << arg.motor2;
    return os;
}

dc_motor_value get_dc_motor_value(int v) {

    // 100 et -100,  -100
    auto value = dc_motor_value{};

    int zero_diff = 0;

    if (v < -5) {
        value.active = true;
        value.direction = Direction::FORWARD;
        zero_diff = v * -1;
    } else if (v > 5) {
        value.active = true;
        value.direction = Direction::REVERSE;
        zero_diff = v;
    } else {
        value.active = false;
    }

    value.power = (zero_diff * 255) / 100;

    return value;
}

hbridge_value get_hbridge_value(control_payload const* control_payload) {

    return hbridge_value{
        get_dc_motor_value(control_payload->l_y),
        get_dc_motor_value(control_payload->l_x)
    };
}

// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
    // check for a special command to instruct the server to stop listening so
    // it can be cleanly exited.
    if (msg->get_payload() == "stop-listening") {
        s->stop_listening();
        return;
    }

    const auto delimiter = ",";
    const auto payload = msg->get_payload();

    const auto v = split(payload, delimiter);

    std::cout << payload << std::endl;

    if (v.size() != 4) {
        return;
    }

    std::vector<int> newV;
    newV.reserve( v.size() );    //  avoids unnecessary reallocations
    std::transform( v.begin(), v.end(),
                std::back_inserter( newV ),
                [](std::string str) { return std::atoi(str.c_str()); });

    const auto control_struct = control_payload{newV.at(1), newV.at(0), newV.at(3), newV.at(2)};
    const auto hbridge = get_hbridge_value(&control_struct);

    std::cout << control_struct.l_x << " " << control_struct.l_y << std::endl;

    if (hbridge.motor1.active == true) {

        if (hbridge.motor1.direction == FORWARD) {
            firmata_digitalWrite(firmata, 7, HIGH); //light led
            firmata_digitalWrite(firmata, 8, LOW); //light led
        } else {
            firmata_digitalWrite(firmata, 7, LOW); //light led
            firmata_digitalWrite(firmata, 8, HIGH); //light led
        }
        
    } else {
        firmata_digitalWrite(firmata, 7, LOW); //light led
        firmata_digitalWrite(firmata, 8, LOW); //light led
    }
}

bool on_httpinit(server * s, websocketpp::connection_hdl hdl)
{
    s->get_con_from_hdl(hdl)->append_header("Access-Control-Allow-Origin", "*");	
    return true;
}


int main() {
    try {
        // Create a server endpoint
        server echo_server;


        firmata = firmata_new("/dev/ttyACM0"); //init Firmata

        while(!firmata->isReady) //Wait until device is up
        firmata_pull(firmata);

        firmata_pinMode(firmata, 7, MODE_OUTPUT); //set pin 13 (led on most arduino) to out
        firmata_pinMode(firmata, 8, MODE_OUTPUT); //set pin 13 (led on most arduino) to out

        std::cout << "firmata initialize" << std::endl;

        // Initialize Asio
        echo_server.init_asio();

        echo_server.set_http_handler(bind(&on_httpinit, &echo_server, ::_1));

        // Register our message handler
        echo_server.set_message_handler(bind(&on_message,&echo_server,::_1,::_2));

        // Listen on port 9002
        echo_server.listen(9002);

        // Start the server accept loop
        echo_server.start_accept();

        // Start the ASIO io_service run loop
        echo_server.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}