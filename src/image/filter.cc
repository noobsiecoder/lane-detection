#include <image/filter.h>
#include <custom_exception.h>

#define REGION_OF_INTEREST 0.49

std::vector<cv::Mat> Filters::regionOfInterest(const std::vector<cv::Mat> &images)
{
    std::vector<cv::Mat> roi_images;
    for (const auto &image : images)
    {
        cv::Mat roi;
        int start_row = image.rows * REGION_OF_INTEREST;
        roi = image(cv::Range(start_row, image.rows), cv::Range::all()); // crop image along its y-axis

        roi_images.push_back(roi);
    }

    if (roi_images.empty())
    {
        std::string err = "No images found while preserving the region of interest";
        throw CaptureEmptyException(err);
    }

    return roi_images;
}

std::vector<cv::Mat> Filters::grayscale(const std::vector<cv::Mat> &images)
{
    std::vector<cv::Mat> grayscale_images;
    for (const auto &image : images)
    {
        cv::Mat grayscale;
        cv::cvtColor(image, grayscale, cv::COLOR_BGR2GRAY); // rgb to grayscale

        grayscale_images.push_back(grayscale);
    }

    if (grayscale_images.empty())
    {
        std::string err = "No images found while converting to grayscale";
        throw CaptureEmptyException(err);
    }

    return grayscale_images;
}

std::vector<cv::Mat> Filters::medianBlur(const std::vector<cv::Mat> &images, const int &kernel_size)
{
    std::vector<cv::Mat> blur_images;
    for (const auto &image : images)
    {
        cv::Mat blur_image;
        cv::medianBlur(image, blur_image, kernel_size); // apply median blur with kernel of size := DEFAULT(5)

        blur_images.push_back(blur_image);
    }

    if (blur_images.empty())
    {
        std::string err = "No images found while applying median blur";
        throw CaptureEmptyException(err);
    }

    return blur_images;
}

std::vector<cv::Mat> Filters::gaussianBlur(const std::vector<cv::Mat> &images, const cv::Size &kernel_size, const double &sigmaX, const double &sigmaY, const int &border_type)
{
    std::vector<cv::Mat> blur_images;
    for (const auto &image : images)
    {
        cv::Mat blur_image;

        // kernel_size  :   apply gaussian blur with kernel of size := DEFAULT(5)
        // sigmaX       :   standard deviation along x-axis
        // sigmaY       :   standard deviation along y-axis
        // border_type  :   handling pixels outside boundary during convulation

        cv::GaussianBlur(image, blur_image, kernel_size, sigmaX, sigmaY, border_type);

        blur_images.push_back(blur_image);
    }

    if (blur_images.empty())
    {
        std::string err = "No images found while applying gaussian blur";
        throw CaptureEmptyException(err);
    }

    return blur_images;
}

std::vector<cv::Mat> Filters::histogramEqualization(const std::vector<cv::Mat> &images)
{
    std::vector<cv::Mat> equalized_images;
    for (const auto &image : images)
    {
        cv::Mat equalized_image;
        cv::equalizeHist(image, equalized_image); // apply histogram equalization on image (adjust the contrast)

        equalized_images.push_back(equalized_image);
    }

    if (equalized_images.empty())
    {
        std::string err = "No images found while applying gaussian blur";
        throw CaptureEmptyException(err);
    }

    return equalized_images;
}

std::vector<cv::Mat> Filters::dilate(const std::vector<cv::Mat> &images, const cv::InputArray &kernel_size, const cv::Point &anchor, const int &iterations, const int &border_type, const cv::Scalar &border_value)
{
    std::vector<cv::Mat> dilated_images;
    for (const auto &image : images)
    {
        cv::Mat dilated_image;

        // cv::InputArray kernel
        // cv::Point anchor = cv::Point(-1, -1)
        // int iterations = 1
        // int borderType = 0
        // const cv::Scalar &borderValue = morphologyDefaultBorderValue()

        cv::dilate(image, dilated_image, kernel_size, anchor, iterations, border_type, border_value); // apply dilation on image

        dilated_images.push_back(dilated_image);
    }

    if (dilated_images.empty())
    {
        std::string err = "No images found while applying dilation";
        throw CaptureEmptyException(err);
    }

    return dilated_images;
}

std::vector<cv::Mat> Filters::erode(const std::vector<cv::Mat> &images, const cv::InputArray &kernel_size, const cv::Point &anchor, const int &iterations, const int &border_type, const cv::Scalar &border_value)
{
    std::vector<cv::Mat> eroded_images;
    for (const auto &image : images)
    {
        cv::Mat eroded_image;

        // cv::InputArray kernel
        // cv::Point anchor = cv::Point(-1, -1)
        // int iterations = 1
        // int borderType = 0
        // const cv::Scalar &borderValue = morphologyDefaultBorderValue()

        cv::erode(image, eroded_image, kernel_size, anchor, iterations, border_type, border_value); // apply erosion on image

        eroded_images.push_back(eroded_image);
    }

    if (eroded_images.empty())
    {
        std::string err = "No images found while applying erosion";
        throw CaptureEmptyException(err);
    }

    return eroded_images;
}