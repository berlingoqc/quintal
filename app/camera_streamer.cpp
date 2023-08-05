#include "camera_streamer.hpp"

static GstFlowReturn new_sample (GstElement *sink, CVFrameQueue* data) {
    GstSample *sample;


    g_signal_emit_by_name (sink, "pull-sample", &sample);
    if (sample) {
        // Do something with the sample

		GstBuffer *buffer = gst_sample_get_buffer(sample);
  		GstMapInfo map;

  		gst_buffer_map(buffer, &map, GST_MAP_READ);

  		int width = 640;
  		int height = 480;

        std::unique_lock<std::mutex> lock(data->mtx);

  		cv::Mat frame(cv::Size(width, height), CV_8UC3, (char*)map.data, cv::Mat::AUTO_STEP);

        data->frameQueue.push(frame);
        lock.unlock();
        data->condVar.notify_one();

  		gst_buffer_unmap(buffer, &map);
        gst_sample_unref (sample);
        return GST_FLOW_OK;
    }

    return GST_FLOW_ERROR;
}

void CameraStreamer::init() {
	gst_init (nullptr, nullptr);

    #ifdef __APPLE__
        std::string source_camera = "avfvideosrc device-index=0";
    #elif __linux__
        std::string source_camera = "v4l2src device=/dev/video0";
    #else
        throw std::runtime_error("Unknown system for gstreamer video source");
    #endif


    std::string gstreamer_pipeline = source_camera + " ! videoconvert ! video/x-raw,width=640,height=480,format=BGR ! tee name=t "
                                     "t. ! queue ! videoconvert ! x264enc tune=zerolatency bitrate=1000 key-int-max=30 ! video/x-h264, profile=constrained-baseline ! rtph264pay pt=96 mtu=1200 ! udpsink host=127.0.0.1 port=6000 "
                                     "t. ! queue ! appsink name=appsink emit-signals=true ";

    pipeline = gst_parse_launch(gstreamer_pipeline.c_str(), &error);

    if (error != NULL) {
        g_printerr("Could not construct pipeline: %s\n", error->message);
        g_clear_error (&error);
        return;
    }

    sink = gst_bin_get_by_name(GST_BIN(pipeline), "appsink");
    g_signal_connect (sink, "new-sample", G_CALLBACK (new_sample), getQueue());

    ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (pipeline);
		return;
    }

    std::cout << "starting streaming pipeline" << std::endl;

	bus = gst_element_get_bus (pipeline);
    msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

    if (msg != NULL)
        gst_message_unref (msg);

    gst_object_unref (bus);
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);

	return;
}

CVFrameQueue* CameraStreamer::getQueue() {
    return &queue;
}