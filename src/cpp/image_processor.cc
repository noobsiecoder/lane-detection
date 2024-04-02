#include <image_processor.h>
#include <custom_exception.h>

// constructor
ImageProcessor::ImageProcessor(const std::string &file) : file_(file), error_("Error: ") {}

// destructor
ImageProcessor::~ImageProcessor()
{
    releaseCapture();
}

// check file exists
bool ImageProcessor::fileExists() const
{
    std::ifstream fs(file_);
    return fs.good();
}

// release captured video frame
void ImageProcessor::releaseCapture()
{
    capture_.release();
}

// load video
int ImageProcessor::loadVideo()
{
    if (!fileExists())
    {
        std::string message_ = "Error: \'" + file_ + "\' not found!";
        throw FileNotFoundException(message_);
    }
    else
    {
        capture_.open(file_); // load file into object
        if (!capture_.isOpened())
        {
            return FAILURE; // object is empty
        }

        return SUCCESS; // capture object loaded
    }
}

// load video to std::vector
std::vector<cv::Mat> ImageProcessor::loadVideoToVector()
{
    if (!fileExists())
    {
        error_ += "\'" + file_ + "\' not found!";
        throw FileNotFoundException(error_);
    }
    else
    {
        capture_.open(file_); // load file into object
        if (!capture_.isOpened())
        {
            error_ += "Capture object is open!";
            throw CaptureOpenedException(error_);
        }

        std::vector<cv::Mat> frames_;
        cv::Mat frame;
        while (capture_.read(frame))
        {
            frames_.push_back(frame.clone());
        }

        if (frames_.empty())
        {
            error_ += "Capture object is empty!";
            throw CaptureEmptyException(error_);
        }

        return frames_;
    }
}

// convert video object to grayscale
std::vector<cv::Mat> ImageProcessor::grayscale(const std::vector<cv::Mat> &frames)
{
    std::vector<cv::Mat> frames_;
    for (const auto &frame : frames)
    {
        cv::Mat grayscaleFrame;
        cv::cvtColor(frame, grayscaleFrame, cv::COLOR_BGR2GRAY);

        frames_.push_back(grayscaleFrame.clone());
    }

    if (frames_.empty())
    {
        error_ += "Capture object is empty!";
        throw CaptureEmptyException(error_);
    }

    return frames_;
}

// apply median filter to smooth image
std::vector<cv::Mat> ImageProcessor::medianFilter(const std::vector<cv::Mat> &frames, const int &kernelSize)
{
    std::vector<cv::Mat> frames_;
    for (const auto &frame : frames)
    {
        cv::Mat blurFrame;
        cv::medianBlur(frame, blurFrame, kernelSize);

        frames_.push_back(blurFrame);
    }

    if (frames_.empty())
    {
        error_ += "Capture object is empty!";
        throw CaptureEmptyException(error_);
    }

    return frames_;
}

// apply gaussian noise (remove salt and pepper noise)
std::vector<cv::Mat> ImageProcessor::gaussianFilter(const std::vector<cv::Mat> &frames, const cv::Size &kernelSize, const double &deviation)
{
    std::vector<cv::Mat> frames_;
    for (const auto &frame : frames)
    {
        cv::Mat gaussianFrame;
        cv::GaussianBlur(frame, gaussianFrame, kernelSize, deviation);

        frames_.push_back(gaussianFrame);
    }

    if (frames_.empty())
    {
        error_ += "Capture object is empty!";
        throw CaptureEmptyException(error_);
    }

    return frames_;
}

// apply histogram equalization to imporve contrast and brightness
std::vector<cv::Mat> ImageProcessor::histogramFilter(const std::vector<cv::Mat> &frames)
{
    std::vector<cv::Mat> frames_;
    for (const auto &frame : frames)
    {
        cv::Mat histogramEqFrame;
        cv::equalizeHist(frame, histogramEqFrame);

        frames_.push_back(histogramEqFrame);
    }

    if (frames_.empty())
    {
        error_ += "Capture object is empty!";
        throw CaptureEmptyException(error_);
    }

    return frames_;
}

// apply dilation filter
std::vector<cv::Mat> ImageProcessor::dilate(const std::vector<cv::Mat> &frames, const int &kernelSize)
{
    std::vector<cv::Mat> frames_;
    for (const auto &frame : frames)
    {
        cv::Mat dilateFrame;
        cv::dilate(frame, dilateFrame, kernelSize);

        frames_.push_back(dilateFrame);
    }

    if (frames_.empty())
    {
        error_ += "Capture object is empty!";
        throw CaptureEmptyException(error_);
    }

    return frames_;
}

// apply erosion filter
std::vector<cv::Mat> ImageProcessor::erosion(const std::vector<cv::Mat> &frames, const int &kernelSize)
{
    std::vector<cv::Mat> frames_;
    for (const auto &frame : frames)
    {
        cv::Mat erosionFrame;
        cv::erode(frame, erosionFrame, kernelSize);

        frames_.push_back(erosionFrame);
    }

    if (frames_.empty())
    {
        error_ += "Capture object is empty!";
        throw CaptureEmptyException(error_);
    }

    return frames_;
}
