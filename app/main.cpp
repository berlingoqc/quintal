#include <iostream>

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/thread/thread.hpp>

#include "camera_streamer.hpp"
#include "camera_analysis.hpp"
#include "ws_client.hpp"

#include "webrtc_server.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

typedef int SOCKET;
const int BUFFER_SIZE = 2048;



using boost::asio::ip::udp;


class UDPServer {
public:
    UDPServer(boost::asio::io_service& io_service, short port, std::shared_ptr<rtc::Track> track)
        : socket_(io_service, udp::endpoint(udp::v4(), port)), track_(track) {
        startReceive();
    }

private:
    void startReceive() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_transferred) {
                handleReceive(ec, bytes_transferred);
            });
    }

    void handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred) {
        if (!error) {
            if (bytes_transferred < sizeof(rtc::RtpHeader)) {
                std::cout << "to short" << std::endl;
                startReceive();
				return;
            }
            
            auto rtp = reinterpret_cast<rtc::RtpHeader *>(recv_buffer_.data());
			rtp->setSsrc(42);


            if (track_->isOpen()) {
			    track_->send(reinterpret_cast<const std::byte *>(recv_buffer_.data()), bytes_transferred);
            }

            startReceive();  // Start another receive operation
        } else {
            std::cerr << "Error during receive: " << error.message() << std::endl;
        }
    }

    std::shared_ptr<rtc::Track> track_;
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, 1024> recv_buffer_;
};


int main()
{
    boost::thread_group threadGroup;

    WebSocketClient wsClient;
    CameraStreamer cameraStreamer;
    CameraAnalysis cameraAnalysis;
    WebRTCServer webRtcServer;

    auto reference_queue = cameraStreamer.getQueue();



    threadGroup.create_thread([&]() {
        cameraAnalysis.init(reference_queue);
    });

    wsClient.connect("localhost", "8000", "/mycustomid");


    
    threadGroup.create_thread(
        [&wsClient, &webRtcServer]()
        {
            bool receive_msg = false;
            while(!receive_msg) {
                std::string msg = wsClient.receive();

                nlohmann::json message = nlohmann::json::parse(msg);


                auto it = message.find("id");
                if (it == message.end())
                    continue;
                std::string id = it->get<std::string>();

                it = message.find("type");
                if (it == message.end())
                    continue;
                std::string type = it->get<std::string>();

                if (type == "answer") {
                    
                    auto sdp = message["sdp"].get<std::string>();
                    auto description = rtc::Description(sdp, type);

                    webRtcServer.initConnectionWithPeer(description);
                }


                receive_msg = true;
            }

            std::cout << "end of websocket task" << std::endl;
        });


    boost::function<void(nlohmann::json)> callback = [&wsClient](nlohmann::json message) {

        wsClient.send(message.dump());

        return;
    };

    boost::function<void(std::string)> callback_datachannel = [](std::string message) {
        std::cout << "receive input" << std::endl;
        return;
    };

    webRtcServer.init(callback, callback_datachannel);

    auto track = webRtcServer.getTrack();


    threadGroup.create_thread([&]() {
        cameraStreamer.init(track);
    });

    boost::asio::io_service io_service;
    UDPServer server(io_service, 6000, track);  // Listen on port 12345

    io_service.run();

    //threadGroup.join_all();
}