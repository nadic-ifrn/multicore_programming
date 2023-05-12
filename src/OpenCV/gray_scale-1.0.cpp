//clang++ -fopenmp -I/usr/local/include/opencv4 -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs gray_scale-1.0.cpp -o bin/gray_scale-1.0

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include<omp.h>
int main()
{
    cv::Mat image = cv::imread("input/lena.png");
    cv::Mat gray_image(image.size(), CV_8UC1);
    
    double start, end;
    start = omp_get_wtime();
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);
            gray_image.at<uchar>(i, j) = 0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0];
        }
    }
    end = omp_get_wtime();
    printf("Tempo decorrido = %.16g segundos\n", end - start);

    imwrite("output/grayscale_image.png", gray_image);
    
    return 0;
}
