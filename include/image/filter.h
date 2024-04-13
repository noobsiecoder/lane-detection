#ifndef FILTER_IMAGE
#define FILTER_IMAGE

#include <vector>
#include <opencv2/opencv.hpp>

#define MEDIAN_BLUR_KERNEL_SIZE 5
#define GAUSSIAN_BLUR_KERNEL_SIZE cv::Size(5, 5)
#define GAUSSIAN_BLUR_SIGMA_X 2.0
#define GAUSSIAN_BLUR_SIGMA_Y 0.0
#define GAUSSIAN_BLUR_BORDER_TYPE 4
#define DILATE_KERNEL_SIZE 5
#define DILATE_ANCHOR cv::Point(-1, -1)
#define DILATE_ITERATIONS 1
#define DILATE_BORDER_TYPE 0
#define DILATE_BORDER_VALUE cv::morphologyDefaultBorderValue()

// Filters
//
// NOTE:
//

class Filters
{
public:
    // Initialize object by passing an image as parameter
    //
    // PARAMETERS:
    //      1. image
    //          * type: &std::vector<<cv::Mat>>
    //          * desc: image data
    //      2. options
    //          * type: &<std::vector<FilterOptions>>
    //          * desc: list of filters to apply on the image
    //
    // EXAMPLES:
    //      ...
    //      object Filters(&image, &options);

    // Filters(const std::vector<cv::Mat> &images) : images_(images){};

    // Crop image to only consider the region of interest
    //
    // EXAMPLES:
    //      ...
    //      object applyFilters();
    //
    // RETURNS:
    //      1. filtered_image
    //          * type: <cv::Mat>
    //          * desc: cropped image

    std::vector<cv::Mat> regionOfInterest(const std::vector<cv::Mat> &images);

    // Convert image to grayscale
    //
    // PARAMETERS:
    //      1. image
    //          * type: &<std::vector<cv::Mat>>
    //          * desc: unfiltered images with 3 channels
    //
    // EXAMPLES:
    //      cv::Mat grayscale = object.grayscale();
    //
    // RETURNS:
    //      1. image
    //          * type: std::vector<cv::Mat>
    //          * desc: grayscaled images

    std::vector<cv::Mat> grayscale(const std::vector<cv::Mat> &images);

    // Clear object cache (or) remove image content
    //
    // NOTE:
    //!     Call after applying filters
    //
    // EXAMPLES:
    //      object.~Filters();

    // ~Filters();

    // Apply median blur filter on image
    //
    // PARAMETERS:
    //      1. image
    //          * type: &<std::vector<cv::Mat>>
    //          * desc: unblurred image
    //
    // EXAMPLES:
    //      cv::Mat median_blur = object.medianBlur();
    //
    // RETURNS:
    //      1. filtered_image
    //          * type: std::vector<cv::Mat>
    //          * desc: blurred (median) image

    std::vector<cv::Mat> medianBlur(const std::vector<cv::Mat> &images, const int &kernal_size = MEDIAN_BLUR_KERNEL_SIZE);

    // Apply gaussian blur filter on image
    //
    // PARAMETERS:
    //
    // EXAMPLES:
    //      cv::Mat gaussian_blur = object.gaussianBlur();
    //
    // RETURNS:
    //      1. filtered_image
    //          * type: std::vector<cv::Mat>
    //          * desc: smoothened image

    std::vector<cv::Mat> gaussianBlur(const std::vector<cv::Mat> &images, const cv::Size &kernal_size = GAUSSIAN_BLUR_KERNEL_SIZE, const double &sigmaX = GAUSSIAN_BLUR_SIGMA_X, const double &sigmaY = GAUSSIAN_BLUR_SIGMA_Y, const int &border_type = GAUSSIAN_BLUR_BORDER_TYPE);

    // Apply histogram equalization filter on the image
    //
    // PARAMETERS:
    //
    // EXAMPLES:
    //      cv::Mat hist_eq = object.histogramEqualization();
    //
    // RETURNS:
    //      1. filtered_image
    //          * type: std::vector<cv::Mat>
    //          * desc: histogram equalized image

    std::vector<cv::Mat> histogramEqualization(const std::vector<cv::Mat> &images);

    // Accentuate object by increasing its area
    //
    // PARAMETERS:
    //
    // EXAMPLES:
    //      cv::Mat dilate = object.dilate();
    //
    // RETURNS:
    //      1. filtered_image
    //          * type: <cv::Mat>
    //          * desc: histogram equalized image

    std::vector<cv::Mat> dilate(const std::vector<cv::Mat> &images, const cv::InputArray &kernel_size = DILATE_KERNEL_SIZE, const cv::Point &anchor = DILATE_ANCHOR, const int &iterations = DILATE_ITERATIONS, const int &border_type = DILATE_BORDER_TYPE, const cv::Scalar &border_value = DILATE_BORDER_VALUE);

    // Diminish feature by eroding boundary
    //
    // PARAMETERS:
    //
    // EXAMPLES:
    //      cv::Mat erode = object.erode();
    //
    // RETURNS:
    //      1. filtered_image
    //          * type: <cv::Mat>
    //          * desc: smoothened image

    std::vector<cv::Mat> erode(const std::vector<cv::Mat> &images, const cv::InputArray &kernel_size = DILATE_KERNEL_SIZE, const cv::Point &anchor = DILATE_ANCHOR, const int &iterations = DILATE_ITERATIONS, const int &border_type = DILATE_BORDER_TYPE, const cv::Scalar &border_value = DILATE_BORDER_VALUE);

private:
    // Erase image_ value
    //
    // NOTE:
    //!     Should not be called by any member functions except the destructor
    //
    // EXAMPLES:
    //      object.~Filters();
    //
    // RETURNS:
    //      1. status
    //          * type: <int>
    //          * desc: Return operation status [1 = success (or) 0 = fail]

    int clearImageCache();

    // Erase options_ value
    //
    // NOTE:
    //!     Should not be called by any member functions except the destructor
    //
    // EXAMPLES:
    //      object.~Filters();
    //
    // RETURNS:
    //      1. status
    //          * type: <int>
    //          * desc: Return operation status [1 = success (or) 0 = fail]

    int clearFilterCache();

    std::vector<cv::Mat> images_;
};

#endif