#include "camera_streamer.hpp"

#include <boost/format.hpp>

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

/*
const rtc::SSRC ssrc = 42;
static GstFlowReturn new_sample_2 (GstElement *sink, rtc::Track* track) {
    GstSample *sample;


    g_signal_emit_by_name (sink, "pull-sample", &sample);
    if (sample) {
        // Do something with the sample
		GstBuffer *buffer = gst_sample_get_buffer(sample);
  		GstMapInfo map;

  		gst_buffer_map(buffer, &map, GST_MAP_READ);
        gsize size = gst_buffer_get_size(buffer);

		auto rtp = reinterpret_cast<rtc::RtpHeader *>(buffer);
        rtp->setSsrc(ssrc);

        //std::cout << size << std::endl;

        if (track->isOpen()) {
            GstBuffer *copy_buffer = gst_buffer_copy(buffer);
			auto buff = reinterpret_cast<const std::byte *>(copy_buffer);

            track->send(buff, size);
            
        }

  		gst_buffer_unmap(buffer, &map);
        gst_sample_unref (sample);
        return GST_FLOW_OK;
    }

    return GST_FLOW_ERROR;
}
*/

void CameraStreamer::init(
    const VideoStreamConfig& streamConfig
) {
    SampleCallbackData callbackData;
    callbackData.queue = &this->queue;
    callbackData.height = streamConfig.height();
    callbackData.width = streamConfig.width();
	gst_init (nullptr, nullptr);

    std::string source_camera = streamConfig.gstreamer_source();

    std::string gstreamer_pipeline = source_camera + 
        (boost::format(" ! videoconvert ! video/x-raw,width=%1%,height=%2%,format=%3% ! tee name=t ") % streamConfig.width() % streamConfig.height() % streamConfig.format()).str() +
        (boost::format("t. ! queue ! videoconvert ! x264enc tune=%1% bitrate=%2% key-int-max=%3% ! video/x-h264, profile=%4% ! rtph264pay pt=%5% mtu=%6% ! udpsink host=%7% port=%8% ") %
            streamConfig.h264_enc_tune() % streamConfig.bitrate() % streamConfig.keyintmax() % streamConfig.h264_profile() % streamConfig.h264_codec() % streamConfig.mtu() % streamConfig.udp_host() % streamConfig.udp_port()).str() +
        (boost::format("t. ! queue ! appsink name=appsink emit-signals=true ")).str();

    pipeline = gst_parse_launch(gstreamer_pipeline.c_str(), &error);

    if (error != NULL) {
        g_printerr("Could not construct pipeline: %s\n", error->message);
        g_clear_error (&error);
        return;
    }

    sink = gst_bin_get_by_name(GST_BIN(pipeline), "appsink");
    g_signal_connect (sink, "new-sample", G_CALLBACK (new_sample), &callbackData);


/*
    sinkH264 = gst_bin_get_by_name(GST_BIN(pipeline), "h264sink");
    g_signal_connect (sinkH264, "new-sample", G_CALLBACK(new_sample_2), track.get());
    */

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