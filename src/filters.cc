// MIT License

// Copyright (c) 2024 Abhishek Sriram

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/// @file
/// @brief This file contains the declarations for all the image filtering processes

#include <filters.h>

/// @brief Calculate the four points to preserve region of interest
/// @param midpoint_x image midpoint at x-axis
/// @param midpoint_y image midpoint at y-axis
/// @return Array of points
ROIPoints Filters::calcPoints(const int &midpoint_x, const int &midpoint_y, const int &height)
{
    ROIPoints points = {
        cv::Point(midpoint_x - midpoint_x * DELTA_X1, midpoint_y),
        cv::Point(midpoint_x + midpoint_x * DELTA_X1, midpoint_y),
        cv::Point(midpoint_x + midpoint_x * DELTA_X2, height),
        cv::Point(midpoint_x - midpoint_x * DELTA_X2, height),
    };

    return points;
}

/// @brief Preserve the region of interest in the image
/// @param images Array of images
/// @return Array of preserved region of interest in the image
ImageSequence Filters::roi(const ImageSequence &images)
{
    ImageSequence roi_images;
    for (const auto &image : images)
    {
        cv::Mat roi_image;
        int start_row = image.rows * DELTA_ROI;
        // roi_image = image(cv::Range(start_row, image.rows), cv::Range::all()); // crop image along its y-axis
        const int midpoint_x = image.size().width / 2;
        const int midpoint_y = image.size().height / 2;

        ROIPoints points = calcPoints(midpoint_x, midpoint_y, image.size().height);

        // Plot points to show the region of interest mask
        // cv::circle(image, points[0], 2, cv::Scalar(0, 0, 255), 10);
        // cv::circle(image, points[1], 2, cv::Scalar(0, 255, 0), 10);
        // cv::circle(image, points[2], 2, cv::Scalar(255, 255, 255), 10);
        // cv::circle(image, points[3], 2, cv::Scalar(0, 0, 0), 10);

        // Create a mask for the trapezoidal region
        cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);
        cv::fillConvexPoly(mask, points, cv::Scalar(255));

        // Extract the region of interest using the mask
        image.copyTo(roi_image, mask);

        roi_images.push_back(roi_image);
    }

    return roi_images;
}

/// @brief Inverse perspective mapping of images to potray the resultant images as if it were taken by a bird's eye camera
/// @param images Array of images
/// @return Array of images with inverse perspective mapping
ImageSequence Filters::perspectiveMapping(const ImageSequence &images)
{
    ImageSequence perspective_images;
    for (const auto &image : images)
    {
        const int midpoint_x = image.size().width / 2;
        const int midpoint_y = image.size().height / 2;

        ROIPoints points = calcPoints(midpoint_x, midpoint_y, image.size().height);

        ROIPoints transform_points = {
            cv::Point(0, 0),
            cv::Point(500, 0),
            cv::Point(500, 500),
            cv::Point(0, 500),
        };

        // Convert points to floating-point format
        std::vector<cv::Point2f> srcPointsFloat, transformsFloat;
        cv::Mat(points).convertTo(srcPointsFloat, CV_32F);
        cv::Mat(transform_points).convertTo(transformsFloat, CV_32F);

        cv::Mat perspective_image;
        cv::Mat transformation_matrix = cv::getPerspectiveTransform(srcPointsFloat, transformsFloat);
        cv::warpPerspective(image, perspective_image, transformation_matrix, cv::Size(500, 500));

        transformation_matrices.push_back(transformation_matrix);
        perspective_images.push_back(perspective_image);
    }

    return perspective_images;
}

