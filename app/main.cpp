#include <iostream>

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/thread/thread.hpp>

#include "camera_streamer.hpp"
#include "camera_analysis.hpp"
#include "ws_client.hpp"

#include "webrtc_server.hpp"



int main()
{
    boost::thread_group threadGroup;

    WebSocketClient wsClient;
    CameraStreamer cameraStreamer;
    CameraAnalysis cameraAnalysis;
    WebRTCServer webRtcServer;

    auto reference_queue = cameraStreamer.getQueue();

    threadGroup.create_thread([&]() {
        cameraStreamer.init();
    });

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

    wsClient.run();
    
    threadGroup.join_all();
}