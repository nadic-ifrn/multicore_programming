#include <stdio.h>
#include <opencv2/opencv.hpp>

int main()
{
    cv::Mat image = cv::imread("image.png");
    cv::Mat gray_image(image.size(), CV_8UC1);
    
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);
            gray_image.at<uchar>(i, j) = 0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0];
        }
    }
    
    imwrite("grayscale_image.png", gray_image);
    
    return 0;
}
