#ifndef IMAGE_READ_H_
#define IMAGE_READ_H_
#include <iostream>
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

struct Rectangle{
  Rect rectangleOfAPolygon;
  int area;
  Rectangle(Rect input1, int input2){rectangleOfAPolygon = input1; area = input2;}
};
void find_coordinates(const char* input);

#endif
