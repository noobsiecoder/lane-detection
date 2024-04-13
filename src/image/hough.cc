#include <image/hough.h>
#include <custom_exception.h>

std::vector<cv::Mat> HoughTransform::houghLines(const std::vector<cv::Mat> &frames, const double &rho, const double &theta, const int &threshold, const double &srn, const double &stn, const double &min_theta, const double &max_theta, const cv::Scalar &color, const int &thickness, const int &line_type, const int &shift)
{
    std::vector<cv::Mat> hough_images;

    // rho – Distance resolution of the accumulator in pixels.
    // theta – Angle resolution of the accumulator in radians.
    // threshold – Accumulator threshold parameter. Only those lines are returned that get enough votes (
    // srn – For the multi-scale Hough transform, it is a divisor for the distance resolution rho . The coarse accumulator distance resolution is rho and the accurate accumulator resolution is rho/srn . If both srn=0 and stn=0 , the classical Hough transform is used. Otherwise, both these parameters should be positive.
    // stn – For the multi-scale Hough transform, it is a divisor for the distance resolution theta.
    // min_theta – For standard and multi-scale Hough transform, minimum angle to check for lines. Must fall between 0 and max_theta.
    // max_theta – For standard and multi-scale Hough transform, maximum angle to check for lines. Must fall between min_theta and CV_PI.

    for (const auto &frame : images_)
    {
        cv::Mat color_frame;
        cv::cvtColor(frame, color_frame, cv::COLOR_GRAY2BGR);

        std::vector<cv::Vec2f> lines;
        cv::HoughLines(frame, lines, rho, theta, threshold, srn, stn, min_theta, max_theta);
        for (size_t i = 0; i < lines.size(); i++)
        {
            double rho = lines[i][0], theta = lines[i][1];
            cv::Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a * rho, y0 = b * rho;
            pt1.x = cvRound(x0 + 250 * (b));
            pt1.y = cvRound(y0 + 250 * (-a));
            pt2.x = cvRound(x0 - 250 * (b));
            pt2.y = cvRound(y0 - 250 * (-a));
            line(color_frame, pt1, pt2, color, thickness, line_type, shift);
        }

        hough_images.push_back(color_frame);
    }

    if (hough_images.empty())
    {
        std::string err = "No images found while extracting features";
        throw CaptureEmptyException(err);
    }

    return hough_images;
}