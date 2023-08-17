#pragma once

#include <vector>

#include "msg_header.pb.h"

#include <google/protobuf/message.h>

class ProtobufMessageSender {

public:
    virtual void sendPayload(MsgHeader msg_header, const google::protobuf::Message& msg) = 0;

    std::vector<uint8_t> getBuffer(MsgHeader& msg_header, const google::protobuf::Message& msg) {
        std::vector<uint8_t> buffer;
        std::string binary_data;

        if (!msg.SerializeToString(&binary_data)) {
            // Handle serialization error
            return buffer;
        }

        buffer.reserve(binary_data.size() + 1); // +1 for the header byte

        buffer.push_back(msg_header);
        buffer.insert(buffer.end(), binary_data.begin(), binary_data.end());


        return buffer;
    }
};
