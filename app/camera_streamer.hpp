
#include <gst/gst.h>

struct CameraStreamer {

public:
	void init();

private:
	GstElement* pipeline;
	GstElement* sink;
	GstBus *bus;
	GstMessage *msg;
	GstStateChangeReturn ret;
	GError *error = NULL;

};