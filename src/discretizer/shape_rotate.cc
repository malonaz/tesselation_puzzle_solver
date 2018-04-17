#include "common/point.h"
#include "common/shape_matrix.h"
#include "common/types.h"
#include "shape_rotate.h"
#include "shape_translate.h"


#include <cassert>
#include <cmath>
#include <iostream>


using std::round;
using std::abs;

// used to keep track of direction of segment w.r to x-y axis.
enum Direction {NORTH, EAST, SOUTH, WEST};

// used to repesent the quadrants of a Cartesian coordinate graph
enum Quadrant {I = 1, II = 2, III = 3, IV = 4, INVALID_QUADRANT};

// used as threshold to decide if two points are equal
#define THRESHOLD 0.15


/**
 * Helper function which returns the Quadrant where point (x, y) lies relative to the origin
 *  @params:
 *   x: x coordinate of the point
 *   y: y coordinate of the point
 *   epsilon: a threshold of uncertainty s.t. x and y must be larger in magnitude than epsilon
 *  @returns:
 *   Quadrant enum: I, II, III, IV or INVALID_QUADRANT
 */
Quadrant get_quadrant(int x, int y, int epsilon){
  if (abs(x) <= epsilon || abs(y) <= epsilon)
    return INVALID_QUADRANT;

  if (x > 0)
    return y > 0? I: II;

  // x < 0
  return y > 0? IV: III;
}



/**
 * Helper function which returns true if the given change of Quadrant implies a right turn.
 *  @params:
 *   previous_quadrant: quadrant of shape's previous side
 *   current_quadrant: quandra of shape's current side
 */
bool turn_right(Quadrant previous_quadrant, Quadrant current_quadrant) {
  // assert quadrants are not equal
  //assert (previous_quadrant != current_quadrant);

  
  // 1st corner case
  if (previous_quadrant == IV && current_quadrant == I) {
    return true;
  }

  // 2nd corner case
  if (previous_quadrant == I && current_quadrant == IV) {
    return false;
  }

  return current_quadrant > previous_quadrant;
}



/**
 * Helper function which completely rotates a nearly rotated shape.
 *  @params:
 *   shape_points: nearly rotated shape as a list of points
 *  @returns:
 *   list of points of completely rotated shape
 */
ListOfPoints fix_nearly_rotated_shape(ListOfPoints const &shape_points) {
  
  // gather information about the first side
  Point start = shape_points[0];
  Point end = shape_points[1];

  // find the shift of the first point. We will remove it from all points to center shape at origin
  int x_shift = start.x;
  int y_shift = start.y;
  
  // is the first segment horizontal
  bool current_segment_horizontal = abs(end.x - start.x) > abs(end.y - start.y);

  // used to store the rotated shape
  ListOfPoints rotated_shape_points;

  // push the (0, 0) point as usual
  rotated_shape_points.push_back(Point(0, 0));
  
  for (uint i = 1; i < shape_points.size(); i++){
    // get the previous and current points
    Point previous_point = shape_points[i - 1];
    Point current_point = shape_points[i];
    
    if (current_segment_horizontal)
      // set y equal to previous point's y
      current_point.y = previous_point.y;
    
    else // current segment is vertical
      // set x equal to previous point's x
      current_point.x = previous_point.x;
    
    // apply the shift of the first point
    current_point.x -= x_shift;
    current_point.y -= y_shift;

    // push current point onto rotated shape
    rotated_shape_points.push_back(current_point);

    // if this segment is horizontal, the next one will be vertical & vice-versa
    current_segment_horizontal = !current_segment_horizontal;
  }
  
  return rotated_shape_points;
}


/**
 * Helper function which returns the average side length of the given shape.
 *  @params: 
 *   shape_points: shape as a list of points
 */
float average_side_length(ListOfPoints const &shape_points){

  // used to store the length of all the shape's sides
  float total_side_length = 0;

  for (uint i = 0; i < shape_points.size(); i++){
    
    // get start and end of current side
    Point start = shape_points[i];
    Point end = i == shape_points.size() - 1? shape_points[0]: shape_points[i + 1];

    // compute current side's length
    float current_side_length = start.distanceTo(end);

    // add current side's length to the total
    total_side_length += current_side_length;
  }

  // get num of sides
  uint num_sides = shape_points.size();

  // return the average side length as an integer
  return total_side_length/num_sides;
}


