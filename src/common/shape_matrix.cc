#include <cassert>
#include <cstddef>
#include "shape_matrix.h"

ShapeMatrix::ShapeMatrix(int width, int height):
    width(width), height(height),
    shapeArea(0), shape(NULL) {
  assert(width > 0);
  assert(height > 0);

  int area = width * height;
  this->shape = new bool[area];
  for (int i = 0; i < area; ++i) {
    this->shape[i] = false;
  }
} // ShapeMatrix::ShapeMatrix(int, int)

ShapeMatrix::ShapeMatrix(const ShapeMatrix &copy):
    width(copy.width), height(copy.height),
    shapeArea(copy.shapeArea), shape(NULL) {
  int area = this->width * this->height;
  this->shape = new bool[area];
  for (int i = 0; i < area; ++i) {
    this->shape[i] = copy.shape[i];
  }
} // ShapeMatrix::ShapeMatrix(ShapeMatrix&)

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
} // ShapeMatrix::set(int, bool)

void ShapeMatrix::set(int row, int col, bool value) {
  assert(row >= 0 && row < this->height);
  assert(col >= 0 && col < this->width);
  this->set(row * this->width + col, value);
} // ShapeMatrix::set(int, int, bool)

bool ShapeMatrix::get(int index) const {
  assert(index >= 0 && index < (this->width * this->height));
  return this->shape[index];
} // ShapeMatrix::get(int)

bool ShapeMatrix::get(int row, int col) const {
  assert(row >= 0 && row < this->height);
  assert(col >= 0 && col < this->width);
  return this->shape[row * this->width + col];
} // ShapeMatrix::get(int, int)

int ShapeMatrix::getWidth() const {
  return this->width;
} // ShapeMatrix::getWidth()

int ShapeMatrix::getHeight() const {
  return this->height;
} // ShapeMatrix::getHeight()

int ShapeMatrix::getShapeArea() const {
  return this->shapeArea;
} // ShapeMatrix::getShapeArea()

int ShapeMatrix::getMatrixArea() const {
  return this->width * this->height;
} // ShapeMatrix::getMatrixArea()

ShapeMatrix* ShapeMatrix::rotate() const {
  ShapeMatrix* rotated = new ShapeMatrix(this->height, this->width);
  rotated->shapeArea = this->shapeArea;

  for(int i = 0;  i < rotated->height; i++){
    for(int j = 0; j < rotated->width; j++){
      rotated->shape[i * rotated->width + j] =
        this->shape[(this->height - j - 1) * this->width + i];
    }
  }
  return rotated;
} // ShapeMatrix::rotate()

ShapeMatrix* ShapeMatrix::rotate(int n) const {
  if (n < 0) {
    return NULL;
  }
  n = n % 4; // 4 rotations will go back to itself
  if (n == 0) {
    // no rotation, but create a copy
    return new ShapeMatrix(*this);
  }
  if (n == 1) {
    return this->rotate();
  }

  ShapeMatrix* tempMatrix = this->rotate();
  ShapeMatrix* result = tempMatrix->rotate(n - 1);
  delete tempMatrix;
  return result;
} // ShapeMatrix::rotate(int)


ShapeMatrix& ShapeMatrix::operator=(const ShapeMatrix& rhs) {
  if (this == &rhs) {
    return *this;
  }
  delete[] this->shape;

  this->width = rhs.width;
  this->height = rhs.height;
  this->shapeArea = rhs.shapeArea;

  int area = this->width * this->height;
  this->shape = new bool[area];
  for (int i = 0; i < area; ++i) {
    this->shape[i] = rhs.shape[i];
  }

  return *this;
} // ShapeMatrix::operator=(ShapeMatrix&)

bool ShapeMatrix::operator==(const ShapeMatrix& rhs) const {
  if (this == &rhs) {
    return true;
  }
  
  if (this->width != rhs.width || this->height != rhs.height) {
    return false;
  }
  
  if (this->shapeArea != rhs.shapeArea) {
    return false;
  }
  
  int area = this->width * this->height;
  for (int i = 0; i < area; ++i) {
    if (this->shape[i] != rhs.shape[i]) {
      return false;
    }
  }
  return true;
}

bool ShapeMatrix::operator!=(const ShapeMatrix& rhs) const {
  return !(*this == rhs);
}

ShapeMatrix::~ShapeMatrix() {
  delete[] this->shape;
} // ShapeMatrix::~ShapeMatrix()
