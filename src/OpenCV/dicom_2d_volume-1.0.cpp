//clang++ -fopenmp -I/usr/local/include/opencv4 -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs -I/usr/local/include -I/usr/local/include/nifti  -ldcmimgle -ldcmdata -loflog -lofstd -lopencv_viz -lniftiio dicom_2d_volume-1.0.cpp -o bin/dicom_2d_volume-1.0
//./bin/dicom_2d_volume-1.0 input/dicom_slices_pat01 201 output/dicom_pat01.png
#include <dcmtk/dcmimgle/dcmimage.h>
#include <opencv2/opencv.hpp>
#include <opencv2/viz.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <dirent.h>
#include <chrono>
#include <omp.h>
#include <nifti/nifti1_io.h>

using namespace cv;
using namespace std;
using namespace std::chrono;

bool save_volume_as_nifti(const Mat& volume, const string& output_file_path) {
    // Create a new NIfTI image
    nifti_image *nifti_img = nifti_simple_init_nim();
    nifti_img->nx = volume.cols;
    nifti_img->ny = volume.rows;
    nifti_img->nz = volume.channels();
    nifti_img->nvox = nifti_img->nx * nifti_img->ny * nifti_img->nz;
    nifti_img->nbyper = sizeof(short);
    nifti_img->datatype = NIFTI_TYPE_INT16;
    nifti_img->fname = nifti_makehdrname(output_file_path.c_str(), nifti_img->nifti_type, 0, 0);
    nifti_img->iname = nifti_makeimgname(output_file_path.c_str(), nifti_img->nifti_type, 0, 0);

    // Allocate memory for the NIfTI data and copy the volume data
    nifti_img->data = malloc(nifti_img->nvox * nifti_img->nbyper);
    memcpy(nifti_img->data, volume.data, nifti_img->nvox * nifti_img->nbyper);

    // Save the NIfTI image
    nifti_image_write(nifti_img);

    // Free the NIfTI image
    nifti_image_free(nifti_img);

    return true;
}

void plot_3d_volume(Mat volume, const string& output_file_path) {
       // Create 3D window
    viz::Viz3d window("3D Volume");

    // Create 3D volume from data
    viz::WCloud cloud(volume, viz::Color::white());
    cloud.setRenderingProperty(viz::POINT_SIZE, 2);

    // Add volume to window
    window.showWidget("Volume", cloud);

    // Set camera position and orientation
    window.setViewerPose(viz::makeCameraPose(Vec3f(0, 0, -1), Vec3f(0, 0, 0), Vec3f(0, -1, 0)));

    imwrite(output_file_path, window.getScreenshot());
    
    // Start 3D window
    window.spin();
}

 // Function to list all DICOM files in a folder
vector<string> list_dicom_files(const string& folder_path, int num_slices) {
    vector<string> file_paths;

    DIR* folder = opendir(folder_path.c_str());
    if (folder == NULL) {
        cerr << "Error: cannot open folder " << folder_path << endl;
        return file_paths;
    }

    for (int i = 0; i < num_slices+1; ++i) {
        stringstream ss;
        ss << setw(4) << setfill('0') << i; // add leading zeros
        string file_name = "D" + ss.str() + ".dcm";
        string file_path = folder_path + "/" + file_name;

        if (access(file_path.c_str(), F_OK) != -1) {
            file_paths.push_back(file_path);
        }
    }
    sort(file_paths.begin(), file_paths.end());

    closedir(folder);

    return file_paths;
}

Mat convert_dicom_slice_to_cv_mat(const string& dicom_file_path) {
    // Load DICOM image using DCMTK
    DicomImage* dicom_image = new DicomImage(dicom_file_path.c_str());

    if (dicom_image->getStatus() != EIS_Normal) {
        cerr << "Error: cannot load DICOM image (" << dicom_image->getStatus() << ")" << endl;
        return Mat();
    }
    dicom_image->setMinMaxWindow();
    // Get image data
    const Uint16* pixel_data = (const Uint16*)dicom_image->getOutputData(16 /* bits per pixel */, 0 /* planar configuration */);

    // Get image size
    const int rows = dicom_image->getHeight();
    const int cols = dicom_image->getWidth();

    // Create OpenCV Mat from pixel data
    Mat cv_mat(rows, cols, CV_16UC1);
    memcpy(cv_mat.data, pixel_data, rows*cols*sizeof(Uint16));

    // Free DICOM image memory
    delete dicom_image;

    return cv_mat;
}

