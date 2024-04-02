#ifndef LANE_DETECTION_H
#define LANE_DETECTION_H

#include <opencv2/opencv.hpp>
#include <vector>

class LaneDetection
{
public:
    // apply sobel operator
    std::vector<cv::Mat> sobelFilter(const std::vector<cv::Mat> &frames);

    // apply canny edge operator
    std::vector<cv::Mat> cannyFilter(const std::vector<cv::Mat> &frames, const int &l_threshold = 75, const int &h_threshold = 150);

    // probabilistic hough transform
    std::vector<cv::Mat> houghTransform(const std::vector<cv::Mat> &frames, const std::vector<cv::Mat> &canny_frames);

private:
    // const values for returning status
    // const int FAILURE = 0;
    // const int SUCCESS = 1;

    // cv::VideoCapture capture_; // cv::Video object
    // std::string file_;         // file
    std::string error_; // Error message
};

#endif