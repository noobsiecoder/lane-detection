#include <gtest/gtest.h>
#include <capture.h>
#include <custom_exception.h>

#define INCORRECT_FILE_PATH "data/drive_xxx1_sync/data/"
#define CORRECT_FILE_PATH "data/drive_0042_sync/image_00/data/"

// check if the image has only one channel
bool isGrayscale(const cv::Mat &image)
{
    return image.channels() == 1;
}

TEST(Filter, CHECK_FILE_EXISTS)
{
    //? Intent:
    //? Throw PathNotFoundException exception as path does not exist

    Capture capture(INCORRECT_FILE_PATH);
    ASSERT_THROW(capture.loadImageFiles(), PathNotFoundException);
}

// TEST(Filter, GRAYSCALE)
// {
//     //? Intent:
//     //? Convert to grayscale

//     Capture capture(CORRECT_FILE_PATH);
//     Filters filter;

//     std::vector<cv::Mat> images = capture.loadImageFiles();
//     std::vector<cv::Mat> grayscale = filter.grayscale(images);

//     //? Should fail as the cv::Mat has 3 color channels

//     for (const auto &image : images)
//     {
//         ASSERT_FALSE(isGrayscale(image));
//     }

//     //? cv::Mat has 1 channel (should pass)

//     for (const auto &image : grayscale)
//     {
//         ASSERT_TRUE(isGrayscale(image));
//     }
// }

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}