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
/// @brief This file contains all the declarations for the edge feature implementations

#include <edge_features.h>

/// @brief Detect edges using Canny edge detection. To learn more, visit: https://docs.opencv.org/4.x/da/d22/tutorial_py_canny.html
/// @param images array of images
/// @param l_thresh lowest threshold value
/// @param h_thresh highest threshold value
/// @param aperture Sobel kernel size
/// @return array of images with detected edges
ImageSequence Features::edgeDetection(const ImageSequence &images, const int &l_thresh, const int &h_thresh, const int &aperture)
{
    ImageSequence edge_features_images;
    for (const auto &image : images)
    {
        cv::Mat edge_features_image;
        cv::Canny(image, edge_features_image, l_thresh, h_thresh, aperture); // extract canny edges from the image
        edge_features_images.push_back(edge_features_image);
    }

    return edge_features_images;
}

/// @brief Get probabilistic hough lines from the canny edge detected image. To learn more, visit: https://docs.opencv.org/3.4/d9/db0/tutorial_hough_lines.html
/// @param images array of images
/// @param rho resolution of the parameter r in pixels
/// @param theta resolution of the parameter θ in radians
/// @param threshold minimum number of intersections (or bins) to detect a line
/// @param min_length minimum number of points that can form a line
/// @param max_length maximum gap between two points to be considered in the same line
/// @return array of line points in coordinate space
/// @note The values are in another coordinate space (projected onto another frame). It needs to be projected back onto the original image space
std::vector<CoordinatePoints> Features::probalisiticHoughLines(const ImageSequence &images, const double &rho, const double &theta, const int &threshold, const double &min_length, const double &max_length)
{
    std::vector<CoordinatePoints> p_hough_lines;
    for (const auto &frame : images)
    {
        std::vector<cv::Vec4i> lines;
        cv::HoughLinesP(frame, lines, rho, theta, threshold, min_length, max_length);
        p_hough_lines.push_back(lines);
    }

    return p_hough_lines;
}