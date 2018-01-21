#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>

#include "shape_matrix_io.h"
#include "shape_matrix.h"
#include "types.h"

using namespace std;

void shape_matrix_print(const ShapeMatrix* const shape) {
  shape_matrix_print(*shape);
} // shape_matrix_print(ShapeMatrix*)

void shape_matrix_print(const ShapeMatrix& shape) {
  int width = shape.getWidth();
  int height = shape.getHeight();
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      cout << (shape.get(r, c) ? '1' : '0') << '\t';
    }
    cout << endl;
  }
} // shape_matrix_print(ShapeMatrix&)

void shape_matrix_write(const char* filename, ListOfShapeMatrices* const list) {
  assert(list != NULL);
  ofstream output_file(filename);
  if (output_file.fail()) {
    return;
  }
  ListOfShapeMatrices::iterator iterator;

  for (iterator = list->begin(); iterator != list->end(); ++iterator) {
    ShapeMatrix* shape = *iterator;
    output_file << shape->getWidth() << ' ' << shape->getHeight() << ' ';
    int area = shape->getMatrixArea();
    for (int i = 0; i < area; ++i) {
      output_file << (shape->get(i) ? '1' : '0') << ' ';
    }
    output_file << endl;
  }
} // shape_matrix_write(const char*, ListOfShapeMatrices*)

void shape_matrix_read(const char* filename, ListOfShapeMatrices* const list) {
  assert(list != NULL);
  ifstream input_file(filename);
  if (input_file.fail()) {
    return;
  }

  while (!input_file.eof()) {
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
} // shape_matrix_read(const char*, ListOfShapeMatrices*)
