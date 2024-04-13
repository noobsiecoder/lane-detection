#ifndef HOUGH_IMAGE
#define HOUGH_IMAGE

#include <opencv2/opencv.hpp>

#define HOUGH_RHO 1
#define HOUGH_THETA (CV_PI / 180)
#define HOUGH_THRESHOLD 50
#define HOUGH_SRN 0.0
#define HOUGH_STN 0.0
#define HOUGH_MIN_THETA 0.0
#define HOUGH_MAX_THETA (CV_PI)
#define HOUGH_SCALAR_COLOR cv::Scalar(0, 0, 255)
#define HOUGH_THICKNESS 1
#define HOUGH_LINETYPE cv::LINE_AA
#define HOUGH_SHIFT 0

class HoughTransform
{
public:
    // Initialize object by passing an image as parameter
    //
    // PARAMETERS:
    //      1. image
    //          * type: &<cv::Mat>
    //          * desc: filtered image
    //
    // EXAMPLES:
    //      ...
    //      object HoughTransform(&image);

    HoughTransform(const std::vector<cv::Mat> &images) : images_(images) {}

    // Plot hough lines
    //
    // PARAMETERS:
    //      1. l_thresh
    //          * type: &<int>
    //          * desc: lowest threshold
    //      2. h_thresh
    //          * type: &<int>
    //          * desc: highest threshold
    //
    // EXAMPLES:
    //      ...
    //      cv::Mat houghLines = object.houghLines();
    //      cv::Mat houghLines = object.houghLines(50, 175);
    //
    // RETURNS:
    //      1. image
    //          * type: <cv::Mat>
    //          * desc: detected lines

    std::vector<cv::Mat> houghLines(const std::vector<cv::Mat> &roi, const double &rho = HOUGH_RHO, const double &theta = HOUGH_THETA, const int &threshold = HOUGH_THRESHOLD, const double &srn = HOUGH_SRN, const double &stn = HOUGH_STN, const double &min_theta = HOUGH_MIN_THETA, const double &max_theta = HOUGH_MAX_THETA, const cv::Scalar &color = HOUGH_SCALAR_COLOR, const int &thickness = HOUGH_THICKNESS, const int &line_Type = HOUGH_LINETYPE, const int &shift = HOUGH_SHIFT);

    // Plot probabilistic hough lines
    //
    // PARAMETERS:
    //      1. l_thresh
    //          * type: &<int>
    //          * desc: lowest threshold
    //      2. h_thresh
    //          * type: &<int>
    //          * desc: highest threshold
    //
    // EXAMPLES:
    //      ...
    //      cv::Mat probhoughLines = object.probHoughLines();
    //      cv::Mat probhoughLines = object.probHoughLines(50, 175);
    //
    // RETURNS:
    //      1. image
    //          * type: <cv::Mat>
    //          * desc: detected lines

    cv::Mat probHoughLines();

    // Clear object cache (or) remove image content
    //
    // NOTE:
    //!     Call after peforming canny detection
    //
    // EXAMPLES:
    //      object.~CannyDetection();

    // ~HoughTransform();

private:
    // Erase image_ value
    //
    // NOTE:
    //!     Should not be called by any member functions except the destructor
    //
    // EXAMPLES:
    //      object.~CannyDetection();
    //
    // RETURNS:
    //      1. status
    //          * type: <int>
    //          * desc: Return operation status [1 = success (or) 0 = fail]

    int clearCache();

    std::vector<cv::Mat> images_;
};

#endif