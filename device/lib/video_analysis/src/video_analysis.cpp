
#include "video_analysis.hpp"


void VideoAnalysis::init(CVFrameQueue* data) {
    cv::CascadeClassifier face_cascade;

	if (!face_cascade.load("/opt/haarcascade_frontalface_default.xml")) {
    	std::cerr << "Error loading face cascade\n";
  		return;
	}

    while (true) {
        std::unique_lock<std::mutex> lock(data->mtx);
        data->condVar.wait(lock, [&]{return !data->frameQueue.empty();});
        
        cv::Mat frame = data->frameQueue.front();
        data->frameQueue.pop();
        lock.unlock();

        std::vector<cv::Rect> faces;
  		face_cascade.detectMultiScale(frame, faces);

  		for (size_t i = 0; i < faces.size(); i++) {
			std::cout << faces[i] << std::endl;
  	    }
    }

}