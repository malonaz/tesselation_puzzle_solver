#include <iostream>
#include "image_read.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

Mat src, src_gray;
int thresh = 150;
int max_thresh = 255;
const char* source_window = "Source image";
const char* corners_window = "Corners detected";

void cornerHarris_demo( int, void* );

void find_coordinates(const char* input) {
  src = imread(input, IMREAD_COLOR);
  if (src.empty()) {
    cout << "Could not open or find the image!\n" << endl;
    return;
  }
  Mat src_processed;

  double scale = 800.0 / src.size().width;
  resize(src, src_processed, Size(scale * src.size().width, scale * src.size().height));
  cvtColor(src_processed, src_gray, COLOR_BGR2GRAY);
  adaptiveThreshold(src_gray, src_gray, 120, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 11, 2);
  GaussianBlur(src_gray, src_gray, Size(5, 5), 0, 0);
  namedWindow( source_window, WINDOW_AUTOSIZE );
  imshow( source_window, src_gray );
  cornerHarris_demo(0, 0);
  waitKey(0);
  return;
}


void cornerHarris_demo( int, void* ) {
  Mat dst, dst_norm, dst_norm_scaled;
  dst = Mat::zeros( src.size(), CV_32FC1 );
  int blockSize = 3;
  int apertureSize = 5;
  double k = 0.04;
  cornerHarris( src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );
  normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
  convertScaleAbs( dst_norm, dst_norm_scaled );
  for (int j = 0; j < dst_norm.rows; j++) {
    for(int i = 0; i < dst_norm.cols; i++) {
        if ((int)dst_norm.at<float>(j,i)> thresh) {
          circle( dst_norm_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
        }
      }
   }
  namedWindow( corners_window, WINDOW_AUTOSIZE );
  imshow( corners_window, dst_norm_scaled );
}
