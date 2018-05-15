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
 * Helper function which opens a window to display the given image.
 * Used for debugging purposes
 *  @params:
 *   image_matrix: the matrix representation of an image
 */
void debug_image(Mat image_matrix){

  // name a window
  namedWindow("Debug window", WINDOW_AUTOSIZE);

  // show the window
  imshow( "Debug window", image_matrix);

  // wait for user to press a key to continue
  waitKey(0);
}



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
 * Helper function which uses opencv's library to try and identify polygon corners 
 * and copies them into output parameter polygons_corners
 *  @params:
 *   image_matrix: matrix representation of image from which we will attempt to extract polygons.
 *   polygons_corners: output parameter which will hold a list of corners for each identified polygon.
 */
void identify_polygons(Mat &image_matrix, vector<ListOfPoints> &polygons_corners){
  int shape_count = 0;
  // find the polygon's contours using cv's findContours
  vector<vector<cv::Point>> contours;
  findContours(image_matrix, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

  // used by cv's approxPolyDP function as output parameter
  vector<cv::Point> polydp_output_param;
  for (uint i = 0; i < contours.size(); ++i) {

    // use cv library to extract polygon shapes from contours
    approxPolyDP(Mat(contours[i]), polydp_output_param, arcLength(Mat(contours[i]), true) * 0.02, true);

    // series of checks
    if (fabs(contourArea(contours[i])) < MIN_CONTOUR_AREA ||
	polydp_output_param.size() % 2 == 1 ||
	polydp_output_param.size() < 4) {
      continue;
    }
    shape_count++;
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
	double distance = point.distanceToHighPrecision(polygon_corners[k]);
	
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

/**
 * Helper function which scales down an image matrix and converts it to greyscale
 */ 
void standardize_image(Mat& image_matrix){

  // scale down the iamge
  scale_down_image(image_matrix, image_matrix);

  // convert to grey scale
  cvtColor(image_matrix, image_matrix, COLOR_BGR2GRAY);
  
  
}


/**
 * Helper function which returns true if a list within list_of_lists
 * contains an odd number of items.
 *  @params:
 *   list_of_lists: contains lists that we want to check the size of
 */
bool contains_lists_with_odd_sizes(vector<ListOfPoints>& list_of_lists){

  for (uint i = 0; i < list_of_lists.size(); i++){

    if (list_of_lists[i].size() % 2 == 1)
      return true;
    
  }

  return false;;
}

/**
 * Helper function which attempts to extract polygons' corners
 * by treating the image as a picture of 3D shapes.
 * Optimized to remove shadows and double points.
 *  @params:
 *   image_matrix: the matrix representation of the image
 *   polygons_corners: output parameter which will contain the corners of 
 *                     the identifed polygons
 */
void find_coordinates_3D(Mat &image_matrix, vector<ListOfPoints> &polygons_corners){

  // print message
  cout << "Treating image as picture of 3D shapes" << endl;
  
  // adaptive threshold
  adaptiveThreshold(image_matrix, image_matrix, MAX_VALUE,
		    ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 27, 15);
  
  // get polygon corners
  identify_polygons(image_matrix, polygons_corners);

}


/**
 * Helper function which attempts to extract polygons' corners
 * by treating the image as a picture of 2D shapes.
 *  @params:
 *   image_matrix: the matrix representation of the image
 *   polygons_corners: output parameter which will contain the corners of 
 *                     the identifed polygons
 */
void find_coordinates_2D(Mat &image_matrix, vector<ListOfPoints> &polygons_corners){

  // print message
  cout << "Treating image as image of 2D shapes" << endl;
  
  // adaptive threshold
  adaptiveThreshold(image_matrix, image_matrix, MAX_VALUE,
  		    ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 31, 2);
  
  // get polygon corners
  identify_polygons(image_matrix, polygons_corners);

}


/**
 * Helper function which attempts to extract polygons' corners
 * by treating the image as a picture of 2D shapes.
 *  @params:
 *   image_matrix: the matrix representation of the image
 *   polygons_corners: output parameter which will contain the corners of 
 *                     the identifed polygons
 */
void find_coordinates_2D_picture(Mat &image_matrix, vector<ListOfPoints> &polygons_corners){

  // print message
  cout << "Treating image as picture of 2D shapes" << endl;
  
  /// Reduce noise with a kernel 1 x 1
  cv::blur(image_matrix, image_matrix, Size(1, 1));
 
  
  /// canny edge detector
  int lowThreshold = 20, kernel_size = 3, maxThreshold = 255;
  cv::Canny(image_matrix, image_matrix, lowThreshold, maxThreshold, kernel_size, true);

  // floodfill
  cv::floodFill(image_matrix, cv::Point(0, 0), Scalar(255, 255, 255));

  // invert because identify_polygons will need shapes to be filled in white
  cv::bitwise_not(image_matrix, image_matrix);

  /// Reduce noise with a kernel 5 x 5 kernel
  cv::blur(image_matrix, image_matrix, Size(5, 5));
  
  // get polygon corners
  identify_polygons(image_matrix, polygons_corners);

}




void find_coordinates(const char* image_filename, vector<ListOfPoints> &polygons_corners){
  
  // attemp to get the matrice respresentation of the image
  Mat image_matrix = imread(image_filename);
  
  if (image_matrix.empty()) {
    cout << "Could not open the image!\n" << endl;
    return;
  }
  
  // standardize the image
  standardize_image(image_matrix);

  // make a copy of the standardized image for potential later 3D processing
  Mat image_matrix_3D = image_matrix.clone();
  Mat image_matrix_2D_picture = image_matrix.clone();
  
  // 2d processing
  find_coordinates_2D(image_matrix, polygons_corners);

  // if 2d processing did not work, try the picture of 2d
  if (contains_lists_with_odd_sizes(polygons_corners)){
    polygons_corners.clear();
    find_coordinates_2D_picture(image_matrix_2D_picture, polygons_corners);
  }

  // if 2d picture did not work, then try 3D
  if (contains_lists_with_odd_sizes(polygons_corners)){
    polygons_corners.clear();
    find_coordinates_3D(image_matrix_3D, polygons_corners);
  }
  
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
