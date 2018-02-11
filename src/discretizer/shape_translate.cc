#include "shape_translate.h"
#include "common/point.h"
#include "common/shape_matrix.h"
#include "common/types.h"
#include "common/shape_matrix_io.h"

#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <map>
#include <vector>

using std::map;
using std::min;
using std::max;
using std::round;
using std::vector;

// used as threshold to decide if two lengths are similar
#define THRESHOLD 0.20

/*

Steps:
1) Normalize all points in the shape with the unit length
2) translate the points into horizontal edges only
*/

/**
 * Computes the width and height of the shape inplied by the given shape's list of points. 
 *   @param shape: The list of coordinates that define the shape.
 *   @param width: The width of the shape returned by reference.
 *   @param height: The height of the same returned by reference.
*/
void shape_get_size(const vector<Point> &shape, uint& width, uint& height) {

  // used to store the max & min of all x and y coordinates
  int max_x = 0, max_y = 0, min_x = 0, min_y = 0;

  for (uint i = 0; i < shape.size(); i++){
    
    // get current point
    Point point = shape[i];

    // update min and max x,y coordinates
    min_x = min(min_x, point.x);
    max_x = max(max_x, point.x);
    min_y = min(min_y, point.y);
    max_y = max(max_y, point.y);
    
  }

  // compute the width and height of this shape
  width = (uint)(max_x - min_x);
  height = (uint)(max_y - min_y);  
}


struct Edge{
  int start;
  int end;

  Edge(int start, int end): start(start), end(end){}
};


/**
 * Filter and process an edge, defined as two points, of a shape.
 * Copies all horizontal edges into output parameter horizontal_edges.
 *   @param horizontal_edges: The mapping of y-coordinate to the list of horizontal edges.
 *   @param edge_start: The first point of the edge to process
 *   @param edge_end: The second point of the edge to process
*/
void shape_process_edge(Point edge_start, Point edge_end, map<uint, vector<Edge> > &horizontal_edges) {

  // if y values are not equal, edge is not horizontal
  if (edge_start.y != edge_end.y)
    return;

  // edge is horizontal here. Save edge's y coordinate
  int y_coord = edge_start.y;

  // If there is no entry of this y coordinate in the map, add a new list of edges.
  if (horizontal_edges.find(y_coord) == horizontal_edges.end())  
    horizontal_edges[y_coord] = vector<Edge>();
  
  // create edge
  Edge edge = Edge(edge_start.x, edge_end.x);

  // push the edge onto the map
  horizontal_edges[y_coord].push_back(edge);
  
} 


/**
 * This function looks up the edges present at the given row and if any, updates the row filter
 * by flipping the boolean values of the blocks implied by these edges. 
 */
void process_row_filter(map<uint, vector<Edge> > &horizontal_edges, uint row, bool* row_filter) {

  // if there is no edge at this row, no processing to do 
  if (horizontal_edges.find(row) == horizontal_edges.end()) 
    return;

  // this row has edges. let's loop through and perform the flips
  vector<Edge> edges = horizontal_edges[row];

  for (uint i = 0; i < edges.size(); i++){

    // get current edge
    Edge current_edge = edges[i];

    // get the min and max so we can loop through it properly
    int min_x = min(current_edge.start, current_edge.end);
    int max_x = max(current_edge.start, current_edge.end);

    // perform the flips
    for (int col = min_x; col < max_x; col++) 
      row_filter[col] = !row_filter[col];
    
  }
}



void shape_translate(const vector<Point> &shape, ShapeMatrix* &matrix) {

  // matrix must be null
  assert(matrix == NULL);

  // will be used to store the dimensions of the shape implied by the list of point
  uint width = 0;
  uint height = 0;

  // find width and height of the shape
  shape_get_size(shape, width, height);

  // check the width and height are valid
  if (width == 0 || height == 0) 
    return;
  
  // create the row filter, initializing all values to false.
  bool row_filter[width] = {false};

  // used to store the map of edges organized by their row-value
  map<uint, vector<Edge> > horizontal_edges;

  // get last point of shape 
  Point last_point = shape[shape.size() - 1];

  // used to keep track of the last point processed. initially equal to the last point
  Point last_processed_point = last_point;

  for (uint i = 0; i < shape.size(); i++){

    // get current point
    Point current_point = shape[i];

    // process the edge from last processed point to the current point
    shape_process_edge(last_processed_point, current_point, horizontal_edges);

    // update the last processed point
    last_processed_point = current_point;
  }
  
  // used to store the shape
  matrix = new ShapeMatrix(width, height);

  // process the matrix row-wise
  for (uint row = 0; row < height; row++) {

    // update the row filter
    process_row_filter(horizontal_edges, row, row_filter);

    // process this row, column by column
    for (uint col = 0; col < width; ++col) 
      matrix->set(row, col, row_filter[col]);
    
  }
} 



