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
    //? Check if grayscale frames have only one channel (intensity)
    ImageProcessor image("./data/video/video1.mp4");
    std::vector<cv::Mat> frames_ = image.loadVideoToVector();
    std::vector<cv::Mat> grayscale_frames_ = image.grayscale(frames_);
    for (const auto &frame : grayscale_frames_)
    {
        ASSERT_TRUE(isGrayscale(frame));
    } // PASS

    //? Intent:
    //? Check if captured frames in RGB (3 channels) are not in grayscale
    //! DEV PURPOSE ONLY, COMMENT LATER
    for (const auto &frame : frames_)
    {
        ASSERT_FALSE(isGrayscale(frame));
    } // PASS
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}