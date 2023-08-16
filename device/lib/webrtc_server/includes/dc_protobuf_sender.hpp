#pragma once

#include <rtc/rtc.hpp>

#include "protobuf_message_sender.hpp"

class DCProtobufSender : public ProtobufMessageSender {

public:
    DCProtobufSender(std::shared_ptr<rtc::DataChannel> channel);

    void sendPayload(MsgHeader msg_header, const google::protobuf::Message& msg);

private:

    std::shared_ptr<rtc::DataChannel> channel_;
};