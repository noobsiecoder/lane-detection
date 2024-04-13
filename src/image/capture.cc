#include <image/capture.h>
#include <custom_exception.h>

namespace fs = std::filesystem;

bool Capture::checkPathExists() const
{
    std::ifstream fs(file_path_);
    return fs.good();
}

std::vector<cv::Mat> Capture::loadImageFiles() const
{
    // Check directory path exists

    if (!checkPathExists())
    {
        std::string err = "\'" + file_path_ + "\'" + " not found";
        throw PathNotFoundException(err);
    }
    else
    {
        std::vector<cv::Mat> images;
        std::vector<std::string> png_files;

        // Check for '.png' files in the directory
        // Push file names into a vector

        for (const auto &file : fs::directory_iterator(file_path_))
        {
            if (file.path().extension() == ".png")
            {
                png_files.push_back(file.path().string());
            }
        }

        // Sort file names in ascending order
        // Load image file into cv::Mat data object
        // Push cv::Mat data into a vector

        std::sort(png_files.begin(), png_files.end()); //! Check for any alternative other than sorting
        for (const auto &file : png_files)
        {
            cv::Mat image = cv::imread(file);
            images.push_back(image);
        }

        return images;
    }
}

void Capture::stitchToVideo(const std::vector<cv::Mat> &images, const std::string &file_name, const bool &isColor) const
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

    cv::VideoWriter video("samples/" + file_name + ".avi", codec, 10.36, images[0].size(), isColor);

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
