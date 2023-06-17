#include "cartoon_plugin.h"

QString CartoonPlugin::name() const {
    return "Cartoon";
}

void CartoonPlugin::edit(const cv::Mat &input, cv::Mat &output)
{
    cv::Mat smooth_edge;
    smoothEdge(input, smooth_edge);

    cv::Mat image_edge;
    detectEdge(input, image_edge);

    output = smooth_edge & image_edge;
}

void CartoonPlugin::smoothEdge(const cv::Mat &input, cv::Mat &output)
{
    int num_down = 2;
    int num_bilateral = 7;

    cv::Mat copy1, copy2;
    copy1 = input.clone();

    // scale down copy1
    // bilateral filtering is much slower than other smoothing algorithm (for example, the Gaussian blur algorithm)
    // so we scale down the image before applying the filter.
    for (int i = 0; i < num_down; i++)
    {
        cv::pyrDown(copy1, copy2); // scale down
        copy1 = copy2.clone();
    }

    for (int i = 0; i < num_bilateral; i++)
    {
        cv::bilateralFilter(copy1, copy2, 9, 9, 7);
        copy1 = copy2.clone();
    }

    for (int i = 0; i < num_down; i++)
    {
        cv::pyrUp(copy1, copy2); // scale up to its original size
        copy1 = copy2.clone();
    }

    // cv::pyrDown(copy1, copy2) scale down result image size: `(copy1.cols + 1) / 2, (copy1.rows + 1) / 2`.
    // cv::pyrUp(copy1, copy2) scale up result image size: `copy1.cols * 2, copy1.rows * 2`
    // consequently, the size of the copy1 may not be the same as the original image.
    if (input.cols != copy1.cols || input.rows != copy1.rows)
    {
        cv::Rect rect(0, 0, input.cols, input.rows);
        copy1(rect).copyTo(copy2);
        copy1 = copy2;
    }

    output = copy1;
}

void CartoonPlugin::detectEdge(const cv::Mat &input, cv::Mat &output)
{
    cv::Mat image_gray;

    cv::cvtColor(input, image_gray, cv::COLOR_RGB2GRAY);
    // reduce noise
    // because adaptiveThreshold is prone to be influenced by noise.
    cv::medianBlur(image_gray, image_gray, 5);

    // detect the edges
    // image_gray becomes a binary image that only contains the edges.
    cv::adaptiveThreshold(image_gray, image_gray, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 9, 2);

    cv::cvtColor(image_gray, output, cv::COLOR_GRAY2RGB);
}
