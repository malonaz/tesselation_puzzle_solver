#include "shape_translate.h"

#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <map>

#include "common/point.h"
#include "common/shape_matrix.h"
#include "common/types.h"
#include "common/shape_matrix_io.h"

using std::map;
using std::min;
using std::max;

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
void shape_get_size(const ListOfPoints* const shape, uint& width, uint& height) {

  // used to store the max & min of all x and y coordinates
  int max_x = 0, max_y = 0, min_x = 0, min_y = 0;

  for (uint i = 0; i < shape->size(); i++){
    
    // get current point
    Point point = shape->at(i);

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



/**
 * Filter and process an edge, defined as two points, of a shape.
 * Copies all horizontal edges into output parameter horizontal_edges.
 *   @param horizontal_edges: The mapping of y-coordinate to the list of horizontal edges.
 *   @param edge_start: The first point of the edge to process
 *   @param edge_end: The second point of the edge to process
*/
void shape_process_edge(Point edge_start, Point edge_end, map<uint, ListOfEdges*> &horizontal_edges) {

  // if y values are not equal, edge is not horizontal
  if (edge_start.y != edge_end.y)
    return;

  // edge is horizontal here. Save edge's y coordinate
  int y_coord = edge_start.y;

  // If there is no entry of this y coordinate in the map, add a new list of edges.
  if (horizontal_edges.find(y_coord) == horizontal_edges.end())  
    horizontal_edges[y_coord] = new ListOfEdges();
  
  // create edge
  int* edge = new int[2]{edge_start.x, edge_end.x};

  // push the edge onto the map
  horizontal_edges[y_coord]->push_back(edge);
  
} 


/**
 * This function looks up the edges present at the given row and if any, updates the row filter
 * by flipping the boolean values of the blocks implied by these edges. 
 */
void process_row_filter(map<uint, ListOfEdges*> &horizontal_edges, uint row, bool* row_filter) {

  // if there is no edge at this row, no processing to do 
  if (horizontal_edges.find(row) == horizontal_edges.end()) 
    return;

  // this row has edges. let's loop through and perform the flips
  ListOfEdges* edges = horizontal_edges[row];

  
  for (uint i = 0; i < edges->size(); i++){

    // get current edge
    int* current_edge = edges->at(i);

    // extract x coordinates of current edge
    int x1 = current_edge[0];
    int x2 = current_edge[1];

    // get the min and max so we can loop through it properly
    int min_x = min(x1, x2);
    int max_x = max(x1, x2);

    // perform the flips
    for (int col = min_x; col < max_x; col++) 
      row_filter[col] = !row_filter[col];
    
  }
}



void shape_translate(const ListOfPoints* const shape, ShapeMatrix* &matrix) {

  // matrix cannot be null
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
  map<uint, ListOfEdges*> horizontal_edges;

  // get last point of shape 
  Point last_point = shape->at(shape->size() - 1);

  // used to keep track of the last point processed. initially equal to the last point
  Point last_processed_point = last_point;

  for (uint i = 0; i < shape->size(); i++){

    // get current point
    Point current_point = shape->at(i);

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

  // now we must free the edges we have created from the heap

  // create iterator
  map<uint, ListOfEdges*>::iterator pair;
  
  for (pair = horizontal_edges.begin(); pair != horizontal_edges.end(); pair++) {

    // get the vector of edges
    ListOfEdges* edges = pair->second;

    // free each edge in the vector
    for (uint i = 0; i < edges->size(); i++)
      delete[] edges->at(i);

    // now delete the vector
    delete pair->second;
    
  }
} 


/**
 * Helper function which returns the length of the shortest edge in the shape.
 * Returns -1 if an error is found.
 */
int find_shortest_edge_in_shape(const ListOfPoints* const shape) {

  // used to store the length of shortest edge in shape. initiated as -1 for error signaling purposes
  int shortest_edge_length = -1;



  // get the shape's last point
  Point last_point = shape->at(shape->size() - 1);

  // used to keep track of the last point processed. initially equal to the last point
  Point last_processed_point = last_point;

 
  for (uint i = 0; i < shape->size(); i++) {

    // get current point
    Point current_point = shape->at(i);

    // compute the distance between the last processed point and the current point
    int length = static_cast<int>(current_point.distanceTo(last_processed_point));

    // update the current shortest edge length
    if (length < shortest_edge_length || shortest_edge_length == -1) 
      shortest_edge_length = length;
    
    // update last processed point
    last_processed_point = current_point;
    
  }

  // if the shortest edge length is 0, return -1 for error signaling purposes
  return shortest_edge_length? shortest_edge_length: -1;
} 


/**
 * Helper function which returns the unit length implied by this set of shapes.
 */
int find_unit_length(const ListOfShapes* const shapes) {

  // used to store all shortest edges's lengths
  vector<int> shortest_edges_length;
  
  for (uint i = 0; i < shapes->size(); i++) {

    // compute this shape's shortest edge's length
    int shortest_edge_length = find_shortest_edge_in_shape(shapes->at(i));

    // add this shape's shortest edge's length to the vector that holds them for all shapes
    shortest_edges_length.push_back(shortest_edge_length);

  }

  // sort the lengths of each shape's shortest edge
  std::sort(shortest_edges_length.begin(), shortest_edges_length.end());

  // now we wish to keep all the edges that are with delta of each other and average them.
  // total will keep track of the total of these smallest edges' length
  int total = 0;
  
  for (uint i = 0; i < shortest_edges_length.size() - 1; i++){
    
    // add current edge to total
    total += shortest_edges_length[i];

    // gather information about this edge and the next
    float current_edge_length = static_cast<float>(shortest_edges_length[i]);
    float next_edge_length = static_cast<float>(shortest_edges_length[i + 1]);

    // calculate the length change
    float change = (next_edge_length/current_edge_length) - 1;

    // stop if the percent change of the next edge's length is bigger than our delta. returns avg
    if (change > THRESHOLD)
      return total/(i + 1);
  }

  // we will get here if all of edges's length are similar in proportion
  return total/shapes->size();
}


/**
 * Helper function which 
 */
void shape_reduce(ListOfPoints* const shape, int unit_length) {

  // make sure shape is not null
  assert(shape != NULL);
  
  for (uint i = 0; i < shape->size(); i++){

    // divide each coordinate by the unit length of each shape
    shape->at(i).x /= unit_length;
    shape->at(i).y /= unit_length;
    
  }
} 


/**
 * Helper function which modifies the shapes' coordinate to move it in the first quadrant
 */
void move_shape_to_first_quadrant(ListOfPoints* const shape) {

  // set min_x and min_y to the first point of the shape
  int min_x = shape->at(0).x;
  int min_y = shape->at(0).y;

  for (uint i = 1; i < shape->size(); i++){

    // get current point
    Point current_point = shape->at(i);

    // update mininum x and y values
    min_x = min(current_point.x, min_x);
    min_y = min(current_point.y, min_y);
    
  }

  for (uint i = 0; i < shape->size(); i++){

    // now substract the min x and min y from all the x and y coordinates of shape's points.
    shape->at(i).x -= min_x;
    shape->at(i).y -= min_y;
    
  }
}


bool shape_translate_all_shapes(const ListOfShapes* const shapes, ListOfShapeMatrices* const matrices) {
  // asert matrices and shapes are not null
  assert(shapes != NULL);
  assert(matrices != NULL);

  // get the unit length of these shapes
  int unit_length = find_unit_length(shapes);  

  for (uint i = 0; i < shapes->size(); i++){

    // get the current shape
    ListOfPoints* shape = shapes->at(i);

    // normalize the shape using the unit length
    shape_reduce(shape, unit_length);

    // move the shape to the 1st quadrant
    move_shape_to_first_quadrant(shape);

    // used to hold the matrix
    ShapeMatrix* matrix = NULL;

    // matrix will be assigned a matrix from the heap here
    shape_translate(shape, matrix);
    
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
