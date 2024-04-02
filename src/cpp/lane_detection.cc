#include <lane_detection.h>
#include <custom_exception.h>

// apply sobel operator
std::vector<cv::Mat> LaneDetection::sobelFilter(const std::vector<cv::Mat> &frames)
{
    std::vector<cv::Mat> frames_;
    for (const auto &frame : frames)
    {
        cv::Mat grad_x, grad_y;
        cv::Sobel(frame, grad_x, CV_16S, 1, 0); // compute over x direction
        cv::Sobel(frame, grad_y, CV_16S, 0, 1); // compute over y direction

        // to 8-bit unsigned integer
        cv::Mat abs_grad_x, abs_grad_y;
        cv::convertScaleAbs(grad_x, abs_grad_x);
        cv::convertScaleAbs(grad_y, abs_grad_y);

        // final output
        cv::Mat sobel_frame;
        cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, sobel_frame);
        frames_.push_back(sobel_frame.clone());
    }

    if (frames_.empty())
    {
        error_ += "Capture object is empty!";
        throw CaptureEmptyException(error_);
    }

    return frames_;
}

// apply canny edge detector
std::vector<cv::Mat> LaneDetection::cannyFilter(const std::vector<cv::Mat> &frames, const int &l_threshold, const int &h_threshold)
{
    std::vector<cv::Mat> frames_;
    for (const auto &frame : frames)
    {
        // final output
        cv::Mat canny_frame;
        cv::Canny(frame, canny_frame, l_threshold, h_threshold);
        frames_.push_back(canny_frame.clone());
    }

    if (frames_.empty())
    {
        error_ += "Capture object is empty!";
        throw CaptureEmptyException(error_);
    }

    return frames_;
}

// probabilistic hough transform
std::vector<cv::Mat> LaneDetection::houghTransform(const std::vector<cv::Mat> &frames, const std::vector<cv::Mat> &canny_frames)
{
    // store hough lines
    std::vector<cv::Vec4i> lines;
    std::vector<cv::Mat> frames_;

    // hough lines in canny_frame
    for (const auto &canny_frame : canny_frames)
    {
        cv::HoughLinesP(canny_frame, lines, 1, CV_PI / 180, 50, 50, 10);
    }

    // plot hough lines in original frame
    for (const auto &frame : frames)
    {
        for (const cv::Vec4i &line : lines)
        {
            cv::line(frame, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(0, 0, 255), 2);
        }

        // final output
        frames_.push_back(frame.clone());
        // cv::Canny(frame, canny_frame);
        // frames_.push_back(canny_frame.clone());
    }

    if (frames_.empty())
    {
        error_ += "Capture object is empty!";
        throw CaptureEmptyException(error_);
    }

    return frames_;
}