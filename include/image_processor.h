#ifndef VIDEO_LOADER_H
#define VIDEO_LOADER_H

#include <opencv2/opencv.hpp>
// #include <sstream>
// #include <chrono>
#include <vector>
#include <fstream>

class ImageProcessor
{
public:
    // constructor
    ImageProcessor(const std::string &file);

    // destructor
    ~ImageProcessor();

    // load video object
    int loadVideo();

    // load video object to a vector
    std::vector<cv::Mat> loadVideoToVector();

    // convert video object to grayscale
    std::vector<cv::Mat> grayscale(const std::vector<cv::Mat> &frames);

    // apply median filter to smooth image
    std::vector<cv::Mat> medianFilter(const std::vector<cv::Mat> &frames, const int &kernelSize = 5);

    // apply gaussian noise (remove salt and pepper noise)
    std::vector<cv::Mat> gaussianFilter(const std::vector<cv::Mat> &frames, const cv::Size &kernelSize = cv::Size(5, 5), const double &deviation = 2.50);

    // apply histogram equalization to imporve contrast and brightness
    std::vector<cv::Mat> histogramFilter(const std::vector<cv::Mat> &frames);

    // dilate filter
    std::vector<cv::Mat> dilate(const std::vector<cv::Mat> &frames, const int &kernelSize = 3);

    // erosion filter
    std::vector<cv::Mat> erosion(const std::vector<cv::Mat> &frames, const int &kernelSize = 3);

    // filter the unnecessary area from the frame
    std::vector<cv::Mat> regionOfInterest(const std::vector<cv::Mat> &frames);

private:
    // const values for returning status
    const int FAILURE = 0;
    const int SUCCESS = 1;

    cv::VideoCapture capture_; // cv::Video object
    std::string file_;         // file
    std::string error_;        // Error message

    // check file exists
    bool fileExists() const;

    // release captured video frame
    void releaseCapture();
};

#endif
