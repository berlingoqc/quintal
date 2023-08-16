#include "dc_protobuf_sender.hpp"


DCProtobufSender::DCProtobufSender(std::shared_ptr<rtc::DataChannel> channel): channel_(channel) {

}

void DCProtobufSender::sendPayload(MsgHeader msg_header, const google::protobuf::Message& msg) {
    if (!this->channel_->isOpen()) return;

    auto data = this->getBuffer(msg_header, msg);

    bool isSend = this->channel_->send(reinterpret_cast<const std::byte*>(data.data()), data.size());
}