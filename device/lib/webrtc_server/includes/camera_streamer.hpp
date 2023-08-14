#pragma once

#include <gst/gst.h>

#include "camera_frame_queue.hpp"
#include "config.pb.h"

struct CameraStreamer {

public:
	void init(const VideoStreamConfig& streamConfig);
	CVFrameQueue* getQueue();

private:
	GstElement* pipeline;
	GstElement* sink;
	GstElement* sinkH264;
	GstBus *bus;
	GstMessage *msg;
	GstStateChangeReturn ret;
	GError *error = NULL;

	CVFrameQueue queue;
};