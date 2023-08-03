#include "rtc/rtc.hpp"

#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

#include <nlohmann/json.hpp>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
typedef int SOCKET;

using nlohmann::json;

const int BUFFER_SIZE = 2048;

template <class T> std::weak_ptr<T> make_weak_ptr(std::shared_ptr<T> ptr) { return ptr; };

int main() {
	try {
		rtc::InitLogger(rtc::LogLevel::Debug);
		auto id = "random_id";
		rtc::Configuration config;
		std::string stunServer = "stun:stun.l.google.com:19302";
    	std::cout << "STUN server is " << stunServer << std::endl;
    	config.iceServers.emplace_back(stunServer);
    	config.disableAutoNegotiation = true;
		auto pc = std::make_shared<rtc::PeerConnection>(config);

		pc->onStateChange(
		    [](rtc::PeerConnection::State state) { std::cout << "State: " << state << std::endl; });

		pc->onGatheringStateChange([pc](rtc::PeerConnection::GatheringState state) {
			std::cout << "Gathering State: " << state << std::endl;
			if (state == rtc::PeerConnection::GatheringState::Complete) {
				auto description = pc->localDescription();
				json message = {{"type", description->typeString()},
				                {"sdp", std::string(description.value())}};
				std::cout << message << std::endl;
			}
		});

		SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
		struct sockaddr_in addr = {};
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_port = htons(6000);

		if (bind(sock, reinterpret_cast<const sockaddr *>(&addr), sizeof(addr)) < 0)
			throw std::runtime_error("Failed to bind UDP socket on 127.0.0.1:6000");

		int rcvBufSize = 212992;
		setsockopt(sock, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char *>(&rcvBufSize),
		           sizeof(rcvBufSize));

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



		std::cout << "RTP video stream expected on localhost:6000" << std::endl;
		std::cout << "Please copy/paste the answer provided by the browser: " << std::endl;
		std::string sdp;
		std::getline(std::cin, sdp);

		json j = json::parse(sdp);
		rtc::Description answer(j["sdp"].get<std::string>(), j["type"].get<std::string>());
		pc->setRemoteDescription(answer);

		// Receive from UDP
		char buffer[BUFFER_SIZE];
		int len;
		while ((len = recv(sock, buffer, BUFFER_SIZE, 0)) >= 0) {
			if (len < sizeof(rtc::RtpHeader) || !track->isOpen())
				continue;

			auto rtp = reinterpret_cast<rtc::RtpHeader *>(buffer);
			rtp->setSsrc(ssrc);

			track->send(reinterpret_cast<const std::byte *>(buffer), len);

		}

	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}