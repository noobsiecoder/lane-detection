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
/// @brief This file contains the definition for all the image filtering processes

#ifndef FILTERS_H
#define FILTERS_H

#include <vector>
#include <opencv2/imgproc.hpp>
#include <custom_types.h>

#define DELTA_X1 0.05
#define DELTA_X2 0.4
#define DELTA_Y 0.95
#define DELTA_ROI 0.48

#define DEWARP_SCALAR_COLOR cv::Scalar(0, 0, 255)
#define DEWARP_THICKNESS 2
#define DEWARP_LINETYPE cv::LINE_AA
#define DEWARP_SHIFT 0

#define MEDIAN_KERNEL_SIZE 5

#define GAUSSIAN_KERNEL_SIZE cv::Size(5, 5)
#define GAUSSIAN_SIGMA_X 5
#define GAUSSIAN_SIGMA_Y 5
#define GAUSSIAN_BORDER_TYPE 4

#define BILATERAL_DIAMETER 5
#define BILATERAL_SIGMA_COLOR 0.3
#define BILATERAL_SIGMA_SPACE 10

/// @brief Class to preprocess image before extracting necessary features
class Filters
{
public:
    /// @brief Preserve the region of interest in the image
    /// @param images Array of images
    /// @return Array of preserved region of interest in the image
    ImageSequence roi(const ImageSequence &images);

    /// @brief Inverse perspective mapping of images to potray the resultant images as if it were taken by a bird's eye camera
    /// @param images Array of images
    /// @return Array of images with inverse perspective mapping
    ImageSequence perspectiveMapping(const ImageSequence &images);

    /// @brief Dewarp the image onto original image space
    /// @param images array of images
    /// @param p_hough_lines probabilistic hough lines
    /// @return Dewarped image
    ImageSequence deWarpMapping(const ImageSequence &images, const std::vector<std::vector<cv::Vec4i>> &p_hough_lines, const cv::Scalar &color = DEWARP_SCALAR_COLOR, const int &thickness = DEWARP_THICKNESS, const int &line_type = DEWARP_LINETYPE, const int &shift = DEWARP_SHIFT);

    /// @brief Convert image color from one color space to another
    /// @param images Array of images
    /// @param color_code Color conversion code given by OpenCV
    /// @return Array of images in another color space
    ImageSequence convertColor(const ImageSequence &images, const cv::ColorConversionCodes &color_code);

    /// @brief Apply median filter on images. To learn more, visit: https://docs.opencv.org/4.x/d4/d86/group__imgproc__filter.html#ga564869aa33e58769b4469101aac458f9
    /// @param images Array of images
    /// @param median_kernel_size Kernel size to apply filter
    /// @return array of images with median blur
    ImageSequence medianBlur(const ImageSequence &images, const int &median_kernel_size = MEDIAN_KERNEL_SIZE);

    /// @brief Apply Gaussian filter on images. To learn more, visit: https://docs.opencv.org/4.x/d4/d86/group__imgproc__filter.html#gaabe8c836e97159a9193fb0b11ac52cf1
    /// @param images array of images
    /// @param gaussian_kernel_size gaussian kernel
    /// @param sigma_x kernel standard deviation in X direction
    /// @param sigma_y kernel standard deviation in Y direction
    /// @param gaussian_border_type kernel standard deviation in Y direction; if sigmaY is zero it is set to be equal to sigmaX
    /// @return array of images with gaussian blur
    ImageSequence gaussianBlur(const ImageSequence &images, const cv::Size &gaussian_kernel_size = GAUSSIAN_KERNEL_SIZE, const int &sigma_x = GAUSSIAN_SIGMA_X, const int &sigma_y = GAUSSIAN_SIGMA_Y, const int &gaussian_border_type = GAUSSIAN_BORDER_TYPE);

    /// @brief Apply Bilateral filter on images. To learn more, visit: https://docs.opencv.org/4.x/d4/d86/group__imgproc__filter.html#ga9d7064d478c95d60003cf839430737ed
    /// @param images array of images
    /// @param diamater diameter of each pixel neighborhood that is used during filtering. If it is non-positive, it is computed from sigmaSpace.
    /// @param sigmaColor filter sigma in the color space.
    /// @param sigmaSpace filter sigma in the coordinate space.
    /// @param border_type border mode used to extrapolate pixels outside of the image,
    /// @return images with bilateral blur
    ImageSequence bilateralBlur(const ImageSequence &images, const int &diamater = BILATERAL_DIAMETER, const double &sigmaColor = BILATERAL_SIGMA_COLOR, const double &sigmaSpace = BILATERAL_SIGMA_SPACE, const int &border_type = GAUSSIAN_BORDER_TYPE);

private:
    ImageSequence transformation_matrices; // transformation matrices from src and dst points

    /// @brief Calculate the four points to preserve region of interest
    /// @param midpoint_x image midpoint at x-axis
    /// @param midpoint_y image midpoint at y-axis
    /// @return Array of points
    ROIPoints calcPoints(const int &midpoint_x, const int &midpoint_y, const int &height);
};

#endif // FILTERS_H
