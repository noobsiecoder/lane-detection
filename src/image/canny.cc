#include <image/canny.h>
#include <custom_exception.h>

std::vector<cv::Mat> CannyDetection::canny(const int &l_thresh, const int &h_thresh, const int &aperture)
{
    std::vector<cv::Mat> canny_images;
    for (const auto &image : images_)
    {
        cv::Mat canny_image;

        // l_thresh
        // h_thresh
        // aperture
        // l2_gradient

        cv::Canny(image, canny_image, l_thresh, h_thresh, aperture); // extract canny edges from the image

        canny_images.push_back(canny_image);
    }

    if (canny_images.empty())
    {
        std::string err = "No images found while extracting features";
        throw CaptureEmptyException(err);
    }

    return canny_images;
}