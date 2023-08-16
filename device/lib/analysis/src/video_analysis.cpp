
#include "video_analysis.hpp"

#include "video_analysis.pb.h"


VideoAnalysis::VideoAnalysis(
    CVFrameQueue* data
) : cvFrameQueue_(data), protobufMessageSender_(nullptr) {}

void VideoAnalysis::replaceProtobufSender(std::shared_ptr<ProtobufMessageSender> protobufMessageSender) {
    this->protobufMessageSender_ = protobufMessageSender;
}

void VideoAnalysis::init() {
    cv::CascadeClassifier face_cascade;

	if (!face_cascade.load("/opt/haarcascade_frontalface_default.xml")) {
    	std::cerr << "Error loading face cascade\n";
  		return;
	}
    
    MsgHeader msgHeader = MsgHeader::VIDEO_ANALYSIS_FACES;
    FacesRecognitionEvent facesRecognitionEvent;

    while (true) {
        std::unique_lock<std::mutex> lock(this->cvFrameQueue_->mtx);
        this->cvFrameQueue_->condVar.wait(lock, [&]{return !this->cvFrameQueue_->frameQueue.empty();});
        
        cv::Mat frame = this->cvFrameQueue_->frameQueue.front();
        this->cvFrameQueue_->frameQueue.pop();
        lock.unlock();

        std::vector<cv::Rect> faces;
  		face_cascade.detectMultiScale(frame, faces);

        facesRecognitionEvent.Clear();
        for (const auto& face : faces) {
            Rect* faceRect = new Rect();
            faceRect->set_x(face.x);
            faceRect->set_y(face.y);
            faceRect->set_height(face.height);
            faceRect->set_width(face.width);
            facesRecognitionEvent.mutable_faces()->AddAllocated(faceRect);
        }

        if (facesRecognitionEvent.faces_size() > 0) {
            if (this->protobufMessageSender_ != nullptr) {
                this->protobufMessageSender_->sendPayload(msgHeader, facesRecognitionEvent);
            }
        }

    }

}