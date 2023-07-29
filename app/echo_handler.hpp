#ifndef WEBSOCKETPP_ECHO_SERVER_HANDLER_HPP
#define WEBSOCKETPP_ECHO_SERVER_HANDLER_HPP

class echo_handler : public server::handler {
    void on_message(connection_ptr con, std::string msg) {
        std::cout << msg << std::endl;
    }
};

#endif // WEBSOCKETPP_ECHO_SERVER_HANDLER_HPP