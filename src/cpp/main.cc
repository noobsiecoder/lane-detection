#include <image_processor.h>
#include <type_traits>

template <typename T>
void playVideo(T &source)
{
    // variables to calculate framerate
    int num_frames = 0;
    auto start_time = std::chrono::steady_clock::now();
    
    // Process frame from cv::VideoCapture
    if constexpr (std::is_same<typename std::remove_reference<T>::type, cv::VideoCapture>::value)
    {
        cv::Mat frame;
        while (source.read(frame))
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
            cv::putText(frame, text.str(), cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);

            // display player
            cv::imshow("Video", frame);

            // quit if user inputs 'q'
            if (cv::waitKey(30) == 'q')
            {
                break;
            }
        }
    }
    // Process frame from std::vector<cv::Mat>
    else if constexpr (std::is_same<typename std::remove_reference<T>::type, std::vector<cv::Mat>>::value)
    {
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
            cv::putText(frame, text.str(), cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);

            // display player
            cv::imshow("Video", frame);

            // quit if user inputs 'q'
            if (cv::waitKey(30) == 'q')
            {
                break;
            }
        }
    }
    else
    {
        std::cerr << "Unsupported type: " << typeid(T).name() << std::endl;
    }
}

int main(int argc, char **argv)
{
    const std::string &file = "./data/video/video1.mp4";
    ImageProcessor image(file);
    image.loadVideo();

    // cv::VideoCapture capture = image.getCapturedFrames();
    // playVideo(capture);

    image.grayscaleVideo();
    // std::vector<cv::Mat> grayscale = image.getGrayscaleFrames();
    // playVideo(grayscale);

    image.medianFilter();
    // std::vector<cv::Mat> medianBlur = image.getMedianBlurFrames();
    // playVideo(medianBlur);

    image.gaussianFilter();
    // std::vector<cv::Mat> gaussianFilter = image.getGaussianBlurFrames();
    // playVideo(gaussianFilter);

    image.histogramFilter();
    // std::vector<cv::Mat> histogramFilter = image.getHistogramFilteredFrames();
    // playVideo(histogramFilter);

    image.miscellaneousFilters();
    std::vector<cv::Mat> miscellaneousFilter = image.getMiscellaneousFilteredFrames();

    playVideo(miscellaneousFilter);

    // image.~ImageProcessor();
    // capture.release();
    cv::destroyAllWindows();
    return 0;
}