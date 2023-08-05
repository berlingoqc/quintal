#include <iostream>

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>


#include <rtc/rtc.hpp>

#include <nlohmann/json.hpp>

#include "camera_streamer.hpp"

int main()
{

    boost::asio::io_context io_context;

    boost::thread gstreamer_thread([&]() {
        CameraStreamer cameraStreamer;
        cameraStreamer.init();
    });


    gstreamer_thread.join();

}