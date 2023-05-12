// This code shows how to use OpenCV to create a swirl warp effect on an input image using a transformation formula.
//clang++ -fopenmp -I/usr/local/include/opencv4 -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs warp-2.0.cpp -o bin/warp_image-2.0
//./warp_image lena.png 0.01
#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>
#include <chrono>
#include <omp.h>

using namespace cv;
using namespace std;
using namespace std::chrono;

// Interpolate the pixel value from the input image at the given x and y coordinates using bilinear interpolation
Vec3b BilinearlyInterpolate(Mat inputImage, float x, float y) {
    float x1 = floor(x);
    float y1 = floor(y);
    float x2 = x1 + 1.0f;
    float y2 = y1 + 1.0f;
    float alpha = x - x1;
    float beta = y - y1;

    Vec3b intensity1 = inputImage.at<Vec3b>(y1, x1) * (1 - alpha) + inputImage.at<Vec3b>(y1, x2) * alpha;
    Vec3b intensity2 = inputImage.at<Vec3b>(y2, x1) * (1 - alpha) + inputImage.at<Vec3b>(y2, x2) * alpha;
    
    Vec3b interpolatedIntensity = intensity1 * (1 - beta) + intensity2 * beta;
    
    return interpolatedIntensity;
}

int main(int argc, char** argv) {
    
    // Check if the correct number of command-line arguments was provided
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <input_image_path> <twist_amount>" << endl;
        return -1;
    }

    // Read the input image
    Mat inputImage = imread(argv[1]);
    if (inputImage.empty()) {
        cout << "Could not read the image: " << argv[1] << endl;
        return -1;
    }

    // Define the center point for the swirl warp
    Point2f center(inputImage.cols/2.0f, inputImage.rows/2.0f);

    // Define the amount of twist for the swirl warp
    float twistAmount = atof(argv[2]);

    // Create a new image to hold the output
    Mat outputImage(inputImage.size(), inputImage.type());

    double start, end;
    start = omp_get_wtime();
    // Compute the transformation for each pixel in the output image
    #pragma omp parallel for shared(inputImage, outputImage, center)
    for (int y = 0; y < outputImage.rows; y++) {
        for (int x = 0; x < outputImage.cols; x++) {
            // Compute the distance and angle from the center point for this pixel
            float dx = x - center.x;
            float dy = y - center.y;
            float radius = sqrt(dx*dx + dy*dy);
            float angle = twistAmount * radius / 2.0f;
            // Compute the new x and y coordinates for this pixel
            float newX = (dx * cos(angle) + dy * sin(angle)) + center.x;
            float newY = (-dx * sin(angle) + dy * cos(angle)) + center.y;
            // Interpolate the pixel value from the input image at the new x and y coordinates using bilinear interpolation
            if (newX >= 0 && newX < outputImage.cols && newY >= 0 && newY < outputImage.rows)
                outputImage.at<Vec3b>(y, x) = BilinearlyInterpolate(inputImage, newX, newY);
        }
    }
    end = omp_get_wtime();
    printf("Tempo decorrido = %.16g segundos\n", end - start);

    // Save the output image
    imwrite("output/warped_image.png", outputImage);

    return 0;
}