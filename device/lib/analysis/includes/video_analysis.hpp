#pragma once

#include "video_queue.hpp"
#include "protobuf_message_sender.hpp"

struct VideoAnalysis
{

public:
    VideoAnalysis(
        CVFrameQueue* CVFrameQueue
    );

    void init();

    void replaceProtobufSender(std::shared_ptr<ProtobufMessageSender> protobufMessageSender);

private:
    CVFrameQueue* cvFrameQueue_;
    std::shared_ptr<ProtobufMessageSender> protobufMessageSender_;
};
