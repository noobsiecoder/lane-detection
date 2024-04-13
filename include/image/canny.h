#ifndef CANNY_DETECTION_IMAGE
#define CANNY_DETECTION_IMAGE

#include <opencv2/opencv.hpp>

#define CANNY_LOWER_THRESHOLD 75
#define CANNY_HIGHER_THRESHOLD 175
#define CANNY_APERTURE 3

// Canny Edge Detection
//
// NOTE:
//

class CannyDetection
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
    //      object CannyDetection(&image);

    CannyDetection(const std::vector<cv::Mat> &images) : images_(images) {}

    // Perform canny edge detection on an image
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
    //      cv::Mat canny = object.canny();
    //      cv::Mat canny = object.canny(50, 175);
    //
    // RETURNS:
    //      1. image
    //          * type: std::vector<cv::Mat>
    //          * desc: detected lines

    std::vector<cv::Mat> canny(const int &l_thresh = CANNY_LOWER_THRESHOLD, const int &h_thresh = CANNY_HIGHER_THRESHOLD, const int &aperture = CANNY_APERTURE);

    // Clear object cache (or) remove image content
    //
    // NOTE:
    //!     Call after peforming canny detection
    //
    // EXAMPLES:
    //      object.~CannyDetection();

    // ~CannyDetection();

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