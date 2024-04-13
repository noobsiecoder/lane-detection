#ifndef CAPTURE_IMAGE
#define CAPTURE_IMAGE

#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#define VIDEO_IS_COLOR false

class Capture
{
public:
    // Initialize object by passing an image as parameter
    //
    // PARAMETERS:
    //      1. file_path
    //          * type: &std::string
    //          * desc: file path containing images
    //
    // EXAMPLES:
    //      ...
    //      object Capture(&file_path);

    Capture(const std::string &file_path) : file_path_(file_path) {}

    // Load image files into a vector of cv::Mat
    //
    // EXAMPLES:
    //      ...
    //      object.loadImageFiles();
    //
    // RETURNS:
    //      1. images
    //          * type: std::vector<cv::Mat>
    //          * desc: array of images

    std::vector<cv::Mat> loadImageFiles() const;

    // Convert into a video format
    //
    // PARAMETERS:
    //      1. images
    //          * type: &std::vector<cv::Mat>
    //          * desc: array of images
    //
    // EXAMPLES:
    //      ...
    //      object.stitchToVideo();
    //
    // RETURNS:
    //      1. video
    //          * type: cv::VideoWriter
    //          * desc: video

    void stitchToVideo(const std::vector<cv::Mat> &images, const std::string &file_name, const bool &isColor = VIDEO_IS_COLOR) const;

private:
    // Check if path exists
    //
    // RETURNS:
    //      1. filtered_image
    //          * type: <cv::Mat>
    //          * desc: smoothened image

    bool checkPathExists() const;
    std::string file_path_;
};

#endif