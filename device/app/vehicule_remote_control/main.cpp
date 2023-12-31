// STD
#include <iostream>
#include <map>
#include <thread>
//#include <format>

// ASIO
#include <asio.hpp>

// ANALYSIS
//#include "video_analysis.hpp"

// WEBRTC_SERVER
#include "camera_streamer.hpp"
#include "udp_sink_server.hpp"
#include "webrtc_server.hpp"

// FIRMATA
#include "firmata_client.hpp"

// VEHICULEE
#include "car_builder.hpp"
#include "car.hpp"
#include "iot_control.hpp"
#include "motor.hpp"

// UTILS
#include "file_manager.hpp"

// QUINTAL_PROTO
#include "msg_header.pb.h"
#include "control.pb.h"
#include "config.pb.h"

// PROTOBUF
#include <google/protobuf/util/json_util.h>


void loadConfig(const char* path, Config* config) {
    try {
        auto content = FileManager::loadFile(path);

        google::protobuf::util::JsonStringToMessage(content, config);
    } catch (const std::exception& ex) {
        // No file , create one instead

        config->set_id("mycustomid");

        auto firmataConfig = new FirmataConfig();
        firmataConfig->set_device_name("/dev/ttyACM0");

        IoTControlConfig iotControl;
        iotControl.set_type(IoTControlType::FIRMATA);
        iotControl.set_allocated_firmata(firmataConfig);

        (*config->mutable_controls())["firmata"] = iotControl;

        auto car = new CarConfig();

        auto steering = new MotorConfig();
        auto throtle = new MotorConfig();

        auto steering_dc = new DCMotorConfig();
        auto throtle_dc = new DCMotorConfig();
        steering_dc->set_int1(5);
        steering_dc->set_int2(6);
        steering_dc->set_power(11);

        throtle_dc->set_int1(3);
        throtle_dc->set_int2(4);
        throtle_dc->set_power(10);

        steering->set_iot_control("firmata");
        throtle->set_iot_control("firmata");

        steering->set_allocated_dc(steering_dc);
        throtle->set_allocated_dc(throtle_dc);

        car->set_allocated_steering(steering);
        car->set_allocated_throtle(throtle);

        config->set_allocated_car(car);


        auto signalingConfig = new SignalingConfig();
        signalingConfig->set_url("wss://signaling.cars.growbe.ca");

        config->set_allocated_signaling_config(signalingConfig);

        auto rtcConfig = new WebRTCConfig();
        rtcConfig->add_ice_servers("stun:stun.l.google.com:19302");
	    rtcConfig->add_ice_servers("turn:username1:key1@turn.cars.growbe.ca:3478");

        config->set_allocated_web_rtc(rtcConfig);

        auto videoStreamConfig = new VideoStreamConfig();
        videoStreamConfig->set_gstreamer_source("v4l2src device=/dev/video0");
        videoStreamConfig->set_ssrc(42);
        videoStreamConfig->set_h264_codec(96);
        videoStreamConfig->set_width(640);
        videoStreamConfig->set_height(480);
        videoStreamConfig->set_format("BGR");
        videoStreamConfig->set_bitrate(1000);
        videoStreamConfig->set_keyintmax(30);
        videoStreamConfig->set_h264_enc_tune("zerolatency");
        videoStreamConfig->set_h264_profile("constrained-baseline");
        videoStreamConfig->set_mtu(1000);
        videoStreamConfig->set_udp_host("127.0.0.1");
        videoStreamConfig->set_udp_port(6000);

        config->set_allocated_video_stream(videoStreamConfig);
        
        std::cout << "fallback to default config" << std::endl;

        std::string json_string;
        google::protobuf::util::JsonPrintOptions options;
        options.add_whitespace = true;
        options.always_print_primitive_fields = true;
        google::protobuf::util::MessageToJsonString(*config, &json_string, options);

        FileManager::writeFile(path, json_string);
    }

}

void cameraStreamThread(std::shared_ptr<CameraStreamer> cameraStreamer, VideoStreamConfig videoStreamConfig) {
    cameraStreamer->init(videoStreamConfig);
}

int main()
{

    Config config;

    loadConfig("./config.json", &config);

    std::cout << "starting car with ID " << config.id() << std::endl;

    auto controls = CarBuilder::getControl(config.controls());

    CarBuilder carBuilder(controls);
    
    auto car = carBuilder.buildCar(config.car());

    asio::io_service io_service;

    std::vector<std::thread> threads;

    // maybe bundle all of those logic together
    rtc::WebSocket ws;
    auto cameraStreamer = std::make_shared<CameraStreamer>();
    WebRTCServer webRtcServer;
    UDPSinkServer server(io_service, config.video_stream().udp_port()); 

    auto reference_queue = cameraStreamer->getQueue();

    //VideoAnalysis cameraAnalysis(reference_queue);

    //threadGroup.create_thread([&]() {
    //    cameraAnalysis.init();
    //});

    auto threadCameraStream = std::thread(cameraStreamThread, cameraStreamer, config.video_stream());

    //threadCameraStream.detach();

        //std::thread([]() {
            //cameraStreamer.init(config.video_stream());
        //});

    ws.onOpen([]() {
        std::cout << "WebSocket open" << std::endl;
    });

    ws.onMessage([&webRtcServer](std::variant<rtc::binary, rtc::string> msg) {

        std::cout << "receive message from websocket " << std::endl;
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

    std::stringstream ss;

    ss << config.signaling_config().url() << "/devices/" << config.id();

    ws.open(ss.str());
    

    std::function<void(nlohmann::json)> callback = [&ws](nlohmann::json message) {

        std::cout << "sending payload " << std::endl;
        ws.send(message.dump());

        return;
    };

    std::function<void(rtc::binary)> callback_datachannel = [&car](rtc::binary message) {

        auto header = static_cast<MsgHeader>(message.at(0));

        switch (header) {
            case CONTROL_EVENT:
                {
                    ControlEvent controlEvent;
                    bool success = controlEvent.ParseFromArray(message.data() + 1, message.size() - 1);

                    std::cout << "receive control payload " << controlEvent.y() << " " << controlEvent.x() << std::endl;

                    if (success) {
                        if (controlEvent.y() > 0) {
                            car->forward(255);
                        } else if (controlEvent.y() < 0) {
                            car->backward(255);
                        } else {
                            car->stopThrotle();
                        }

                        if (controlEvent.x() > 0) {
                            car->steerLeft(255);
                        } else if (controlEvent.x() < 0) {
                            car->steerRight(255);
                        } else {
                            car->stopSteer();
                        }

                    } else {
                        std::cerr << "failed to parse a CONTROL_EVENT payload" << std::endl;
                    }
                }
                break;;
            default:
                break;;
        }

        return;
    };

    std::function<void(std::shared_ptr<rtc::Track>)> callback_track = [&server](std::shared_ptr<rtc::Track> track) {
        std::cout << "receive track" << std::endl;
        server.replaceTrack(track);
        return;
    };

    std::function<void(std::shared_ptr<ProtobufMessageSender>)> callback_sender = [&server](std::shared_ptr<ProtobufMessageSender> track) {
        std::cout << "receive protobuf sender" << std::endl;

        //cameraAnalysis.replaceProtobufSender(track);

        return;
    };


    webRtcServer.init(
        config.id(), config.web_rtc(),
        config.video_stream(),
        callback, callback_datachannel,
        callback_track,
        callback_sender
    );

    io_service.run();
}
