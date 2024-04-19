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
/// @brief This file contains all the declaration for image and video processing

#include <capture.h>

namespace fs = std::filesystem;

/// @brief Check if path exists in /usr
/// @return a boolean value for path exists
bool Capture::checkPathExists() const
{
    std::ifstream fs(path_);
    return fs.good();
}

/// @brief Load image files into an array
/// @return array of images
ImageSequence Capture::loadImageFiles() const
{
    // Check directory path exists
    if (!checkPathExists())
    {
        std::string err = "\'" + path_ + "\'" + " not found";
        throw PathNotFoundException(err);
    }
    else
    {
        ImageSequence images;
        std::vector<std::string> png_files;

        // Check for '.png' files in the directory
        // Push file names into a vector
        for (const auto &file : fs::directory_iterator(path_))
        {
            if (file.path().extension() == ".png")
            {
                png_files.push_back(file.path().string());
            }
        }

        // Sort file names in ascending order
        // Load image file into cv::Mat data object
        // Push cv::Mat data into a vector
        std::sort(png_files.begin(), png_files.end());
        for (const auto &file : png_files)
        {
            cv::Mat image = cv::imread(file);
            images.push_back(image);
        }

        return images;
    }
}

/// @brief Stitch frames into a video
/// @param images array of images
/// @param file_name name of the output video file
/// @param frame_rate frame rate of the video
/// @param isColor boolean value to stitch frames into video in color if true
void Capture::stitchToVideo(const ImageSequence &images, const std::string &file_name, const double &frame_rate, const bool &isColor) const
{
    // Check if there are any images
    if (images.empty())
    {
        std::string err = "No images found";
        throw CaptureEmptyException(err);
    }

    // Define the codec and create a VideoWriter object
    int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');

    // codec    :   four code for video formatting
    // fps      :   check numebr of frames and total seconds
    // size     :   pixel resolution
    // isColor  :   image channel
    cv::VideoWriter video("samples/" + file_name + ".avi", codec, frame_rate, images[0].size(), isColor);

    // Check if the VideoWriter object is opened successfully
    if (!video.isOpened())
    {
        std::string err = "Could not open the output video file for writing";
        throw CaptureOpenedException(err);
    }

    // Write images to the video
    for (const auto &image : images)
    {
        video.write(image);
    }

    // Release the VideoWriter object
    video.release();
}
