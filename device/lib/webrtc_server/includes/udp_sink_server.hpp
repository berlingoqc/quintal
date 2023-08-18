
#include <mutex>
#include <array>

#include <asio.hpp>

#include <rtc/rtc.hpp>


using asio::ip::udp;

const int BUFFER_SIZE = 2048;


class UDPSinkServer {
public:
    UDPSinkServer(asio::io_service& io_service, short port)
        : socket_(io_service, udp::endpoint(udp::v4(), port)), track_(nullptr) {
        startReceive();
    }

    void replaceTrack(std::shared_ptr<rtc::Track> track) {
        this->mtx_.lock();
        this->track_ = track;
        this->mtx_.unlock();

    }


private:
    void startReceive() {
        socket_.async_receive_from(
            asio::buffer(recv_buffer_), remote_endpoint_,
            [this](asio::error_code ec, std::size_t bytes_transferred) {
                handleReceive(ec, bytes_transferred);
            });
    }

    void handleReceive(const asio::error_code& error, std::size_t bytes_transferred) {
        if (!error) {
            if (bytes_transferred < sizeof(rtc::RtpHeader)) {
                std::cout << "to short" << std::endl;
                startReceive();
				return;
            }
            
            auto rtp = reinterpret_cast<rtc::RtpHeader *>(recv_buffer_.data());
			rtp->setSsrc(42);


            mtx_.lock();
            if (track_ == nullptr) {
                startReceive();
            } else if (track_->isOpen()) {
			    track_->send(reinterpret_cast<const std::byte *>(recv_buffer_.data()), bytes_transferred);
            }
            mtx_.unlock();

            startReceive();  // Start another receive operation
        } else {
            std::cerr << "Error during receive: " << error.message() << std::endl;
        }
    }


    std::mutex mtx_;
    std::shared_ptr<rtc::Track> track_;
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, BUFFER_SIZE> recv_buffer_;
};
