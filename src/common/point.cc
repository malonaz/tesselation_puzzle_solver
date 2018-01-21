#include "point.h"
#include <cmath>

float Point::distanceTo(Point that){
  float y_diff = that.y - y;
  float x_diff = that.x - x;
  return std::sqrt(y_diff*y_diff + x_diff*x_diff);
}

float Point::slopeTo(Point that){
  float y_diff = that.y - y;
  float x_diff = that.x - x;
  return y_diff/x_diff;
}
