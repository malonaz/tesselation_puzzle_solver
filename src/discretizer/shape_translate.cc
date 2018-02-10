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

int find_shortest_edge_in_shape(const ListOfPoints* const shape) {
  int shortest_edge_length = -1;
  
  if (shape->size() < 4) {
    return shortest_edge_length;
  }
  
  ListOfPoints::const_iterator iterator;
  iterator = shape->begin();
  Point first_point = *iterator;
  Point last_processed_point  = *iterator;

  // skip processing the first point
  for (++iterator; iterator != shape->end(); ++iterator) {
    Point current_point = *iterator;

    int length = (int)current_point.distanceTo(last_processed_point);
    if (length < shortest_edge_length || shortest_edge_length == -1) {
      shortest_edge_length = length;
    }

    last_processed_point = current_point;
  }

  // process the edge of last point to the first point
  int length = (int)first_point.distanceTo(last_processed_point);
  if (length < shortest_edge_length || shortest_edge_length == -1) {
    shortest_edge_length = length;
  }

  if (shortest_edge_length == 0) {
    shortest_edge_length = -1;
  }

  return shortest_edge_length;
} // find_shortest_edge_in_shape(ListOfPoints*)

/**
  Find the shortest edge length among all the shapes and take it
  as the unit length

  \param shapes The list of shapes to process
  \return The unit length among all the shapes
*/
int find_unit_length(const ListOfShapes* const shapes) {
  int unit_length = -1;

  // used to store all shortest edges
  std::vector<int> shortest_edges;
  
  ListOfShapes::const_iterator it;
  for (it = shapes->begin(); it != shapes->end(); ++it) {
    
    int shortest_edge_in_shape = find_shortest_edge_in_shape(*it);

    // add current shortest edge to all shortest edges vector
    shortest_edges.push_back(shortest_edge_in_shape);

    std::cout << shortest_edge_in_shape << std::endl;

    if (shortest_edge_in_shape == -1) {
      continue;
    }
    if (shortest_edge_in_shape < unit_length || unit_length == -1) {
      unit_length = shortest_edge_in_shape;
    }


    
  }

  std::sort(shortest_edges.begin(), shortest_edges.end());

  
  int avg = 0;
  for (uint i = 0; i < shortest_edges.size() - 1; i++){
    // add current edge to total
    avg += shortest_edges[i];

    // gather information about this edge and the next
    float current_edge = static_cast<float>(shortest_edges[i]);
    float next_edge = static_cast<float>(shortest_edges[i + 1]);

    // calculate the length change
    float chg = (next_edge/current_edge) - 1;

    if (std::abs(chg) > 0.15){
      // divide total and stop here
      avg /= i + 1;
      break;
    }

  }
      
  std::cout << "size: " << avg << std::endl;
  
  return avg;

} // find_unit_length(ListOfShapes*)

void shape_reduce(ListOfPoints* const shape, int unit_length) {
  assert(shape != NULL);
  ListOfPoints::iterator iterator;
  for (iterator = shape->begin(); iterator != shape->end(); ++iterator) {
    Point* current_point = &(*iterator);
    current_point->x /= unit_length;
    current_point->y /= unit_length;
  }
} // shape_reduce(ListOfPoints* shape, int)

void shape_zero(ListOfPoints* const shape) {
  ListOfPoints::iterator iterator;
  iterator = shape->begin();
  int min_x = (*iterator).x;
  int min_y = (*iterator).y;
  for (++iterator; iterator != shape->end(); ++iterator) {
    Point current_point = *iterator;
    min_x = min(current_point.x, min_x);
    min_y = min(current_point.y, min_y);
  }

  for (iterator = shape->begin(); iterator != shape->end(); ++iterator) {
    Point* current_point = &(*iterator);
    current_point->x -= min_x;
    current_point->y -= min_y;
  }
}

bool shape_translate_all_shapes(const ListOfShapes* const shapes,
    ListOfShapeMatrices* const matrices) {
  assert(shapes != NULL);
  assert(matrices != NULL);
  int unit_length = find_unit_length(shapes);
  if (unit_length == -1) {
    return false;
  }

  ListOfShapes::const_iterator it;
  for (it = shapes->begin(); it != shapes->end(); ++it) {
    ListOfPoints* shape = *it;
    shape_reduce(shape, unit_length);
    shape_zero(shape);
    ShapeMatrix* matrix = NULL;
    shape_translate(shape, matrix);
    // we're ignoring shapes that cannot be produced from
    if (matrix == NULL) {
      continue;
    }
    matrices->push_back(matrix);
    shape_matrix_print(matrix);
  }

  return true;
} // shape_translate_all_shapes(ListOfShapes*, vector<ShapeMatrix*>*)
