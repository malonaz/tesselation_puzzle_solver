#include <fstream>
#include <iostream>
#include <vector>

#include "shape_matrix_io.h"
#include "shape_matrix.h"

using namespace std;

void shape_matrix_print(ShapeMatrix* const shape) {
  int width = shape->getWidth();
  int height = shape->getHeight();
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      cout << (shape->get(r, c) ? '1' : '0') << '\t';
    }
    cout << endl;
  }
}

void shape_matrix_read(const char* filename, vector<ShapeMatrix*>* const list) {
  ifstream input_file(filename);

  while(!input_file.eof()) {
    int width, height, area;
    input_file >> width;
    input_file >> height;
    area = width * height;
    ShapeMatrix* shape = new ShapeMatrix(width, height);
    for (int i = 0; i < area; ++i) {
      int d;
      input_file >> d;
      shape->set(i, d == 1);
    }
    list->push_back(shape);
  }
}
