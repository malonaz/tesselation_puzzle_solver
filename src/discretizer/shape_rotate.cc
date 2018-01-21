#include "shape_rotate.h"
#include "common/shape_matrix.h"
#include "common/point.h"
#include "common/types.h"
#include "shape_translate.h"

#include "assert.h"
#include <iostream>
#include <cmath>


// used to keep track of direction of segment w.r to x-y axis.
enum Direction {NORTH, EAST, SOUTH, WEST};

// used to repesent the quadrants of a Cartesian coordinate graph
enum Quadrant {I = 1, II = 2, III = 3, IV = 4, INVALID_QUADRANT};

/**
 * Helper function which prints the list of points to the standard output stream
 * in the following way (x1, y1), (x2, y2) etc.
 */
void print_list_of_points(std::vector<Point> shape_points){
  for (uint i = 0; i < shape_points.size(); i++){
    int x = shape_points[i].x;
    int y = shape_points[i].y;
    std::cout << "(" << x << ", " << y << "), ";

  }

  std::cout << std::endl;  
}


/**
 * Helper function which returns the Quadrant where (x, y) lies relative to the origin
 */ 
Quadrant get_quadrant(int x, int y){
  if (x == 0 || y == 0)
    return INVALID_QUADRANT;
  
  if (x > 0){
    if (y > 0)
      return I;
    // y < 0
    return II;
  }
  
  // x < 0
  if (y > 0)
    return  IV;

  // y < 0
  return III;
}



/**
 * Helper function, which returns true if the given change of Quadrant implies a right turn.
 */
bool turn_right(Quadrant previous_quadrant, Quadrant current_quadrant){
  // assert quadrants are not equal
  assert (previous_quadrant != current_quadrant);
  
  // 1st corner case
  if (previous_quadrant == IV && current_quadrant == I)
    return true; 

  // 2nd corner case
  if (previous_quadrant == I && current_quadrant == IV)
    return false;
   
  return current_quadrant > previous_quadrant;
}


/**
 * Helper function which rotates the given shape's coordinates so that all sides 
 * of the given shape are vertical or horizontal.
 */
void rotate_shape(std::vector<Point> shape_points, std::vector<Point> rotated_shape_points ){
  // size must be minimum 4 for a polygon with 90 degrees corners only
  assert(shape_points.size() >= 4);

  // gather information about first side
  Point start = shape_points[0];
  Point end = shape_points[1];
  
  // get distance rounded to nearest integer
  float length = std::round(start.distanceTo(end));

  // add point 0,0 as start of the shape and corresponding end of shape
  rotated_shape_points.push_back(Point(0, 0));
  rotated_shape_points.push_back(Point(length, 0));

  // set previous quadrant to the first side's quadrant, and previous direction to East
  Quadrant previous_quadrant = get_quadrant(end.x - start.x, end.y - start.y);
  Direction previous_direction = EAST;

  // set previous endpoint
  Point previous_endpoint = rotated_shape_points[1];

  for (uint i = 1; i < shape_points.size() - 1; i++){

    start = shape_points[i];
    end = shape_points[i + 1];

    // get quadrant
    Quadrant current_quadrant = get_quadrant(end.x - start.x, end.y - start.y);

    // get length
    length = std::round(start.distanceTo(end));
    
    // are we turning right?
    bool turning_right = turn_right(previous_quadrant, current_quadrant);


    // create new_point
    Point new_point = previous_endpoint;
    
    if (previous_direction == EAST){
      new_point.y += turning_right? -length: length;
      previous_direction = turning_right? SOUTH: NORTH;
    }

    else if (previous_direction == SOUTH){
      new_point.x += turning_right? -length: length;
      previous_direction = turning_right? WEST: EAST;
    }

    else if (previous_direction == NORTH){
      new_point.x += turning_right? length: -length;
      previous_direction = turning_right? EAST: WEST; 
    }

    else { // previous direction is WEST
      if (turning_right)
	std::cout << i << " making a right turn " << std::endl;

      new_point.y += turning_right? length: -length;
      previous_direction = turning_right? NORTH: SOUTH;
    }
    
    // pushback new point
    rotated_shape_points.push_back(new_point);

    // save this side's endpoint
    previous_endpoint = new_point;

    // save previous quadrant
    previous_quadrant = current_quadrant;
  }
}


void rotate_shapes(std::vector< std::vector<Point> > shapes,
		   std::vector< std::vector<Point> > rotated_shapes){

}
