#include <string>
#include <iostream>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <opencv2/opencv.hpp>

//cv::CascadeClassifier face_cascade;


// The appsink callback function
static GstFlowReturn new_sample (GstElement *sink, gpointer *data) {
    GstSample *sample;


    g_signal_emit_by_name (sink, "pull-sample", &sample);
    if (sample) {
        // Do something with the sample

		GstBuffer *buffer = gst_sample_get_buffer(sample);
  		GstMapInfo map;

  		gst_buffer_map(buffer, &map, GST_MAP_READ);

  		int width = 640;
  		int height = 480;

  		cv::Mat frame(cv::Size(width, height), CV_8UC3, (char*)map.data, cv::Mat::AUTO_STEP);

		// Perform face detection.
  		//std::vector<cv::Rect> faces;
  		//face_cascade.detectMultiScale(frame, faces);

  		//for (size_t i = 0; i < faces.size(); i++) {
		//	std::cout << faces[i] << std::endl;
  		//}

  		gst_buffer_unmap(buffer, &map);
        gst_sample_unref (sample);
        return GST_FLOW_OK;
    }

    return GST_FLOW_ERROR;
}

int main(int argc, char *argv[]) {


	//if (!face_cascade.load("/opt/haarcascade_frontalface_default.xml")) {
  	//	std::cerr << "Error loading face cascade\n";
  	//	return -1;
	//}


    GstElement *pipeline, *sink;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    GError *error = NULL;

    gst_init (&argc, &argv);

    std::string gstreamer_pipeline = "v4l2src device=/dev/video0 ! videoconvert ! video/x-raw,width=640,height=480,format=BGR ! tee name=t "
                                     "t. ! queue ! videoconvert ! x264enc tune=zerolatency bitrate=1000 key-int-max=30 ! video/x-h264, profile=constrained-baseline ! rtph264pay pt=96 mtu=1200 ! udpsink host=127.0.0.1 port=6000 "
                                     "t. ! queue ! appsink name=appsink emit-signals=true ";

    pipeline = gst_parse_launch(gstreamer_pipeline.c_str(), &error);

    if (error != NULL) {
        g_printerr("Could not construct pipeline: %s\n", error->message);
        g_clear_error (&error);
        return -1;
    }

    sink = gst_bin_get_by_name(GST_BIN(pipeline), "appsink");
    g_signal_connect (sink, "new-sample", G_CALLBACK (new_sample), NULL);

    ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (pipeline);
        return -1;
    }

    bus = gst_element_get_bus (pipeline);
    msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

    if (msg != NULL)
        gst_message_unref (msg);
    gst_object_unref (bus);
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);
    return 0;
}