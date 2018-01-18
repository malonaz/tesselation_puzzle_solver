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

void shape_translate(vector<int[2]>* const shape, ShapeMatrix* matrix) {
  int width = 0;
  int height = 0;
  shape_get_size(shape, width, height);
  
}
