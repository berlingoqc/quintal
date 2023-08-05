#include <iostream>

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include "camera_streamer.hpp"
#include "camera_analysis.hpp"


int main()
{
    boost::thread_group threadGroup;
    boost::asio::io_context io_context;

    CameraStreamer cameraStreamer;
    CameraAnalysis cameraAnalysis;
    auto reference_queue = cameraStreamer.getQueue();

    threadGroup.create_thread([&]() {
        cameraStreamer.init();
    });

    threadGroup.create_thread([&]() {
        cameraAnalysis.init(reference_queue);
    });


    threadGroup.join_all();
}