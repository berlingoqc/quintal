
#include "webrtc_server.hpp"


#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>


using nlohmann::json;

const int BUFFER_SIZE = 2048;

void WebRTCServer::init(
	const std::string& id,
	const WebRTCConfig& webRtcConfig,
	const VideoStreamConfig& videoStreamingConfig,
	boost::function<void(json)>callback,
	boost::function<void(rtc::binary)>callback_datachannel,
	boost::function<void(std::shared_ptr<rtc::Track>)>callbackTrack,
	boost::function<void(std::shared_ptr<ProtobufMessageSender>)> callbackSenderReader
) {
	rtc::InitLogger(rtc::LogLevel::Debug);

	this->startPC(id, webRtcConfig, videoStreamingConfig, callback, callback_datachannel, callbackTrack, callbackSenderReader);
}

std::shared_ptr<rtc::Track> WebRTCServer::getTrack() {
	return this->track;
}

std::shared_ptr<ProtobufMessageSender> WebRTCServer::getDCSender() {
	return this->dcProtobufSender;
}

void WebRTCServer::initConnectionWithPeer(rtc::Description description) {
	std::cout << "initConnectionWithPeer : " << std::endl;
	pc->setRemoteDescription(description);
}

void WebRTCServer::startVideoFetching() {

}

void WebRTCServer::stopVideoFetching() {

}

void WebRTCServer::startPC(
	const std::string& id,
	const WebRTCConfig& webRtcConfig,
	const VideoStreamConfig& videoStreamingConfig,
	boost::function<void(json)>callback,
	boost::function<void(rtc::binary)>callback_datachannel,
	boost::function<void(std::shared_ptr<rtc::Track>)>callbackTrack,
	boost::function<void(std::shared_ptr<ProtobufMessageSender>)> callbackSenderReader
) {

	rtc::Configuration config;

	for (const auto iceServer : webRtcConfig.ice_servers()) {
   		config.iceServers.emplace_back(iceServer);
	}

   	config.disableAutoNegotiation = true;

	pc = std::make_shared<rtc::PeerConnection>(config);

	pc->onStateChange(
	    [](rtc::PeerConnection::State state) { std::cout << "State: " << state << std::endl; });

	pc->onGatheringStateChange([wpc = make_weak_ptr(pc), callback, id](rtc::PeerConnection::GatheringState state) {
		std::cout << "Gathering State: " << state << std::endl;
		if (state == rtc::PeerConnection::GatheringState::Complete) {
			if(auto pc = wpc.lock()) {
				auto description = pc->localDescription();
				json message = {
					{"id", id},
					{"type", description->typeString()},
			        {"sdp", std::string(description.value())}
				};
				
				callback(message);
			}
		}
	});

	const rtc::SSRC ssrc = videoStreamingConfig.ssrc();
	rtc::Description::Video media("video");
	media.addH264Codec(videoStreamingConfig.h264_codec()); // Must match the payload type of the external h264 RTP stream
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

	this->dcProtobufSender = std::make_shared<DCProtobufSender>(dc);

	callbackSenderReader(this->dcProtobufSender);

	pc->setLocalDescription();

	pc->onDataChannel([&](std::shared_ptr<rtc::DataChannel> _dc) {
		std::cout << "[Got a DataChannel with label: " << _dc->label() << "]" << std::endl;

		_dc->onClosed(
	 	   [&]() { std::cout << "[DataChannel closed: " << _dc->label() << "]" << std::endl; });

		_dc->onMessage([](auto data) {
			if (std::holds_alternative<std::string>(data)) {
				std::cout << "[Received message: " << std::get<std::string>(data) << "]"
				          << std::endl;
			}
			});
	});

	pc->onStateChange([id, this, webRtcConfig, videoStreamingConfig, callback, callback_datachannel, callbackTrack, callbackSenderReader](rtc::PeerConnection::State state) {
        std::cout << "State: " << state << std::endl;
        if (state == rtc::PeerConnection::State::Closed) {
			try {
				pc->close();
				this->startPC(id, webRtcConfig, videoStreamingConfig, callback, callback_datachannel, callbackTrack, callbackSenderReader);
			} catch (...) {
				std::cout << "error ?" << std::endl;
			}
		} else {
			std::cout << "just elsing " << std::endl;
		}
    });
}