#include "shape_matrix.h"

ShapeMatrix::ShapeMatrix(int width, int height) {
  this->width = width;
  this->height = height;
  this->shapeArea = 0;
  int area = width * height;
  this->shape = new bool[area];
  for (int i = 0; i < area; ++i) {
    this->shape[i] = false;
  }
}

void ShapeMatrix::set(int index, bool value) {
  if (this->shape[index] == value) {
    return;
  }
  if (value) {
    ++this->shapeArea;
  } else {
    --this->shapeArea;
  }
  this->shape[index] = value;
}

void ShapeMatrix::set(int row, int col, bool value) {
  this->set(row * this->width + col, value);
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
  return this->shapeArea;
}

int ShapeMatrix::getMatrixArea() {
  return this->width * this->height;
}

ShapeMatrix::~ShapeMatrix() {
  delete[] this->shape;
}
