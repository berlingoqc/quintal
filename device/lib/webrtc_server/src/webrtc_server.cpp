
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
	boost::function<void(rtc::binary)>callback_datachannel,
	boost::function<void(std::shared_ptr<rtc::Track>)>callbackTrack
) {
	rtc::InitLogger(rtc::LogLevel::Debug);

	this->startPC(callback, callback_datachannel, callbackTrack);
}

std::shared_ptr<rtc::Track> WebRTCServer::getTrack() {
	return this->track;
}

void WebRTCServer::initConnectionWithPeer(rtc::Description description) {
	pc->setRemoteDescription(description);
}

void WebRTCServer::startVideoFetching() {

}

void WebRTCServer::stopVideoFetching() {

}

void WebRTCServer::startPC(
	boost::function<void(json)>callback,
	boost::function<void(rtc::binary)>callback_datachannel,
	boost::function<void(std::shared_ptr<rtc::Track>)>callbackTrack
) {

	auto id = "random_id";

	rtc::Configuration config;

	std::string stunServer = "stun:stun.relay.metered.ca:80";
	std::string turnServer = "turn:1d5df4140ad2e51e190d2cc4:BggnkQ7ytzN7zepr@a.relay.metered.ca:80";
   	std::cout << "STUN server is " << stunServer << std::endl;

   	config.iceServers.emplace_back(stunServer);
	config.iceServers.emplace_back(turnServer);
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
					{"id", "mycustomid"},
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

	this->track = track;

	callbackTrack(this->track);

    auto dc = pc->createDataChannel("ping-pong");
    dc->onOpen([id, wdc = make_weak_ptr(dc)]() {
		std::cout << "datachannel is open" << std::endl;
    });

    dc->onMessage([id, wdc = make_weak_ptr(dc), callback_datachannel](rtc::binary msg) {
		callback_datachannel(msg);
    }, [](rtc::string msg) {
		std::cout << "string msg " << msg << std::endl;
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

	pc->onStateChange([id, this, callback, callback_datachannel, callbackTrack](rtc::PeerConnection::State state) {
        std::cout << "State: " << state << std::endl;
        if (state == rtc::PeerConnection::State::Disconnected ||
            state == rtc::PeerConnection::State::Failed ||
            state == rtc::PeerConnection::State::Closed) {
			pc->close();
			this->startPC(callback, callback_datachannel, callbackTrack);
        }
    });
}