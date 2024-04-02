#include <image_processor.h>
#include <lane_detection.h>
#include <type_traits>

void playVideo(std::vector<cv::Mat> &source)
{
    // variables to calculate framerate
    int num_frames = 0;
    auto start_time = std::chrono::steady_clock::now();

    for (const auto &frame : source)
    {
        if (frame.empty())
        {
            std::cerr << "End of Video" << std::endl;
            break;
        }

        // calculate framerate
        num_frames++;
        auto end_time = std::chrono::steady_clock::now();
        auto elasped_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() / 1000.0;
        double fps = num_frames / elasped_time;

        // display text
        std::stringstream text;
        text << fps;
        cv::putText(frame, text.str(), cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 2);

        // display player
        cv::imshow("Video", frame);

        // quit if user inputs 'q'
        if (cv::waitKey(30) == 'q')
        {
            break;
        }
    }
}

void videoWriter(const std::vector<cv::Mat> &frames)
{
    // Get frame dimensions from the first frame
    int frameWidth = frames[0].cols;
    int frameHeight = frames[0].rows;

    // Create VideoWriter object to save the frames to a video file
    cv::VideoWriter outputVideo("output_video.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 25, cv::Size(frameWidth, frameHeight));
    if (!outputVideo.isOpened())
    {
        std::cerr << "Error: Could not open output video file for writing." << std::endl;
    }

    // Write each frame to the output video file
    for (const auto &frame : frames)
    {
        outputVideo.write(frame);
    }

    // Release the VideoWriter object
    outputVideo.release();
}

int main(int argc, char **argv)
{
    const std::string &file = "./data/video/video1.mp4";
    ImageProcessor image(file);
    LaneDetection frames;

    std::vector<cv::Mat>
        frames_ = image.loadVideoToVector();
    std::vector<cv::Mat> grayscale_frames_ = image.grayscale(frames_);
    // playVideo(grayscale_frames_);

    std::vector<cv::Mat> medianblur_frames_ = image.medianFilter(grayscale_frames_);
    // playVideo(medianblur_frames_);

    std::vector<cv::Mat> gaussianblur_frames_ = image.gaussianFilter(medianblur_frames_);
    // playVideo(gaussianblur_frames_);

    std::vector<cv::Mat> histeq_frames_ = image.histogramFilter(gaussianblur_frames_);
    // playVideo(histeq_frames_);

    std::vector<cv::Mat> dilate_frames_ = image.dilate(histeq_frames_);
    // playVideo(dilate_frames_);

    std::vector<cv::Mat> erosion_frames_ = image.erosion(dilate_frames_);
    // playVideo(erosion_frames_);

    std::vector<cv::Mat> sobel_frames_ = frames.sobelFilter(erosion_frames_);
    // playVideo(sobel_frames_);

    std::vector<cv::Mat> canny_frames_ = frames.cannyFilter(sobel_frames_);
    // playVideo(canny_frames_);

    std::vector<cv::Mat> hough_frames_ = frames.houghTransform(frames_, canny_frames_);
    // playVideo(hough_frames_);
    videoWriter(hough_frames_);

    // image.~ImageProcessor();
    // capture.release();
    cv::destroyAllWindows();
    return 0;
}