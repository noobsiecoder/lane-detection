#include <gtest/gtest.h>
#include <image_processor.h>
#include <custom_exception.h>

// check if the image has only one channel
bool isGrayscale(const cv::Mat &image)
{
    return image.channels() == 1;
}

TEST(ImageProcessing, CHECK_FILE)
{
    //? Intent:
    //? Check if capture_ is loaded when file exist
    ImageProcessor image1("./data/video/video1.mp4");
    ASSERT_EQ(image1.loadVideo(), 1); // PASS

    //? Intent:
    //? Check if loadVideo() returns 0 when capture_ is empty
    //! UNCOMMENTING MAY PRODUCE UNEXPECTED RESPONSE
    // ASSERT_NE(image1.loadVideo(), 1); // PASS

    //? Intent:
    //? Check if FileNotFoundException is thrown when file does not exist
    ImageProcessor image2("/data/video/video1.mp4");
    ASSERT_THROW(image2.loadVideo(), FileNotFoundException); // PASS
}

TEST(ImageProcessing, GRAYSCALE)
{
    //? Intent:
    //? Check if grayscale object is loaded
    ImageProcessor image("./data/video/video1.mp4");
    image.loadVideo();
    ASSERT_EQ(image.grayscaleVideo(), 1); // PASS

    //? Intent:
    //? Check if grayscaleVideo() returns 0 when grayscale_ is empty
    // ASSERT_NE(image.grayscaleVideo(), 1); // PASS

    //? Intent:
    //? Check if grayscale frames have only one channel (intensity)
    //! DEV PURPOSE ONLY, COMMENT LATER
    std::vector<cv::Mat> grayscaleFrames = image.getGrayscaleFrames();
    for (const auto &frame : grayscaleFrames)
    {
        ASSERT_TRUE(isGrayscale(frame));
    } // PASS

    //? Intent:
    //? Check if captured frames in RGB (3 channels) are not in grayscale
    //! DEV PURPOSE ONLY, COMMENT LATER
    cv::VideoCapture capturedFrames = image.getCapturedFrames();
    cv::Mat frame;
    while (capturedFrames.read(frame))
    {
        ASSERT_FALSE(isGrayscale(frame));
    } // PASS
}

TEST(ImageProcessing, MEDIAN_BLUR)
{
    //? Intent:
    //? Check if grayscale object is loaded
    ImageProcessor image("./data/video/video1.mp4");
    image.loadVideo();
    image.grayscaleVideo();
    ASSERT_EQ(image.medianFilter(), 1); // PASS

    //? Intent:
    //? Check if medianFilter() returns 0 when grayscale_ is empty
    // ASSERT_NE(image.grayscaleVideo(), 1); // PASS
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}