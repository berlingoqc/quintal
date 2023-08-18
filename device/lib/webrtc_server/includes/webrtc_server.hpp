#pragma once

#include <memory>
#include <optional>

#include <nlohmann/json.hpp>

#include <rtc/rtc.hpp>

#include "protobuf_message_sender.hpp"

#include "dc_protobuf_sender.hpp"

#include "config.pb.h"

template <class T> std::weak_ptr<T> make_weak_ptr(std::shared_ptr<T> ptr) { return ptr; };

struct WebRTCServer {

public:
	void init(
		const std::string& id,
		const WebRTCConfig& webRtcConfig,
		const VideoStreamConfig& videoStreamingConfig,
		std::function<void(nlohmann::json)> callbackGathering,
		std::function<void(rtc::binary)> callbackDatachannel,
		std::function<void(std::shared_ptr<rtc::Track>)> callbackTrack,
		std::function<void(std::shared_ptr<ProtobufMessageSender>)> callbackSenderReader
	);

	void initConnectionWithPeer(rtc::Description description);

	void startVideoFetching();

	void stopVideoFetching();

	std::shared_ptr<rtc::Track> getTrack();

	std::shared_ptr<ProtobufMessageSender> getDCSender();

private:
	std::shared_ptr<rtc::PeerConnection> pc;
	std::shared_ptr<rtc::DataChannel> dc;
	std::shared_ptr<rtc::Track> track;

	std::shared_ptr<DCProtobufSender> dcProtobufSender;

	std::optional<rtc::Description> peer;

	void startPC(
		const std::string& id,
		const WebRTCConfig& webRtcConfig,
		const VideoStreamConfig& videoStreamingConfig,
		std::function<void(nlohmann::json)>callback,
		std::function<void(rtc::binary)>callback_datachannel,
		std::function<void(std::shared_ptr<rtc::Track>)> callbackTrack,
		std::function<void(std::shared_ptr<ProtobufMessageSender>)> callbackSenderReader
	);
};