Mat create_dicom_volume_test2(const vector<string>& dicom_file_paths) {
    // Load first DICOM image to get image size
    Mat first_slice = convert_dicom_slice_to_cv_mat(dicom_file_paths[0]);
    const int rows = first_slice.rows;
    const int cols = first_slice.cols;

    // Create 3D volume
    vector<Mat> slices(dicom_file_paths.size());
    bool error_occurred = false;

    for (size_t i = 0; i < dicom_file_paths.size(); ++i) {
        cout << dicom_file_paths[i] << endl;
        Mat slice = convert_dicom_slice_to_cv_mat(dicom_file_paths[i]);

        if (slice.empty() || slice.rows != rows || slice.cols != cols) {
            cerr << "Error: invalid DICOM slice" << endl;
            return Mat();
        }

        // Normalize slice to 0-1 range
        normalize(slice, slice, 0, 1, NORM_MINMAX);

        // Convert the slice to CV_16UC1
        Mat channel;
        slice.convertTo(channel, CV_16UC1, 65535);

        // Assign the slice to the corresponding index in the slices vector
        slices[i] = channel;
    }

    // Merge the slices in a single img
    Mat volume;
    merge(slices, volume);

    return volume;
}

Mat create_dicom_volume(const vector<string>& dicom_file_paths) {
    // Load first DICOM image to get image size
    Mat first_slice = convert_dicom_slice_to_cv_mat(dicom_file_paths[0]);
    const int rows = first_slice.rows;
    const int cols = first_slice.cols;
    const int num_slices = dicom_file_paths.size();

    // Create a 2D volume with slices placed side by side
    Mat volume(rows, cols * num_slices, CV_16UC1, Scalar(0));

    for (size_t i = 0; i < num_slices; ++i) {
        Mat slice = convert_dicom_slice_to_cv_mat(dicom_file_paths[i]);

        if (slice.empty() || slice.rows != rows || slice.cols != cols) {
            cerr << "Error: invalid DICOM slice" << endl;
            return Mat();
        }

        // Normalize slice to 0-1 range
        normalize(slice, slice, 0, 1, NORM_MINMAX);

        // Convert the slice to CV_16UC1 format with a scale factor
        Mat channel;
        slice.convertTo(channel, CV_16UC1, 65535);

        // Place the channel in the corresponding position in the volume
        channel.copyTo(volume(Rect(i * cols, 0, cols, rows)));
    }

    cout << volume.size() << endl;
    cout << volume.channels() << endl;

    return volume;
}

Mat create_dicom_volume_test(const vector<string>& dicom_file_paths) {
    // Load first DICOM image to get image size
    Mat first_slice = convert_dicom_slice_to_cv_mat(dicom_file_paths[0]);
    const int rows = first_slice.rows;
    const int cols = first_slice.cols;

    // Create 3D volume
    Mat volume(rows, cols, CV_16UC1);
    for (size_t i = 0; i < dicom_file_paths.size(); ++i) {
        Mat slice = convert_dicom_slice_to_cv_mat(dicom_file_paths[i]);

        if (slice.empty() || slice.rows != rows || slice.cols != cols) {
            cerr << "Error: invalid DICOM slice" << endl;
            return Mat();
        }

        slice.convertTo(slice, CV_16UC1, 0.5, 32768);
        slice.copyTo(volume.row(i));


    }

    return volume;
}


int main(int argc, char** argv) {
    
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <path to DICOM folder> <number of slices> <output file path>" << endl;
        return EXIT_FAILURE;
    }
    // List DICOM files in folder
    vector<string> dicom_file_paths = list_dicom_files(argv[1],atoi(argv[2]));

    if (dicom_file_paths.empty()) {
        cerr << "Error: no DICOM files found in folder " << argv[1] << endl;
        return EXIT_FAILURE;
    }
    
    double start, end;
    start = omp_get_wtime();
    // Create 4D volume from DICOM files
    Mat volume = create_dicom_volume(dicom_file_paths);
    end = omp_get_wtime();
    printf("Tempo decorrido = %.16g segundos\n", end - start);

    if (volume.empty()) {
        cerr << "Error: cannot create DICOM volume" << endl;
        return EXIT_FAILURE;
    }

    // Save 4D volume to disk as DICOM file
    string output_file_path = argv[3];
    imwrite(output_file_path, volume);
    //save_volume_as_nifti(volume, output_file_path);
    
    // Plot 3D volume
    //plot_3d_volume(volume,output_file_path);

    return EXIT_SUCCESS;
}
