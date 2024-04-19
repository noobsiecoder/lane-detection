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
/// @brief This file contains all the definitions for the edge feature implementations

#ifndef FEATURES_H
#define FEATURES_H

#include <custom_types.h>
#include <opencv2/imgproc.hpp>

#define CANNY_LOWER_THRESHOLD 75
#define CANNY_HIGHER_THRESHOLD 175
#define CANNY_APERTURE 3

#define P_HOUGH_RHO 1
#define P_HOUGH_THETA (CV_PI / 180)
#define P_HOUGH_THRESHOLD 25
#define P_HOUGH_MIN_LINE_LENGTH 100.0
#define P_HOUGH_MAX_LINE_LENGTH 90.0

/// @brief Class to extract feature from a sequence of images
class Features
{
public:
    /// @brief Detect edges using Canny edge detection. To learn more, visit: https://docs.opencv.org/4.x/da/d22/tutorial_py_canny.html
    /// @param images array of images
    /// @param l_thresh lowest threshold value
    /// @param h_thresh highest threshold value
    /// @param aperture Sobel kernel size
    /// @return array of images with detected edges
    ImageSequence edgeDetection(const ImageSequence &images, const int &l_thresh = CANNY_LOWER_THRESHOLD, const int &h_thresh = CANNY_HIGHER_THRESHOLD, const int &aperture = CANNY_APERTURE);

    /// @brief Get probabilistic hough lines from the canny edge detected image. To learn more, visit: https://docs.opencv.org/3.4/d9/db0/tutorial_hough_lines.html
    /// @param images array of images
    /// @param rho resolution of the parameter r in pixels
    /// @param theta resolution of the parameter θ in radians
    /// @param threshold minimum number of intersections (or bins) to detect a line
    /// @param min_length minimum number of points that can form a line
    /// @param max_length maximum gap between two points to be considered in the same line
    /// @return array of line points in coordinate space
    /// @note The values are in another coordinate space (projected onto another frame). It needs to be projected back onto the original image space
    std::vector<CoordinatePoints> probalisiticHoughLines(const ImageSequence &images, const double &rho = P_HOUGH_RHO, const double &theta = P_HOUGH_THETA, const int &threshold = P_HOUGH_THRESHOLD, const double &min_length = P_HOUGH_MIN_LINE_LENGTH, const double &max_length = P_HOUGH_MAX_LINE_LENGTH);

private:
};

#endif // FEATURES_H
