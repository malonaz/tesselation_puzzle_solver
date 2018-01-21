#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>

#include "shape_translate.h"
#include "common/shape_matrix.h"

using namespace std;

/*

Steps:
1) Normalize all points in the shape with the unit length
2) translate the points into horizontal edges only
*/

void shape_get_size(const ListOfPoints* const shape, int& width, int& height) {
  width = 0;
  height = 0;
  ListOfPoints::const_iterator iterator;

  for (iterator = shape->begin(); iterator != shape->end(); ++iterator) {
    int* coordinate = (int*)*iterator;
    if (coordinate[0] > width) {
      width = coordinate[0];
    }
    if (coordinate[1] > height) {
      height = coordinate[1];
    }
  }
}

void shape_process_edge(map<int, ListOfEdges*> &horizontal_edges, int coord1[2], int coord2[2]) {
  if (coord1[1] != coord2[1]) {
    return;
  }

  // edge has same y coordinates
  int y_coord = coord1[1];

  if (horizontal_edges.find(y_coord) == horizontal_edges.end()) {
    // there's no entry of this y coordinate in the map
    horizontal_edges[y_coord] = new ListOfEdges();
  }

  // array must be created on heap so that other methods can access it later.
  int* edge = new int[4]{ coord1[0], coord1[1], coord2[0], coord2[1] };
  horizontal_edges[y_coord]->push_back(edge);
}

void process_row_filter(map<int, ListOfEdges*> &horizontal_edges, int row, bool row_filter[]) {
  if (horizontal_edges.find(row) == horizontal_edges.end()) {
    return;
  }
  // has the entry for this row. so let's loop through and perform the flips
  ListOfEdges* edges = horizontal_edges[row];

  ListOfEdges::const_iterator edges_iterator;
  for (edges_iterator = edges->begin(); edges_iterator != edges->end(); ++edges_iterator) {
    int* current_edge = (int*) *edges_iterator;
    int x1 = current_edge[0];
    int x2 = current_edge[2];

    int min_x = min(x1, x2);
    int max_x = max(x1, x2);

    for (int col = min_x; col < max_x; ++col) {
      row_filter[col] = !row_filter[col];
    }
  }
}

void shape_translate(const ListOfPoints* const shape, ShapeMatrix* &matrix) {
  int width = 0;
  int height = 0;
  matrix = NULL;

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
  for (int i = 0; i < width; ++i) {
    row_filter[i] = false;
  }

  // the map of edges organized by their row-value
  map<int, ListOfEdges*> horizontal_edges;
  ListOfPoints::const_iterator iterator;

  iterator = shape->begin();
  int* first_point = (int*) *iterator;
  int* last_processed_point  = (int*) *iterator;

  // skip processing the first point
  for (++iterator; iterator != shape->end(); ++iterator) {
    int* current_coord = (int*) *iterator;
    shape_process_edge(horizontal_edges, last_processed_point, current_coord);
    last_processed_point = current_coord;
  }
  // process the edge of last point to the first point
  shape_process_edge(horizontal_edges, last_processed_point, first_point);

  // clean up handled by caller of this method
  matrix = new ShapeMatrix(width, height);

  // process the matrix row-wise
  for (int row = 0; row < height; ++row) {
    process_row_filter(horizontal_edges, row, row_filter);
    for (int col = 0; col < width; ++col) {
      matrix->set(row, col, row_filter[col]);
    }
  }

  map<int, ListOfEdges*>::iterator map_it;
  for (map_it = horizontal_edges.begin(); map_it != horizontal_edges.end(); ++map_it) {
    ListOfEdges* edges = map_it->second;
    ListOfEdges::iterator list_it;
    for (list_it = edges->begin(); list_it != edges->end(); ++list_it) {
      delete[] *list_it;
    }
    delete map_it->second;
  }
}

int find_shortest_edge_in_shape(ListOfPoints* const shape) {
  int shortest_edge_length = -1;
  ListOfPoints::const_iterator iterator;
  int* first_point = (int*) *iterator;
  int* last_processed_point  = (int*) *iterator;

  // skip processing the first point
  for (++iterator; iterator != shape->end(); ++iterator) {
    int* current_coord = (int*) *iterator;

    int length = abs(current_coord[0] - last_processed_point[0]);
    if (length == 0) {
      length = abs(current_coord[1] - last_processed_point[1]);
    }
    if (length < shortest_edge_length || shortest_edge_length == -1) {
      shortest_edge_length = length;
    }

    last_processed_point = current_coord;
  }

  // process the edge of last point to the first point
  int length = abs(first_point[0] - last_processed_point[0]);
  if (length == 0) {
    length = abs(first_point[1] - last_processed_point[1]);
  }
  if (length < shortest_edge_length || shortest_edge_length == -1) {
    shortest_edge_length = length;
  }

  return shortest_edge_length;
}

int find_unit_length(ListOfShapes* const shapes) {
  int unit_length = -1;

  ListOfShapes::iterator it;
  for (it = shapes->begin(); it != shapes->end(); ++it) {
    int shortest_edge_in_shape = find_shortest_edge_in_shape(*it);
    if (shortest_edge_in_shape < unit_length || unit_length == -1) {
      unit_length = shortest_edge_in_shape;
    }
  }

  return unit_length;
}

void shape_reduce(ListOfPoints* const shape, int unit_length) {
  ListOfPoints::const_iterator iterator;
  for (iterator = shape->begin(); iterator != shape->end(); ++iterator) {
    int* current_coord = (int*) *iterator;
    current_coord[0] = current_coord[0] / unit_length;
    current_coord[1] = current_coord[1] / unit_length;
  }
}

bool shape_translate_all_shapes(ListOfShapes* const shapes,
    vector<ShapeMatrix*>* const matrices) {
  int unit_length = find_unit_length(shapes);
  if (unit_length == -1) {
    return false;
  }

  ListOfShapes::iterator it;
  for (it = shapes->begin(); it != shapes->end(); ++it) {
    ListOfPoints* shape = *it;
    shape_reduce(shape, unit_length);
    ShapeMatrix* matrix = NULL;
    shape_translate(shape, matrix);
    if (matrix == NULL) {
      continue;
    }
    matrices->push_back(matrix);
  }

  return true;
}
