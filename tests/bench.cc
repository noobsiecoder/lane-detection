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
/// @brief This file is solely for benchmarking

#include <iostream>
#include <chrono>
#include <sys/resource.h> // on Linux
#include <capture.h>
#include <custom_exception.h>
#include <edge_features.h>
#include <filters.h>

#define FILE_PATH "data/drive_0002_sync/image_02/data"

int main()
{
    // Start the clock
    auto start = std::chrono::high_resolution_clock::now();

    // Your code for lane detection or any other processing
    // Replace this with your actual code
    // Main block to perform lane detection
    Capture capture(FILE_PATH);
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
    // capture.stitchToVideo(lane_detected_images, output_path, true);                                   // stitch cv::Mat's into a video

    // End the clock
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time
    std::chrono::duration<double> elapsed = end - start;

    // Print the elapsed time
    std::cout << "Processing time: " << elapsed.count() << " seconds" << std::endl;

    // Get memory usage
    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    std::cout << "Memory usage: " << r_usage.ru_maxrss << " kilobytes" << std::endl;


    return 0;
}