/**
 * Helper function which uses delta to figure out which lengths are similar
 * and returns the average of the shortest lengths
 */
int get_average_shortest_length(vector<int> &lengths){

  // sort the vector
  std::sort(lengths.begin(), lengths.end());

  // now we wish to keep track of all the lengths that are within delta of each other
  // and average them. Total will be used to keep a total count
  int total = 0;
  
  for (uint i = 0; i < lengths.size() - 1; i++){

    // add current length to total
    total += lengths[i];

    // gather information about length and the next
    float current_length = static_cast<float>(lengths[i]);
    float next_length = static_cast<float>(lengths[i + 1]);

    // calculate the length change
    float change = (next_length/current_length) - 1;

    // stop if the percent change of the next edge's length is bigger than our delta. returns avg
    if (change > THRESHOLD)
      return total/(i + 1);
  }

  // we will get here if all the lengths are of similar proportions
  return total/lengths.size();
}



/**
 * Helper function which returns the length of the shortest edge in the shape.
 */
int find_shortest_edge_in_shape(const vector<Point> &shape) {

  // used to store all the edge's lengths
  vector<int> edges_length;

  // get the shape's last point
  Point last_point = shape[shape.size() - 1];

  // used to keep track of the last point processed. initially equal to the last point
  Point last_processed_point = last_point;
 
  for (uint i = 0; i < shape.size(); i++) {

    // get current point
    Point current_point = shape[i];

    // compute the distance between the last processed point and the current point
    int length = static_cast<int>(current_point.distanceTo(last_processed_point));

    // push it onto the edges length vector
    edges_length.push_back(length);
    
    // update last processed point
    last_processed_point = current_point;
    
  }
  // will need to replace this
  std::sort(edges_length.begin(), edges_length.end());
  return edges_length[0];
  
  
  //  return get_average_shortest_length(edges_length);

} 


/**
 * Helper function which returns the unit length implied by this set of shapes.
 */
int find_unit_length(const vector<vector<Point>*> &shapes) {

  // used to store all shortest edges's lengths
  vector<int> shortest_edges_length;
  
  for (uint i = 0; i < shapes.size(); i++) {

    // compute this shape's shortest edge's length
    int shortest_edge_length = find_shortest_edge_in_shape(*shapes[i]);

    // add this shape's shortest edge's length to the vector that holds them for all shapes
    shortest_edges_length.push_back(shortest_edge_length);

  }

  return get_average_shortest_length(shortest_edges_length);
}


/**
 * Helper function which 
 */
void normalize_shape(vector<Point>  &shape, int unit_length) {
  
  for (uint i = 0; i < shape.size(); i++){

    // divide each coordinate by the unit length of each shape
    shape[i].x = round(static_cast<float>(shape[i].x)/unit_length);
    shape[i].y = round(static_cast<float>(shape[i].y)/unit_length);
    
  }
} 


/**
 * Helper function which modifies the shapes' coordinate to move it in the first quadrant
 */
void move_shape_to_first_quadrant(vector<Point> &shape) {

  // set min_x and min_y to the first point of the shape
  int min_x = shape[0].x;
  int min_y = shape[0].y;

  for (uint i = 1; i < shape.size(); i++){

    // get current point
    Point current_point = shape[i];

    // update mininum x and y values
    min_x = min(current_point.x, min_x);
    min_y = min(current_point.y, min_y);
    
  }

  for (uint i = 0; i < shape.size(); i++){

    // now substract the min x and min y from all the x and y coordinates of shape's points.
    shape[i].x -= min_x;
    shape[i].y -= min_y;
    
  }
}


bool shape_translate_all_shapes(const vector<ListOfPoints*>* const shapes, vector<ShapeMatrix*>* const matrices) {

  // assert matrices and shapes are not null
  assert(shapes != NULL);
  assert(matrices != NULL);

  // get the unit length of these shapes
  int unit_length = static_cast<float>(find_unit_length(*shapes))*1.10;
  
  for (uint i = 0; i < shapes->size(); i++){

    // get the current shape
    vector<Point>* shape = shapes->at(i);

    
    // normalize the shape using the unit length
    normalize_shape(*shape, unit_length);

    
    // move the shape to the 1st quadrant
    move_shape_to_first_quadrant(*shape);

    
    // used to hold the matrix
    ShapeMatrix* matrix = NULL;

    // matrix will be assigned a matrix from the heap here
    shape_translate(*shape, matrix);
    
    // ignore shapes that cannot produce matrices
    if (matrix == NULL) 
      continue;

    // push this matrix onto the matrices
    matrices->push_back(matrix);

    // print it for debug
    shape_matrix_print(matrix);
    
  }

  return true;
} 
