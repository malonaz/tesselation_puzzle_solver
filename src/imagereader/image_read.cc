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

#define MAX_VALUE 128
#define SCALE_DOWN_IMAGE_HEIGHT 800
#define SCALE_DOWN_IMAGE_WIDTH 1280
#define MIN_CONTOUR_AREA 1200
#define PIXEL_EPSILON 15

/**
 * Helper function which scales down a given image.
 *  @params:
 *   source: matrice representing the image we wish to scale down
 *   target: output parameter which will contain the scaled down image
 */
void scale_down_image(const Mat& source, Mat& target){

  // get diagonal of image
  double image_diag = sqrt(pow(source.size().height, 2.0) + pow(source.size().width, 2.0));
  double process_diag = sqrt(pow(SCALE_DOWN_IMAGE_HEIGHT, 2.0) + pow(SCALE_DOWN_IMAGE_WIDTH, 2.0));

  // compute scale 
  double scale = process_diag / image_diag;

  // compute the size (height, width) using the scale
  Size size(scale * source.size().width, scale * source.size().height);

  // resize photo
  resize(source, target, size);
}


/**
 * Helper function which applies filters to an image of 3D shapes.
 * Optimized to remove shadows and double points.
 *  @params:
 *   image_matrix: the matrix representation of the image
 */
void shape_filter_3D(Mat image_matrix){

  // adaptive threshold
  adaptiveThreshold(image_matrix, image_matrix, MAX_VALUE,
		    ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 27, 16);
}


/**
 * Helper function which applies filters to an image of 2D shapes
 *  @params:
 *   image_matrix: the matrix representation of the image
 */
void shape_filter_2D(Mat image_matrix){

  // adaptive threshold
  adaptiveThreshold(image_matrix, image_matrix, MAX_VALUE,
		    ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 31, 2);
}


/**
 * Helper function which processed the contours of shapes, to identify polygons.
 *  @params:
 *   contours: list of shapes contours
 *   polygons_corners:
 */
void process_contours(vector<vector<cv::Point>> contours, vector<ListOfPoints> &polygons_corners){

  // used by cv's approxPolyDP function as output parameter
  vector<cv::Point> polydp_output_param;
  for (uint i = 0; i < contours.size(); ++i) {

    // use cv library to extract polygon shapes from contours
    approxPolyDP(Mat(contours[i]), polydp_output_param, arcLength(Mat(contours[i]), true) * 0.02, true);
    if (fabs(contourArea(contours[i])) < MIN_CONTOUR_AREA || polydp_output_param.size() % 2 == 1) {
      continue;
    }

    // will be used to store a shape as a list of points
    ListOfPoints polygon_corners;

    for (uint j = 0; j < polydp_output_param.size(); ++j) {

      // flag to determine whether this point will be added
      bool add_point = true;

      // create a point. We will now evaluate whether it is a duplicate
      Point point(polydp_output_param[j].x, polydp_output_param[j].y);

      // iterate through existing points, looking for a duplicate
      for (uint k = 0; k < polygon_corners.size(); k++){

	// compute euclidean distance to each point
	float distance = point.distanceTo(polygon_corners[k]);

	// don't add the point if it is within PIXEL_EPSILON of another
	if (distance < PIXEL_EPSILON){
	  add_point = false;
	  break;
	}
	
      }

      // add the point if flag says so
      if (add_point)
	polygon_corners.push_back(point);
    }

    // add the shape
    polygons_corners.push_back(polygon_corners);
  }
}

void find_coordinates(const char* image_filename, vector<ListOfPoints> &polygons_corners){
  
  bool debug = true;
  
  // attemp to get the matrice respresentation of the image
  Mat image_matrix = imread(image_filename);
  if (image_matrix.empty()) {
    cout << "Could not open or find the image!\n" << endl;
    return;
  }
  
  // create a window
  if (debug)
    namedWindow("Debug window", WINDOW_AUTOSIZE);
    
  // scale down the image
  scale_down_image(image_matrix, image_matrix);

  // show the window
  if (debug){
    imshow( "Debug window", image_matrix);
    waitKey(0);
  }
    
  // convert to grey scale
  cvtColor(image_matrix, image_matrix, COLOR_BGR2GRAY);
  if (debug){
    imshow( "Debug window", image_matrix);
    waitKey(0);
  }
  
  // adaptive threshold
  shape_filter_2D(image_matrix);
  
  if (debug){
    imshow( "Debug window", image_matrix);
    waitKey(0);
  }
  

  // find the polygon's contours using cv's findContours
  vector<vector<cv::Point>> contours;
  findContours(image_matrix, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);


  // process the contours
  process_contours(contours, polygons_corners);
}



void debug_coordinates(const char* image_filename, const vector<ListOfPoints> &polygons_corners){

  // attempt to get the matrice representation of the image
  Mat image_matrix = imread(image_filename);
  if (image_matrix.empty()) {
    cout << "Could not open or find image!\n" << endl;
  }

  // scale down the image
  scale_down_image(image_matrix, image_matrix);

  // name a window
  namedWindow("Debug window", WINDOW_AUTOSIZE);

  // print out coordinates of corners to std output stream
  for (uint i = 0; i < polygons_corners.size(); i++) {
    
    cout << "Shape " << i << ": " << endl;

    for (uint j = 0; j < polygons_corners[i].size(); j++) {

      // get point and print its coordinates
      Point p = polygons_corners[i][j];      
      cout << "\t(" << p.x << ", " << p.y << ")" << endl;

      // add a small circle around the corner
      circle(image_matrix, cv::Point(p.x, p.y), 5, Scalar(255), 2, 8, 0);
    }

    // insert line break
    cout << endl;

    // show the window
    imshow( "Debug window", image_matrix);

    // wait for user to press a key to continue
    waitKey(0);
  }
}