/**
 * Helper function which sets x equal to the the first integer in array that
 * is within epsilon of x. Otherwise, adds x to array.
 *  @params: 
 *   array: contain integers that we have already processed
 *   epsilon: error threshold to compare two integers.
 *   x: integer which we wish to compare to processed integers in array
 */
void align_integer(std::vector<int> &array, int epsilon, int &x){

  for (uint i = 0; i < array.size(); i++){
    
    if (abs(array[i] - x) <= epsilon){
      // set x equal to the integer
      x = array[i];
      return;
    }
    
  }

  // no match. add x to array
  array.push_back(x);
}



/**
 * Helper function which corrects points that should be on the same line but 
 * are off within the given epsilon
 */
void align_points(ListOfPoints &shape_points, int epsilon){

  // used to save processed x and y coordinates;
  std::vector<int> processed_xs;
  std::vector<int> processed_ys;
  
  for (uint i = 0; i < shape_points.size(); i++){

    // align point x
    align_integer(processed_xs, epsilon, shape_points[i].x);

    // align point y
    align_integer(processed_ys, epsilon, shape_points[i].y);
    
  }
  
}


/**
 * Helper function which rotates the given shape's coordinates so that all sides
 * of the given shape are vertical or horizontal.
 */
void rotate_shape(const ListOfPoints &shape_points, ListOfPoints &rotated_shape_points) {
  
  // size must be minimum 4 for a polygon with 90 degrees corners only & size must be even
  assert(shape_points.size() >= 4);
  assert(shape_points.size() % 2 == 0);

  // gather your epsilon for this shape
  int epsilon = THRESHOLD * average_side_length(shape_points);

  // gather information about first side
  Point start = shape_points[0];
  Point end = shape_points[1];

  // get distance rounded to nearest integer
  float length = round(start.distanceTo(end));

  // add point (0,0) as start of the shape and corresponding end of shape
  rotated_shape_points.push_back(Point(0, 0));
  rotated_shape_points.push_back(Point(length, 0));

  // set previous quadrant to the first side's quadrant, and previous direction to East
  Quadrant previous_quadrant = get_quadrant(end.x - start.x, end.y - start.y, epsilon);
  Direction previous_direction = EAST;

  // save the endpoint of the first side. This is the point we are moving from next
  Point previous_endpoint = rotated_shape_points[1];

  for (uint i = 1; i < shape_points.size() - 1; i++) {

    // get start and end of current side
    start = shape_points[i];
    end = shape_points[i + 1];

    // get quadrant
    Quadrant current_quadrant = get_quadrant(end.x - start.x, end.y - start.y, epsilon);

    if (current_quadrant == INVALID_QUADRANT){
      // shape is already rotated. only needs a bit of fix
      rotated_shape_points = fix_nearly_rotated_shape(shape_points);

      // now align points if needed
      align_points(rotated_shape_points, epsilon); 
      return;
    }

    // get length
    length = round(start.distanceTo(end));

    // are we turning right?
    bool turning_right = turn_right(previous_quadrant, current_quadrant);

    // create new_point
    Point new_point = previous_endpoint;

    if (previous_direction == EAST) {
      new_point.y += turning_right? -length: length;
      previous_direction = turning_right? SOUTH: NORTH;
      
    } else if (previous_direction == SOUTH) {
      new_point.x += turning_right? -length: length;
      previous_direction = turning_right? WEST: EAST;
      
    } else if (previous_direction == NORTH) {
      new_point.x += turning_right? length: -length;
      previous_direction = turning_right? EAST: WEST;
      
    } else { // previous direction is WEST)
      new_point.y += turning_right? length: -length;
      previous_direction = turning_right? NORTH: SOUTH;
    }

    // add new point
    rotated_shape_points.push_back(new_point);

    // save this side's endpoint
    previous_endpoint = new_point;

    // save previous quadrant
    previous_quadrant = current_quadrant;
  }

  // align points if needed
  align_points(rotated_shape_points, epsilon); 
}

void rotate_shapes(const vector<ListOfPoints> &shapes,
		   vector<ListOfPoints> &rotated_shapes) {
  for (uint i = 0; i < shapes.size(); i++) {
    
    // if shape has less than 4, disregard it
    if (shapes[i].size() < 4 || shapes[i].size() % 2 != 0)
      continue;

    // create new shape to store the rotated_shape in
    ListOfPoints rotated_shape;

    // rotate the shape
    rotate_shape(shapes[i], rotated_shape);

    // push it onto output parameter rotated_shapes
    rotated_shapes.push_back(rotated_shape);
  }
}
