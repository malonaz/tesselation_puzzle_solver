#ifndef APS_COMMON_POINT_H_
#define APS_COMMON_POINT_H_


struct Point{
  // x and y coordinates
  int x;
  int y;


  /**
   * Constructor
   */
  Point(int x, int y): x(x), y(y){}

  
  /*
   * Returns the euclidean distance to that point
   */ 
  float distanceTo(Point that);

  /*
   * returns the slope to that point
   */
  float slopeTo(Point that);
  
  bool operator==(const Point& rhs) const;
};

#endif
