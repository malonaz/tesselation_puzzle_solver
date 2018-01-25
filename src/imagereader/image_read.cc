#include <cmath>
#include <iostream>
#include <string>
#include <vector>
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

void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	cv::Rect r = cv::boundingRect(contour);

	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
	cv::putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}

void find_coordinates(const char* input) {
  src = imread(input);
  if (src.empty()) {
    cout << "Could not open or find the image!\n" << endl;
    return;
  }
  Mat src_processed;

  double scale = 800.0 / src.size().width;
  resize(src, src_processed, Size(scale * src.size().width, scale * src.size().height));
  cvtColor(src_processed, src_gray, COLOR_BGR2GRAY);
  adaptiveThreshold(src_gray, src_gray, 100, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 11, 2);
  //GaussianBlur(src_gray, src_gray, Size(5, 5), 0, 0);
	//Mat cannised;
	//Canny(src_gray, cannised, 0, 250, 5);

  vector< vector<Point> > contours;
  findContours(src_gray, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
  vector<Point> approx;
	Mat src_gray_display = src_gray.clone();

  namedWindow( source_window, WINDOW_AUTOSIZE );
  imshow( source_window, src_gray_display );

  for (unsigned int i = 0; i < contours.size(); ++i) {
    approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);
    if (fabs(contourArea(contours[i])) < 50 || approx.size() % 2 == 1) {
      continue;
    }

    setLabel(src_gray_display, to_string(approx.size()), contours[i]);
		cout << "New Contour: " << endl;
		for (unsigned int j = 0; j < approx.size(); ++j) {
			cout << approx[j] << endl;
			circle( src_gray_display, approx[j], 5,  Scalar(255), 2, 8, 0 );
		  imshow( source_window, src_gray_display );
			waitKey(0);
		}
  }


  //cornerHarris_demo(0, 0);
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
