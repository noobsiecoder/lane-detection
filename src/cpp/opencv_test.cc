#include <opencv2/opencv.hpp>
#include <sstream>
#include <chrono>

int main(int argc, char **argv)
{
    // filename
    const std::string VideoFileName = "./data/video/video1.mp4";

    // create a video object from the video file
    cv::VideoCapture cap(VideoFileName);

    // open/play video file
    if (!cap.isOpened())
    {
        std::cerr << "Error: unable to open video file!" << std::endl;
    }

    // player window
    cv::namedWindow("Video", cv::WINDOW_NORMAL);

    // variables to calculate framerate
    int num_frames = 0;
    auto start_time = std::chrono::steady_clock::now();
    // double fps = cap.get(cv::CAP_PROP_FPS);

    // loop to read frames from the video file
    while (true)
    {
        cv::Mat frame;
        // check if frame is readable
        if (!cap.read(frame))
        {
            std::cerr << "Error: Unable to read frame from the video" << std::endl;
        }

        // convertion to grayscale
        cv::Mat grayscale;
        cv::cvtColor(frame, grayscale, cv::COLOR_BGR2GRAY);
        // check if frame is empty
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
        cv::putText(grayscale, text.str(), cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);

        // display player
        cv::imshow("Video", grayscale);

        // quit if user inputs 'q'
        if (cv::waitKey(30) == 'q')
        {
            break;
        }
    }

    // release the videocapture object
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
