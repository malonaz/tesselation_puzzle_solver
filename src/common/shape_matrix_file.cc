#include <fstream>
#include <iostream>
#include <vector>

#include "shape_matrix_file.h"
#include "ShapeMatrix.h"

using namespace std;

void shape_matrix_read(const char* filename, vector<ShapeMatrix*>* list) {
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
    list.push_back(shape);
  }
}