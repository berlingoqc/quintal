#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

#include <opencv2/opencv.hpp>

struct CVFrameQueue {
    std::queue<cv::Mat> frameQueue;
    std::mutex mtx;
    std::condition_variable condVar;
};