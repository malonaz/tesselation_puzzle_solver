#include "image_read.h"

#include <cmath>
#include <math.h>
#include <iostream>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "common/types.h"

using std::vector;
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
#define SCALE_DOWN_IMAGE_HEIGHT 800
#define SCALE_DOWN_IMAGE_WIDTH 1280
#define MIN_CONTOUR_AREA 1200


/**
 * Helper function which scales down a given image.
 *  @params:
 *   source: matrice representing the image we wish to scale down
 *   target: output parameter which will contain the scaled down image
 *   height:
 *   width:
 */
void scale_down_image(const Mat& source, Mat& target, double height,  double width) {

  // get diagonal of image
  double image_diag = sqrt(pow(source.size().height,2.0) + pow(source.size().width, 2.0));
  double process_diag = sqrt(pow(height,2.0) + pow(width, 2.0));

  // compute scale 
  double scale = process_diag / image_diag;

  // compute the size (height, width) using the scale
  Size size(scale * source.size().width, scale * source.size().height);

  // resize photo
  resize(source, target, size);
}


void find_coordinates(const char* image_filename, vector<ListOfPoints> &polygons_corner_coordinates){
  Mat src, src_gray, src_processed;

  src = imread(image_filename);
  if (src.empty()) {
    cout << "Could not open or find the image!\n" << endl;
    return;
  }
  
  scale_down_image(src, src_processed,SCALE_DOWN_IMAGE_HEIGHT, SCALE_DOWN_IMAGE_WIDTH);
  cv::bitwise_not(src_processed, src_processed);
  cvtColor(src_processed, src_gray, COLOR_BGR2GRAY);
  adaptiveThreshold(src_gray, src_gray, ADAPTIVE_THRESHOLD_VALUE,
    ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 13, 5);

  vector<vector<cv::Point>> contours;
  findContours(src_gray, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
  vector<cv::Point> approx;

  for (uint i = 0; i < contours.size(); ++i) {
    approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);
    if (fabs(contourArea(contours[i])) < MIN_CONTOUR_AREA || approx.size() % 2 == 1) {
      continue;
    }

    ListOfPoints shapeList;

    for (uint j = 0; j < approx.size(); ++j) {
      shapeList.push_back(Point(approx[j].x,approx[j].y));
    }

    polygons_corner_coordinates.push_back(shapeList);
  }
}

void debug_coordinates(const char* image_filename, const vector<ListOfPoints> &polygons_corner_coordinates){

  // get the matrice representation of the image
  Mat src = imread(image_filename);

  // check that image was read correctly
  if (src.empty()) {
    cout << "Could not open or find image!\n" << endl;
  }

  // scale down the image
  scale_down_image(src, src, SCALE_DOWN_IMAGE_HEIGHT, SCALE_DOWN_IMAGE_WIDTH);

  // name a window
  namedWindow("Debug window", WINDOW_AUTOSIZE);

  // print out coordinates of corners to std output stream
  for (uint i = 0; i < polygons_corner_coordinates.size(); i++) {
    
    cout << "Shape " << i << ": " << endl;

    for (uint j = 0; j < polygons_corner_coordinates[i].size(); j++) {

      // get point and print its coordinates
      Point p = polygons_corner_coordinates[i][j];      
      cout << "\t(" << p.x << ", " << p.y << ")" << endl;

      // add a small circle around the corner
      circle(src, cv::Point(p.x, p.y), 5, Scalar(255), 2, 8, 0);
    }

    // insert line break
    cout << endl;

    // show the window
    imshow( "Debug window", src);

    // wait for user to press a key to continue
    waitKey(0);
  }
}