/// @brief Dewarp the image onto original image space
/// @param images array of images
/// @param p_hough_lines probabilistic hough lines
/// @return Dewarped image
ImageSequence Filters::deWarpMapping(const ImageSequence &images, const std::vector<std::vector<cv::Vec4i>> &p_hough_lines, const cv::Scalar &color, const int &thickness, const int &line_type, const int &shift)
{
    ImageSequence dewarped_images;
    for (size_t i = 0; i < images.size(); i++)
    {
        cv::Mat inverse_matrix = transformation_matrices[i].inv();
        std::vector<cv::Vec4i> hough_lines = p_hough_lines[i];
        std::vector<cv::Vec4i> dewarped_hough_lines;

        for (const auto &hough_line : hough_lines)
        {
            int x1 = hough_line[0];
            int y1 = hough_line[1];
            int x2 = hough_line[2];
            int y2 = hough_line[3];

            // Transform the endpoints from the warped image to the original image
            cv::Mat p1 = (cv::Mat_<float>(3, 1) << x1, y1, 1);
            cv::Mat p2 = (cv::Mat_<float>(3, 1) << x2, y2, 1);

            // Ensure inv_matrix is of type CV_32F
            cv::Mat inv_matrix_float;
            inverse_matrix.convertTo(inv_matrix_float, CV_32F);

            cv::Mat p1_de_warped = inv_matrix_float * p1;
            cv::Mat p2_de_warped = inv_matrix_float * p2;

            // Convert homogeneous coordinates to regular coordinates
            p1_de_warped /= p1_de_warped.at<float>(2);
            p2_de_warped /= p2_de_warped.at<float>(2);

            // Adjust the coordinates based on the top-left corner of the ROI
            int p1_de_warped_x = static_cast<int>(p1_de_warped.at<float>(0));
            int p1_de_warped_y = static_cast<int>(p1_de_warped.at<float>(1));
            int p2_de_warped_x = static_cast<int>(p2_de_warped.at<float>(0));
            int p2_de_warped_y = static_cast<int>(p2_de_warped.at<float>(1));

            dewarped_hough_lines.push_back(cv::Vec4i(p1_de_warped_x, p1_de_warped_y, p2_de_warped_x, p2_de_warped_y));
        }

        for (const auto lines : dewarped_hough_lines)
        {
            line(images[i], cv::Point(lines[0], lines[1]), cv::Point(lines[2], lines[3]), color, thickness, line_type, shift);
        }
        dewarped_images.push_back(images[i].clone());
    }

    return dewarped_images;
}

/// @brief Convert image color from one color space to another
/// @param images Array of images
/// @param color_code Color conversion code given by OpenCV
/// @return Array of images in another color space
ImageSequence Filters::convertColor(const ImageSequence &images, const cv::ColorConversionCodes &color_code)
{
    ImageSequence color_converted_images;
    for (const auto &image : images)
    {
        cv::Mat color_converted_image;
        cv::cvtColor(image, color_converted_image, color_code);
        color_converted_images.push_back(color_converted_image);
    }

    return color_converted_images;
}

/// @brief Apply median filter on images. To learn more, visit: https://docs.opencv.org/4.x/d4/d86/group__imgproc__filter.html#ga564869aa33e58769b4469101aac458f9
/// @param images Array of images
/// @param median_kernel_size Kernel size to apply filter
/// @return array of images with median blur
ImageSequence Filters::medianBlur(const ImageSequence &images, const int &median_kernel_size)
{
    ImageSequence blurred_images;
    for (const auto &image : images)
    {
        cv::Mat blurred_image;
        cv::medianBlur(image, blurred_image, MEDIAN_KERNEL_SIZE);
        blurred_images.push_back(blurred_image);
    }

    return blurred_images;
}

/// @brief Apply Gaussian filter on images. To learn more, visit: https://docs.opencv.org/4.x/d4/d86/group__imgproc__filter.html#gaabe8c836e97159a9193fb0b11ac52cf1
/// @param images array of images
/// @param gaussian_kernel_size gaussian kernel
/// @param sigma_x kernel standard deviation in X direction
/// @param sigma_y kernel standard deviation in Y direction
/// @param gaussian_border_type kernel standard deviation in Y direction; if sigmaY is zero it is set to be equal to sigmaX
/// @return array of images with gaussian blur
ImageSequence Filters::gaussianBlur(const ImageSequence &images, const cv::Size &gaussian_kernel_size, const int &sigma_x, const int &sigma_y, const int &gaussian_border_type)
{
    ImageSequence blurred_images;
    for (const auto &image : images)
    {
        cv::Mat blurred_image;
        cv::GaussianBlur(image, blurred_image, gaussian_kernel_size, sigma_x, sigma_y, gaussian_border_type);
        blurred_images.push_back(blurred_image);
    }

    return blurred_images;
}

/// @brief Apply Bilateral filter on images. To learn more, visit: https://docs.opencv.org/4.x/d4/d86/group__imgproc__filter.html#ga9d7064d478c95d60003cf839430737ed
/// @param images array of images
/// @param diamater diameter of each pixel neighborhood that is used during filtering. If it is non-positive, it is computed from sigmaSpace.
/// @param sigmaColor filter sigma in the color space.
/// @param sigmaSpace filter sigma in the coordinate space.
/// @param border_type border mode used to extrapolate pixels outside of the image,
/// @return images with bilateral blur
ImageSequence Filters::bilateralBlur(const ImageSequence &images, const int &diamater, const double &sigma_color, const double &sigma_space, const int &border_type)
{
    ImageSequence blurred_images;
    for (const auto &image : images)
    {
        cv::Mat blurred_image;
        cv::bilateralFilter(image, blurred_image, diamater, sigma_color, sigma_space, border_type);
        blurred_images.push_back(blurred_image);
    }

    return blurred_images;
}
