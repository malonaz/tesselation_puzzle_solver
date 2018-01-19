#include <cassert>
#include "shape_matrix.h"

ShapeMatrix::ShapeMatrix(int width, int height) {
  assert(width > 0);
  assert(height > 0);
  this->width = width;
  this->height = height;
  this->shapeArea = 0;

  int area = width * height;
  this->shape = new bool[area];
  for (int i = 0; i < area; ++i) {
    this->shape[i] = false;
  }
}

ShapeMatrix::ShapeMatrix(ShapeMatrix& const copy) {
  this->width = copy->width;
  this->height = copy->height;
  this->shapeArea = copy->shapeArea;

  int area = this->width * this->height;
  this->shape = new bool[area];
  this->shape = new bool[area];
  for (int i = 0; i < area; ++i) {
    this->shape[i] = copy->shape[i];
  }
}

void ShapeMatrix::set(int index, bool value) {
  assert(index >= 0 && index < (this->width * this->height));
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
  assert(row >= 0 && row < this->height);
  assert(col >= 0 && col < this->width);
  this->set(row * this->width + col, value);
}

bool ShapeMatrix::get(int index) {
  assert(index >= 0 && index < (this->width * this->height));
  return this->shape[index];
}

bool ShapeMatrix::get(int row, int col) {
  assert(row >= 0 && row < this->height);
  assert(col >= 0 && col < this->width);
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

ShapeMatrix* ShapeMatrix::rotate() {
  // TODO

  return NULL;
}

ShapeMatrix* ShapeMatrix::rotate(int n) {
  if (n < 0) {
    return NULL;
  }
  n = n % 4; // 4 rotations will go back to itself
  if (n == 0) {
    // no rotation, but create a copy
    return new ShapeMatrix(this);
  }
  if (n == 1) {
    return this->rotate();
  }

  ShapeMatrix* tempMatrix = this->rotate();
  ShapeMatrix* result = tempMatrix->rotate(n - 1);
  delete tempMatrix;
  return result;
}

ShapeMatrix::~ShapeMatrix() {
  delete[] this->shape;
}
