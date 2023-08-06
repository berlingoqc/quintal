#include <iostream>

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include "camera_streamer.hpp"
#include "camera_analysis.hpp"
#include "ws_client.hpp"

//#include "webrtc_server.hpp"


int main()
{
    boost::thread_group threadGroup;

    WebSocketClient wsClient;
    CameraStreamer cameraStreamer;
    CameraAnalysis cameraAnalysis;
    //WebRTCServer webRtcServer;
    auto reference_queue = cameraStreamer.getQueue();

    threadGroup.create_thread([&]() {
        cameraStreamer.init();
    });

    threadGroup.create_thread([&]() {
        cameraAnalysis.init(reference_queue);
    });

    wsClient.connect("localhost", "8000", "/mycustomid");

    wsClient.send("{\"id\":\"supper_id\"}");

    bool foundPeer = false;
    while(!foundPeer) {
        std::cout << wsClient.receive() << std::endl;
    }

    //webRtcServer.init();

    threadGroup.join_all();
}