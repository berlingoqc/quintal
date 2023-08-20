#include "camera_streamer.hpp"

struct SampleCallbackData {

public:
    int32_t width;
    int32_t height;

    CVFrameQueue* queue;
};

static GstFlowReturn new_sample (GstElement *sink, SampleCallbackData* data) {
    GstSample *sample;

    g_signal_emit_by_name (sink, "pull-sample", &sample);
    if (sample) {

		GstBuffer *buffer = gst_sample_get_buffer(sample);
  		GstMapInfo map;

  		gst_buffer_map(buffer, &map, GST_MAP_READ);

  		int width = data->width;
  		int height = data->height;

        std::unique_lock<std::mutex> lock(data->queue->mtx);

  		cv::Mat frame(cv::Size(width, height), CV_8UC3, (char*)map.data, cv::Mat::AUTO_STEP);

        data->queue->frameQueue.push(frame);
        lock.unlock();
        data->queue->condVar.notify_one();

  		gst_buffer_unmap(buffer, &map);
        gst_sample_unref (sample);
        return GST_FLOW_OK;
    }

    return GST_FLOW_ERROR;
}

void CameraStreamer::init(
    const VideoStreamConfig& streamConfig
) {
    SampleCallbackData callbackData;
    callbackData.queue = &this->queue;
    callbackData.height = streamConfig.height();
    callbackData.width = streamConfig.width();
	gst_init (nullptr, nullptr);

    std::string source_camera = streamConfig.gstreamer_source();

    std::stringstream ss;


    ss << source_camera << " ";

    if (streamConfig.custom_pipeline() == "") {
        ss  << "! videoconvert ! video/x-raw,width=" << streamConfig.width() << ",height=" << streamConfig.height() << ",format=" << streamConfig.format() << " ! tee name=t ";
        ss << "t. ! queue ! videoconvert ! x264enc tune= " << streamConfig.h264_enc_tune() << " bitrate=" << streamConfig.bitrate() << " key-int-max=" << streamConfig.keyintmax();
        ss << " ! video/x-h264, profile=" << streamConfig.h264_profile() << " ! rtph264pay pt=" << streamConfig.h264_codec() << " mtu=" << streamConfig.mtu() << " ! udpsink host=" << streamConfig.udp_host() << " port=" << streamConfig.udp_port()<< " ";
        ss << " t. ! queue ! appsink name=appsink emit-signals=true ";
    } else {
        ss << streamConfig.custom_pipeline();
    }

    std::cout << ss.str() << std::endl;

    pipeline = gst_parse_launch(ss.str().c_str(), &error);

    if (error != NULL) {
        g_printerr("Could not construct pipeline: %s\n", error->message);
        g_clear_error (&error);
        return;
    }

    //sink = gst_bin_get_by_name(GST_BIN(pipeline), "appsink");
    //g_signal_connect (sink, "new-sample", G_CALLBACK (new_sample), &callbackData);


    ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (pipeline);
		return;
    }


    std::cout << "starting streaming pipeline" << std::endl;

    // Main loop (optional, if you need one)
    //GMainLoop *loop = g_main_loop_new(nullptr, FALSE);
    //g_main_loop_run(loop);

    // Clean up
    //gst_element_set_state(pipeline, GST_STATE_NULL);
    //gst_object_unref(pipeline);
    //g_main_loop_unref(loop);

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