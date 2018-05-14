#ifndef APS_COMMON_POINT_H_
#define APS_COMMON_POINT_H_

#include <ostream>

using std::ostream;

/**
 * Point. This structure represents a point in the Cartesian coordinate graph.
 */
struct Point {

   // x-coordinate of this point
  int x;

  //  y-coordinate of this point
  int y;

  /**
   * Constructor
   */
  Point(int x, int y): x(x), y(y) {}


  /**
   * Returns the euclidean distance to that point.
   */
  float distanceTo(Point that) const;

  /**
   * Returns the euclidean distance to that point with higher precision.
   */
  double distanceToHighPrecision(Point that) const;

  /**
   * Returns the slope to that point.
   */
  float slopeTo(Point that) const;


  bool operator==(const Point& rhs) const;


  /**
   * Overrides the the << operator for a Point object such that it is print (x, y).
   */
  friend ostream& operator<<(ostream& stream, const Point& point);

};

#endif
