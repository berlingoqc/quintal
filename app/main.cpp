#include <iostream>

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/thread/thread.hpp>

#include "camera_streamer.hpp"
#include "camera_analysis.hpp"
#include "udp_sink_server.hpp"
#include "webrtc_server.hpp"


int main()
{

    boost::asio::io_service io_service;
    boost::thread_group threadGroup;

    rtc::WebSocket ws;
    CameraStreamer cameraStreamer;
    CameraAnalysis cameraAnalysis;
    WebRTCServer webRtcServer;

    UDPSinkServer server(io_service, 6000); 

    auto reference_queue = cameraStreamer.getQueue();



    threadGroup.create_thread([&]() {
        cameraAnalysis.init(reference_queue);
    });

    threadGroup.create_thread([&]() {
        cameraStreamer.init();
    });

    ws.onOpen([]() {
        std::cout << "WebSocket open" << std::endl;
    });

    ws.onMessage([&webRtcServer](std::variant<rtc::binary, rtc::string> msg) {
        if (std::holds_alternative<rtc::string>(msg)) {
             nlohmann::json message =  nlohmann::json::parse(std::get<std::string>(msg));


                auto it = message.find("id");
                if (it == message.end())
                    return;
                std::string id = it->get<std::string>();

                it = message.find("type");
                if (it == message.end())
                    return;
                std::string type = it->get<std::string>();

                if (type == "answer") {
                    
                    auto sdp = message["sdp"].get<std::string>();
                    auto description = rtc::Description(sdp, type);

                    webRtcServer.initConnectionWithPeer(description);
                }
        }
    });

    ws.open("wss://signaling.cars.growbe.ca/devices/mycustomid");
    

    boost::function<void(nlohmann::json)> callback = [&ws](nlohmann::json message) {

        ws.send(message.dump());

        return;
    };

    boost::function<void(std::string)> callback_datachannel = [](std::string message) {
        std::cout << "receive input" << std::endl;
        return;
    };

    boost::function<void(std::shared_ptr<rtc::Track>)> callback_track = [&server](std::shared_ptr<rtc::Track> track) {
        std::cout << "receive track" << std::endl;
        server.replaceTrack(track);
        return;
    };


    webRtcServer.init(callback, callback_datachannel, callback_track);



    io_service.run();
    

    //threadGroup.join_all();
}