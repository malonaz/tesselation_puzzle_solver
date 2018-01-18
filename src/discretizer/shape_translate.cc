#include <vector>
#include "shape_translate.h"
#include "../common/shape_matrix.h"

using namespace std;

void shape_get_size(vector<vector<int>*>* const shape, int& width, int& height) {
  width = 0;
  height = 0;
  int num_points = shape.size();
  
  for (int i = 0; i < num_points; ++i) {
    vector<int>* coordinate = shape[i];
    if (coordinate[0] > max_width) {
      max_width = coordinate[0];
    }
    if (coordinate[1] > max_height) {
      max_height = coordinate[1];
    }
  }
}

void shape_translate(vector<vector<int>*>* const shape, ShapeMatrix* matrix) {
  int width = 0;
  int height = 0;
  shape_get_size(shape, width, height);
  
}
