#include "shape_matrix.h"

ShapeMatrix::ShapeMatrix(int width, int height) {
  this->width = width;
  this->height = height;
  int area = width * height;
  this->shape = new bool[area];
}

void ShapeMatrix::set(int index, bool value) {
  this->shape[index] = value;
}

void ShapeMatrix::set(int row, int col, bool value) {
  this->shape[row * this->width + col] = value;
}

bool ShapeMatrix::get(int index) {
  return this->shape[index];
}

bool ShapeMatrix::get(int row, int col) {
  return this->shape[row * this->width + col];
}

int ShapeMatrix::getWidth() {
  return this->width;
}

int ShapeMatrix::getHeight() {
  return this->width;
}

int ShapeMatrix::getShapeArea() {
  int c = 0;
  int area = this->width * this->height;
  for (int i = 0; i < area; ++i) {
    if (this->shape[i]) {
      ++c;
    }
  }
  return c;
}

int ShapeMatrix::getMatrixArea() {
  return this->width * this->height;
}

ShapeMatrix::~ShapeMatrix() {
  delete[] this->shape;
}
