#include <vector>
#include "shape_translate.h"
#include "../common/shape_matrix.h"

using namespace std;

void shape_get_size(vector<int[2]>* const shape, int& width, int& height) {
  width = 0;
  height = 0;
  vector<int[2]>::const_iterator iterator;
  
  for (iterator = shape->begin(); iterator != shape->end(); iterator++) {
    int* coordinate = (int*)*iterator;
    if (coordinate[0] > width) {
      width = coordinate[0];
    }
    if (coordinate[1] > height) {
      height = coordinate[1];
    }
  }
}

void shape_translate(vector<int[2]>* const shape, int unit_size, ShapeMatrix* matrix) {
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