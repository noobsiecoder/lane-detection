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
/// @brief This file contains all the definitions for image and video processing

#ifndef CAPTURE_H
#define CAPTURE_H

#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <custom_exception.h>
#include <custom_types.h>

#define FRAME_RATE 10.36
#define VIDEO_IS_COLOR false

/// @brief Class to perform image and video processing
class Capture
{
public:
    /// @brief Default constructor to store path
    /// @param path path containing KITTI image dataset files
    Capture(const std::string &path) : path_(path) {}

    /// @brief Load image files into an array
    /// @return array of images
    ImageSequence loadImageFiles() const;

    /// @brief Stitch image frames into a video
    /// @param images array of images
    /// @param file_name file name of the output video
    /// @param frame_rate frame rate of the video
    /// @param isColor boolean value to stitch frames into video in color if true
    void stitchToVideo(const ImageSequence &images, const std::string &file_name, const double &frame_rate = FRAME_RATE, const bool &isColor = VIDEO_IS_COLOR) const;

private:
    std::string path_; // store path

    /// @brief Check if path exists in directory /usr/..
    /// @return boolean value for the path
    bool checkPathExists() const;
};

#endif // CAPTURE_H
