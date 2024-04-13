#include <custom_exception.h>
#include <image/canny.h>
#include <image/capture.h>
#include <image/filter.h>
#include <image/hough.h>

typedef std::vector<cv::Mat> ImageSequence;

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

    ImageSequence images = capture.loadImageFiles();                                  // load image file into cv::Mat
    ImageSequence grayscale = filter.grayscale(images);                               // convert images to grayscale images
    ImageSequence roi = filter.regionOfInterest(grayscale);                           // crop the grayscale images by preserving the region of interest
    ImageSequence medianblur_images = filter.medianBlur(roi);                         // apply median blur on the image
    ImageSequence equalized_images = filter.histogramEqualization(medianblur_images); // apply histogram equalization on the image
    ImageSequence dilated_images = filter.dilate(equalized_images);                   // apply dilation on the image
    ImageSequence erosion_images = filter.erode(dilated_images);                      // apply erosion on the image

    CannyDetection canny_feature(erosion_images);
    ImageSequence canny_images = canny_feature.canny(); // extract canny edges

    HoughTransform hough_feature(canny_images);
    ImageSequence hough_lines = hough_feature.houghLines(roi); // extract hough lines
    capture.stitchToVideo(hough_lines, output_path, true);     // stitch cv::Mat's into a video

    return 0;
}
