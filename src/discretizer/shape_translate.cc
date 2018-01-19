#include <vector>
#include <map>
#include <algorithm>
#include "shape_translate.h"
#include "../common/shape_matrix.h"

using namespace std;

/*

Steps:
1) Normalize all points in the shape with the unit length
2) translate the points into horizontal edges only
*/

void shape_get_size(vector<int[2]>* const shape, int& width, int& height) {
  width = 0;
  height = 0;
  vector<int[2]>::const_iterator iterator;

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

void shape_process_edge(map<int, vector<int*>*> horizontal_edges, int coord1[2], int coord2[2]) {
  if (coord1[1] != coord2[1]) {
    return;
  }

  // edge has same y coordinates
  int y_coord = coord1[1];

  if (horizontal_edges.find(y_coord) == horizontal_edges.end()) {
    // there's no entry of this y coordinate in the map
    horizontal_edges[y_coord] = new vector<int*>();
  }

  int edge[] = { coord1[0], coord1[1], coord2[0], coord2[1] };
  horizontal_edges[y_coord]->push_back(edge);
}

void process_row_filter(map<int, vector<int*>*> horizontal_edges, int row, bool row_filter[]) {
  if (horizontal_edges.find(row) == horizontal_edges.end()) {
    return;
  }
  // has the entry for this row. so let's loop through and perform the flips
  vector<int*>* edges = horizontal_edges[row];

  vector<int*>::const_iterator edges_iterator;
  for (edges_iterator = edges->begin(); edges_iterator != edges->end(); ++edges_iterator) {
    int* current_edge = (int*) *edges_iterator;
    int x1 = current_edge[0];
    int x2 = current_edge[2];

    int min_x = min(x1, x2);
    int max_x = max(x1, x2);

    for(int col = min_x; col <= max_x; ++col) {
      row_filter[col] = !row_filter[col];
    }
  }
}

void shape_translate(vector<int[2]>* const shape, ShapeMatrix* matrix) {
  int width = 0;
  int height = 0;
  bool is_even_num_points = shape->size() % 2 == 0;
  
  // find width and height of the shape
  shape_get_size(shape, width, height);
  
  // create the row filter
  bool row_filter[width];
  for (int i = 0; i < width; ++i) {
    row_filter[i] = false;
  }
  
  vector<int*> horizontal_edges;
  
  vector<int[2]>::const_iterator iterator;
  for (iterator = shape->begin(); iterator != shape->end(); iterator++) {
    int* coord1 = (int*)*iterator;
    
  }
  
}

void shape_translate_all_shapes(vector<vector<int[2]>*>* const shapes, vector<ShapeMatrix*>* const matrices) {
  
}