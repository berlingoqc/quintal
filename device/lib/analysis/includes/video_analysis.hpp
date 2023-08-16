#pragma once

#include "video_queue.hpp"
#include "protobuf_message_sender.hpp"

struct VideoAnalysis
{

public:
    void init(CVFrameQueue* CVFrameQueue);
};
