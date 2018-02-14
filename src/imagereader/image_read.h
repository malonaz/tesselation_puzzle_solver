#ifndef IMAGE_READ_H_
#define IMAGE_READ_H_
#include "common/types.h"

#include <vector>

using std::vector;

void find_coordinates(const char* input, vector<ListOfPoints> &list);
void debug_coordinates(const char* filename, const vector<ListOfPoints> &list);
#endif

//ordering points clockwise
//https://www.pyimagesearch.com/2016/03/21/ordering-coordinates-clockwise-with-python-and-opencv/
//perspective transform
//https://www.pyimagesearch.com/2014/08/25/4-point-opencv-getperspective-transform-example/
