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
  Get the width and height of a shape defined by a list of coordinates.
  \param shape The list of coordinates that define the shape.
  \param width The width of the shape returned by reference.
  \param height The height of the same returned by reference.
*/
void shape_get_size(const ListOfPoints* const shape, uint& width, uint& height) {
  int max_x = 0;
  int max_y = 0;
  int min_x = 0;
  int min_y = 0;

  ListOfPoints::const_iterator iterator;

  for (iterator = shape->begin(); iterator != shape->end(); ++iterator) {
    Point point = *iterator;
    min_x = min(min_x, point.x);
    max_x = max(max_x, point.x);
    min_y = min(min_y, point.y);
    max_y = max(max_y, point.y);
  }
  width = (uint)(max_x - min_x);
  height = (uint)(max_y - min_y);
} // shape_get_size(ListOfPoints*, uint&, uint&)

/**
  Filter and process an edge, defined by two points, of a shape.
  If the edge is horizontal, the edge gets added into horizontal_edges map.
  \param horizontal_edges The mapping of y-coordinate to the list of horizontal edges.
    If the incoming edge defined by point1 and point2 is horizontal, it gets
    added to this map.
  \param point1 The first point of the edge to process
  \param point2 The second point of the edge to process
*/
void shape_process_edge(map<uint, ListOfEdges*> &horizontal_edges,
    Point point1, Point point2) {
  if (point1.y != point2.y) {
    return;
  }

  // edge has same y coordinates
  int y_coord = point1.y;

  if (horizontal_edges.find(y_coord) == horizontal_edges.end()) {
    // there's no entry of this y coordinate in the map
    horizontal_edges[y_coord] = new ListOfEdges();
  }

  // array must be created on heap so that other methods can access it later.
  int* edge = new int[2]{ point1.x, point2.x };
  horizontal_edges[y_coord]->push_back(edge);
} // shape_process_edge(map<uint, ListOfEdges*>&, int[], int[])

void process_row_filter(map<uint, ListOfEdges*> &horizontal_edges,
    uint row, bool row_filter[]) {
  if (horizontal_edges.find(row) == horizontal_edges.end()) {
    return;
  }
  // has the entry for this row. so let's loop through and perform the flips
  ListOfEdges* edges = horizontal_edges[row];

  ListOfEdges::const_iterator edges_it;
  for (edges_it = edges->begin(); edges_it != edges->end(); ++edges_it) {
    int* current_edge = (int*) *edges_it;
    int x1 = current_edge[0];
    int x2 = current_edge[1];

    int min_x = min(x1, x2);
    int max_x = max(x1, x2);

    for (int col = min_x; col < max_x; ++col) {
      row_filter[col] = !row_filter[col];
    }
  }
} // process_row_filter(map<uint, ListOfEdges*>&, uint, bool[])

void shape_translate(const ListOfPoints* const shape, ShapeMatrix* &matrix) {
  assert(matrix == NULL);
  uint width = 0;
  uint height = 0;

  // find width and height of the shape
  shape_get_size(shape, width, height);

  if (width == 0 || height == 0) {
    return;
  }

  // minimum 4 points
  if (shape->size() < 4) {
    return;
  }

  // create the row filter
  bool row_filter[width];
  for (uint i = 0; i < width; ++i) {
    row_filter[i] = false;
  }

  // the map of edges organized by their row-value
  map<uint, ListOfEdges*> horizontal_edges;
  ListOfPoints::const_iterator iterator;

  iterator = shape->begin();
  Point first_point = *iterator;
  Point last_processed_point  = *iterator;

  // skip processing the first point
  for (++iterator; iterator != shape->end(); ++iterator) {
    Point current_point = *iterator;
    shape_process_edge(horizontal_edges, last_processed_point, current_point);
    last_processed_point = current_point;
  }
  // process the edge of last point to the first point
  shape_process_edge(horizontal_edges, last_processed_point, first_point);

  // clean up handled by caller of this method
  matrix = new ShapeMatrix(width, height);

  // process the matrix row-wise
  for (uint row = 0; row < height; ++row) {
    process_row_filter(horizontal_edges, row, row_filter);
    for (uint col = 0; col < width; ++col) {
      matrix->set(row, col, row_filter[col]);
    }
  }

  // perform clean up on the edges we've created.
  map<uint, ListOfEdges*>::iterator map_it;
  for (map_it = horizontal_edges.begin(); map_it != horizontal_edges.end(); ++map_it) {
    ListOfEdges* edges = map_it->second;
    ListOfEdges::iterator list_it;
    for (list_it = edges->begin(); list_it != edges->end(); ++list_it) {
      delete[] *list_it;
    }
    delete map_it->second;
  }
} // shape_translate(ListOfPoints*, ShapeMatrix*&)

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
