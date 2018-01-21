#ifndef APS_COMMON_POINT_H_
#define APS_COMMON_POINT_H_

struct Point{
  /**
    x-coordinate of this point
  */
  int x;
  
  /**
    y-coordinate of this point
  */
  int y;

  /**
   * Constructor
   */
  Point(int x, int y): x(x), y(y) {}

  
  /*
    \param that The other point to calculate distance to
    \return the euclidean distance to that point
   */ 
  float distanceTo(Point that) const;

  /*
    \param
    \return The slope to that point
   */
  float slopeTo(Point that) const;
  
  bool operator==(const Point& rhs) const;
};

#endif
