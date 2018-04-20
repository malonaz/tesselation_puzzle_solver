#ifndef IMAGE_READ_H_
#define IMAGE_READ_H_
#include "common/types.h"

#include <vector>

using std::vector;


/**
 * Function which given an image, finds the coordinates of the corner of any polygon present 
 * in the image.
 *  @params:
 *   image_filename: the filename of the image we wish to analyze
 *   polygons_corner_coordinates: output parameter which will contain the coordinates of 
 *   each polygon's corners. each polygon's corners are ordered clockwise.
 */
void find_coordinates(const char* image_filename, vector<ListOfPoints> &polygons_corner_coordinates);


/**
 * Function which is used for debugging find_coordinates() function. It displays a screen 
 * and allows the user to step-by-step see which corners are found. It is called after 
 * an image has been processed using find_coordinates() and the list of polygon coordinates
 * has been generated.
 *  @params:
 *   image_filename: the image that was analy
 *   polygons_corner_coordinates: containst the pixel coordinates of each polygon's corners. 
 */
void debug_coordinates(const char* image_filename, const vector<ListOfPoints> &polygons_corner_coordinates);


#endif

//ordering points clockwise
//https://www.pyimagesearch.com/2016/03/21/ordering-coordinates-clockwise-with-python-and-opencv/
//perspective transform
//https://www.pyimagesearch.com/2014/08/25/4-point-opencv-getperspective-transform-example/
