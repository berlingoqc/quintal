
#include "webrtc_server.hpp"


#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>


using nlohmann::json;

const int BUFFER_SIZE = 2048;

void WebRTCServer::init(
	boost::function<void(json)>callback,
	boost::function<void(std::string)>callback_datachannel
) {

	rtc::InitLogger(rtc::LogLevel::Debug);

	auto id = "random_id";
	rtc::Configuration config;
	std::string stunServer = "stun:stun.l.google.com:19302";
   	std::cout << "STUN server is " << stunServer << std::endl;
   	config.iceServers.emplace_back(stunServer);
   	config.disableAutoNegotiation = true;
	

	pc = std::make_shared<rtc::PeerConnection>(config);

	pc->onStateChange(
	    [](rtc::PeerConnection::State state) { std::cout << "State: " << state << std::endl; });

	pc->onGatheringStateChange([wpc = make_weak_ptr(pc), callback](rtc::PeerConnection::GatheringState state) {
		std::cout << "Gathering State: " << state << std::endl;
		if (state == rtc::PeerConnection::GatheringState::Complete) {
			if(auto pc = wpc.lock()) {
				auto description = pc->localDescription();
				json message = {
					{"id", "peerid"},
					{"type", description->typeString()},
			        {"sdp", std::string(description.value())}
				};
				
				callback(message);
			}
		}
	});

	const rtc::SSRC ssrc = 42;
	rtc::Description::Video media("video");
	media.addH264Codec(96); // Must match the payload type of the external h264 RTP stream
	media.addSSRC(ssrc, "video-send");
	auto track = pc->addTrack(media);


    auto dc = pc->createDataChannel("ping-pong");
    dc->onOpen([id, wdc = make_weak_ptr(dc)]() {
		std::cout << "datachannel is open" << std::endl;
    });

    dc->onMessage(nullptr, [id, wdc = make_weak_ptr(dc), callback_datachannel](std::string msg) {
        std::cout << "Message from " << id << " received: " << msg << std::endl;

		callback_datachannel(msg);
    });

	this->dc = dc;

	pc->setLocalDescription();

	pc->onDataChannel([&](std::shared_ptr<rtc::DataChannel> _dc) {
		std::cout << "[Got a DataChannel with label: " << _dc->label() << "]" << std::endl;
		//dc1 = _dc;

		_dc->onClosed(
	 	   [&]() { std::cout << "[DataChannel closed: " << _dc->label() << "]" << std::endl; });

		_dc->onMessage([](auto data) {
			if (std::holds_alternative<std::string>(data)) {
				std::cout << "[Received message: " << std::get<std::string>(data) << "]"
				          << std::endl;
			}
			});
	});

}

void WebRTCServer::initConnectionWithPeer(rtc::Description description) {
	pc->setRemoteDescription(description);
}

void WebRTCServer::startVideoFetching() {

}

void WebRTCServer::stopVideoFetching() {

}