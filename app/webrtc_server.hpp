#pragma once

#include <memory>
#include <optional>

#include <boost/thread/thread.hpp>

#include <rtc/rtc.hpp>

template <class T> std::weak_ptr<T> make_weak_ptr(std::shared_ptr<T> ptr) { return ptr; };

struct WebRTCServer {

public:
	void init();

	void initConnectionWithPeer(rtc::Description description);

	void startVideoFetching();

	void stopVideoFetching();

private:
	std::shared_ptr<rtc::PeerConnection> pc;

	std::optional<rtc::Description> peer;
};