#include "point.h"

#include <cmath>
#include <ostream>

using std::ostream;
using std::sqrt;

float Point::distanceTo(Point that) const {
  float y_diff = that.y - y;
  float x_diff = that.x - x;
  return sqrt(y_diff * y_diff + x_diff * x_diff);
}

double Point::distanceToHighPrecision(Point that) const{
  double y_diff = that.y - y;
  double x_diff = that.x - x;
  return sqrt(y_diff * y_diff + x_diff * x_diff);
}

float Point::slopeTo(Point that) const {
  float y_diff = that.y - y;
  float x_diff = that.x - x;
  return y_diff / x_diff;
}

bool Point::operator==(const Point& rhs) const {
  return x == rhs.x
    && y == rhs.y;
}

ostream& operator<<(ostream& stream, const Point& point){
  return stream << '(' << point.x << ", " << point.y << ')';
}
