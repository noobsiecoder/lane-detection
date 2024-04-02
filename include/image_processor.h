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

    // convert video object to grayscale
    int grayscaleVideo();

    // apply median filter to smooth image
    int medianFilter(const int &kernelSize = 5);

    // apply gaussian noise (remove salt and pepper noise)
    int gaussianFilter(const cv::Size &kernelSize = cv::Size(5, 5), const double &deviation = 2.50);

    // apply histogram equalization to imporve contrast and brightness
    int histogramFilter();

    // apply dilation and erosion filter
    int miscellaneousFilters(const int &kernelSize = 3);

    // filter the unnecessary area from the frame
    void regionOfInterest();

    // only for development purposes
    std::vector<cv::Mat> getGrayscaleFrames() const;             //! DEV USE ONLY, DELETE LATER
    cv::VideoCapture getCapturedFrames() const;                  //! DEV USE ONLY, DELETE LATER
    std::vector<cv::Mat> getMedianBlurFrames() const;            //! DEV USE ONLY, DELETE LATER
    std::vector<cv::Mat> getGaussianBlurFrames() const;          //! DEV USE ONLY, DELETE LATER
    std::vector<cv::Mat> getHistogramFilteredFrames() const;     //! DEV USE ONLY, DELETE LATER
    std::vector<cv::Mat> getMiscellaneousFilteredFrames() const; //! DEV USE ONLY, DELETE LATER

private:
    // const values for returning status
    const int FAILURE = 0;
    const int SUCCESS = 1;

    cv::VideoCapture capture_;          // cv::Video object
    std::string file_;                  // file
    std::vector<cv::Mat> frames_;       // resultant frame
    std::vector<cv::Mat> grayscale_;    // grayscale frames
    std::vector<cv::Mat> medianblur_;   // frames with median blur
    std::vector<cv::Mat> gaussianblur_; // frames with gaussian blur + median blur
    std::vector<cv::Mat> histogrameq_;  // frames with gaussian blur + median blur + histogram equalization
    std::vector<cv::Mat> miscfilter_;   // frames with gaussian blur + median blur + histogram equalization + dilate + erosion

    // check file exists
    bool fileExists() const;

    // release captured video frame
    void releaseCapture();
};

#endif
