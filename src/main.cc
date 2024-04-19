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
/// @brief This is the main cpp file

#include <capture.h>
#include <custom_exception.h>
#include <edge_features.h>
#include <filters.h>

int main(int argc, char **argv)
{
    // Check if 3 arguments are passed
    if (argc != 3)
    {
        std::string err = "Arguments not found";
        throw ArgsNotFoundException(err);
    }

    // Parse command-line arguments
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        args.push_back(arg);
    }

    // Process parsed arguments
    std::string file_path;
    std::string output_path;
    for (const auto &arg : args)
    {
        if (arg.find("--file=") == 0)
        {
            file_path = arg.substr(7); // Extract path after '--file='
        }
        else if (arg.find("--output=") == 0)
        {
            output_path = arg.substr(9); // Extract path after '--output='
        }
    }

    // Check if both file path and output path are provided
    if (file_path.empty() || output_path.empty())
    {
        std::string err = "Both --file and --output arguments are required";
        throw ArgsNotFoundException(err);
    }

    // Main block to perform lane detection
    Capture capture(file_path);
    Filters filter;
    Features feature;

    ImageSequence images = capture.loadImageFiles();                                                  // load image file into cv::Mat
    ImageSequence roi_images = filter.roi(images);                                                    // region of interest
    ImageSequence grayscale_images = filter.convertColor(roi_images, cv::COLOR_BGR2GRAY);             // convert images to grayscale images
    ImageSequence bilateral_filtered_images = filter.bilateralBlur(grayscale_images);                 // apply bilateral filter
    ImageSequence perspective_images = filter.perspectiveMapping(bilateral_filtered_images);          // perspective mapping to view the road ahead in bird's eye
    ImageSequence edge_features_images = feature.edgeDetection(perspective_images);                   // detect edge features using Canny
    std::vector<CoordinatePoints> hough_lines = feature.probalisiticHoughLines(edge_features_images); // get probablistic hough lines
    ImageSequence lane_detected_images = filter.deWarpMapping(images, hough_lines);                   // plot detected lines on the orignal image
    capture.stitchToVideo(lane_detected_images, output_path, true);                                   // stitch cv::Mat's into a video

    return 0;
}
