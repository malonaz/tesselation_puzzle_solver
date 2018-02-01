#include "image_read.h"

#include <cmath>
#include <iostream>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "common/types.h"

using std::cout;
using std::endl;

using cv::resize;
using cv::cvtColor;
using cv::findContours;
using cv::approxPolyDP;
using cv::imread;
using cv::Mat;
using cv::adaptiveThreshold;
using cv::COLOR_BGR2GRAY;
using cv::RETR_EXTERNAL;
using cv::CHAIN_APPROX_SIMPLE;
using cv::ADAPTIVE_THRESH_GAUSSIAN_C;
using cv::THRESH_BINARY_INV;
using cv::arcLength;
using cv::contourArea;
using cv::WINDOW_AUTOSIZE;
using cv::waitKey;
using cv::Scalar;
using cv::circle;
using cv::Size;

#define ADAPTIVE_THRESHOLD_VALUE 100
#define SCALE_DOWN_IMAGE_WIDTH 800
#define MIN_CONTOUR_AREA 1200

void scaleDownImage(Mat& source, Mat& target, double width) {
  double scale = width / source.size().width;
  Size size(scale * source.size().width, scale * source.size().height);
  resize(source, target, size);
}

void find_coordinates(const char* input, ListOfShapes* const list){
  
  Mat src, src_gray;

  src = imread(input);
  if (src.empty()) {
    cout << "Could not open or find the image!\n" << endl;
    return;
  }
  Mat src_processed;

  scaleDownImage(src, src_processed, SCALE_DOWN_IMAGE_WIDTH);
  cv::bitwise_not(src_processed, src_processed);
  cvtColor(src_processed, src_gray, COLOR_BGR2GRAY);
  adaptiveThreshold(src_gray, src_gray, ADAPTIVE_THRESHOLD_VALUE,
    ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 13, 5);

  vector< vector<cv::Point> > contours;
  findContours(src_gray, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
  vector<cv::Point> approx;

  for (unsigned int i = 0; i < contours.size(); ++i) {
    approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);
    if (fabs(contourArea(contours[i])) < MIN_CONTOUR_AREA || approx.size() % 2 == 1) {
      continue;
    }

    ListOfPoints* shapeList = new ListOfPoints();

    for (unsigned int j = 0; j < approx.size(); ++j) {
      shapeList->push_back(Point(approx[j].x,approx[j].y));
    }
    
    list->push_back(shapeList);
  }
}

void debug_coordinates(const char* filename, const ListOfShapes* const list){
  Mat src = imread(filename);
  if(src.empty()){
    cout << "Could not open or find image!\n" << endl;
  }
  scaleDownImage(src, src, SCALE_DOWN_IMAGE_WIDTH);
  namedWindow( "Debug window", WINDOW_AUTOSIZE );
  
  /*****prints out coordinates of corners*****/
  for (uint i = 0; i < list->size(); i++) {
    cout << "Shape " << i << ": " << endl;
    ListOfPoints* shapeList = (*list)[i];
    for (uint j = 0; j < shapeList->size(); j++) {
      Point p = (*shapeList)[j];
      cout << "\t(" << p.x << ", " << p.y << ")" << endl;
      circle( src, cv::Point(p.x, p.y), 5, Scalar(255), 2, 8, 0 );
    }
    cout << endl;
    imshow( "Debug window", src);
    waitKey(0);
  }
  //waitKey(0);
}
