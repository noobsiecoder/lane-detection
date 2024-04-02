#include <image_processor.h>
#include <custom_exception.h>

// constructor
ImageProcessor::ImageProcessor(const std::string &file) : file_(file) {}

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

// convert video object to grayscale
int ImageProcessor::grayscaleVideo()
{
    cv::Mat frame;
    while (capture_.read(frame))
    {
        cv::Mat grayFrame;
        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

        grayscale_.push_back(grayFrame);
    }

    if (grayscale_.empty())
    {
        return FAILURE;
    }

    return SUCCESS;
}

// apply median filter to smooth image
int ImageProcessor::medianFilter(const int &kernelSize)
{
    for (const auto &grayscaleFrame : grayscale_)
    {
        cv::Mat frame;
        cv::medianBlur(grayscaleFrame, frame, kernelSize);

        medianblur_.push_back(frame);
    }

    if (medianblur_.empty())
    {
        return FAILURE;
    }

    return SUCCESS;
}

// apply gaussian noise (remove salt and pepper noise)
int ImageProcessor::gaussianFilter(const cv::Size &kernelSize, const double &deviation)
{
    for (const auto &medianblurFrame : medianblur_)
    {
        cv::Mat frame;
        cv::GaussianBlur(medianblurFrame, frame, kernelSize, deviation);

        gaussianblur_.push_back(frame);
    }

    if (gaussianblur_.empty())
    {
        return FAILURE;
    }

    return SUCCESS;
}

// apply histogram equalization to imporve contrast and brightness
int ImageProcessor::histogramFilter()
{
    for (const auto &gaussianblurFrame : gaussianblur_)
    {
        cv::Mat frame;
        cv::equalizeHist(gaussianblurFrame, frame);

        histogrameq_.push_back(frame);
    }

    if (histogrameq_.empty())
    {
        return FAILURE;
    }

    return SUCCESS;
}

// apply dilation and erosion filter
int ImageProcessor::miscellaneousFilters(const int &kernelSize)
{
    for (const auto &histogramEqualizationFrames : histogrameq_)
    {
        cv::Mat dilatedFrame;
        cv::Mat frame;

        cv::dilate(histogramEqualizationFrames, dilatedFrame, kernelSize);
        cv::erode(dilatedFrame, frame, kernelSize);

        miscfilter_.push_back(frame);
    }

    if (miscfilter_.empty())
    {
        return FAILURE;
    }

    return SUCCESS;
}

// only for development purposes
//! DEV USE ONLY, DELETE LATER
std::vector<cv::Mat> ImageProcessor::getGrayscaleFrames() const { return grayscale_; }

//! DEV USE ONLY, DELETE LATER
cv::VideoCapture ImageProcessor::getCapturedFrames() const { return capture_; }

//! DEV USE ONLY, DELETE LATER
std::vector<cv::Mat> ImageProcessor::getMedianBlurFrames() const { return medianblur_; }

//! DEV USE ONLY, DELETE LATER
std::vector<cv::Mat> ImageProcessor::getGaussianBlurFrames() const { return gaussianblur_; }

//! DEV USE ONLY, DELETE LATER
std::vector<cv::Mat> ImageProcessor::getHistogramFilteredFrames() const { return histogrameq_; }

//! DEV USE ONLY, DELETE LATER
std::vector<cv::Mat> ImageProcessor::getMiscellaneousFilteredFrames() const { return miscfilter_; }