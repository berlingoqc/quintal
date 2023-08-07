#pragma once

#include <gst/gst.h>

#include <rtc/rtc.hpp>

#include "camera_frame_queue.hpp"

struct CameraStreamer {

public:
	void init(std::shared_ptr<rtc::Track>);
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