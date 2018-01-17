#include "ShapeMatrix.h"

ShapeMatrix::ShapeMatrix(int width, int height) {
  this->width = width;
  this->height = height;
  int area = width * height;
  this->shape = new bool[area];
}

void ShapeMatrix::set(int row, int col, bool value) {
  this->shape[row * this->width + col] = value;
}

bool ShapeMatrix::get(int row, int col) {
  return this->shape[row * this->width + col];
}

ShapeMatrix::~ShapeMatrix() {
  delete[] this->shape;
}