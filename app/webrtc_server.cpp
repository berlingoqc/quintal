
#include "webrtc_server.hpp"


#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

#include <nlohmann/json.hpp>

using nlohmann::json;

const int BUFFER_SIZE = 2048;

void WebRTCServer::init() {

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

	pc->onGatheringStateChange([wpc = make_weak_ptr(pc)](rtc::PeerConnection::GatheringState state) {
		std::cout << "Gathering State: " << state << std::endl;
		if (state == rtc::PeerConnection::GatheringState::Complete) {
			if(auto pc = wpc.lock()) {
				auto description = pc->localDescription();
				json message = {{"type", description->typeString()},
			                {"sdp", std::string(description.value())}};
				std::cout << message << std::endl;
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
   	   if (auto dc = wdc.lock()) {
   	      dc->send("Ping");
   		}
   	});

   	dc->onMessage(nullptr, [id, wdc = make_weak_ptr(dc)](std::string msg) {
       	std::cout << "Message from " << id << " received: " << msg << std::endl;
       	if (auto dc = wdc.lock()) {
			std::cout << "ping" << std::endl;
       	    dc->send("Ping");
       	}
	});

	pc->setLocalDescription();

	std::shared_ptr<rtc::DataChannel> dc1;
	pc->onDataChannel([&](std::shared_ptr<rtc::DataChannel> _dc) {
	std::cout << "[Got a DataChannel with label: " << _dc->label() << "]" << std::endl;
	dc1 = _dc;

	dc1->onClosed(
	    [&]() { std::cout << "[DataChannel closed: " << dc->label() << "]" << std::endl; });

	dc1->onMessage([](auto data) {
		if (std::holds_alternative<std::string>(data)) {
			std::cout << "[Received message: " << std::get<std::string>(data) << "]"
			          << std::endl;
		}
		});
	});

}

void WebRTCServer::initConnectionWithPeer(rtc::Description description) {

}

void WebRTCServer::startVideoFetching() {

}

void WebRTCServer::stopVideoFetching() {

}