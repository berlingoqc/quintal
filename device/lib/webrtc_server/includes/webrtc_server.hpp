#pragma once

#include <memory>
#include <optional>

#include <boost/thread/thread.hpp>
#include <boost/function.hpp>

#include <nlohmann/json.hpp>

#include <rtc/rtc.hpp>

#include "config.pb.h"

template <class T> std::weak_ptr<T> make_weak_ptr(std::shared_ptr<T> ptr) { return ptr; };

struct WebRTCServer {

public:
	void init(
		const std::string& id,
		const WebRTCConfig& webRtcConfig,
		const VideoStreamConfig& videoStreamingConfig,
		boost::function<void(nlohmann::json)> callbackGathering,
		boost::function<void(rtc::binary)> callbackDatachannel,
		boost::function<void(std::shared_ptr<rtc::Track>)> callbackTrack
	);

	void initConnectionWithPeer(rtc::Description description);

	void startVideoFetching();

	void stopVideoFetching();

	std::shared_ptr<rtc::Track> getTrack();

private:
	std::shared_ptr<rtc::PeerConnection> pc;
	std::shared_ptr<rtc::DataChannel> dc;
	std::shared_ptr<rtc::Track> track;

	std::optional<rtc::Description> peer;


	void startPC(
		const std::string& id,
		const WebRTCConfig& webRtcConfig,
		const VideoStreamConfig& videoStreamingConfig,
		boost::function<void(nlohmann::json)>callback,
		boost::function<void(rtc::binary)>callback_datachannel,
		boost::function<void(std::shared_ptr<rtc::Track>)> callbackTrack
	);
};