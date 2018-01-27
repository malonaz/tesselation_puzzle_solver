#include "shape_matrix.h"

#include <cassert>
#include <cstddef>

#include "types.h"

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

void ShapeMatrix::set(uint index, bool value) {
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

void ShapeMatrix::set(uint row, uint col, bool value) {
  assert(row >= 0 && row < this->height);
  assert(col >= 0 && col < this->width);
  this->set(row * this->width + col, value);
} // ShapeMatrix::set(int, int, bool)

bool ShapeMatrix::get(uint index) const {
  assert(index >= 0 && index < (this->width * this->height));
  return this->shape[index];
} // ShapeMatrix::get(int)

bool ShapeMatrix::get(uint row, uint col) const {
  assert(row >= 0 && row < this->height);
  assert(col >= 0 && col < this->width);
  return this->shape[row * this->width + col];
} // ShapeMatrix::get(int, int)

uint ShapeMatrix::getWidth() const {
  return this->width;
} // ShapeMatrix::getWidth()

uint ShapeMatrix::getHeight() const {
  return this->height;
} // ShapeMatrix::getHeight()

uint ShapeMatrix::getShapeArea() const {
  return this->shapeArea;
} // ShapeMatrix::getShapeArea()

uint ShapeMatrix::getMatrixArea() const {
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

ShapeMatrix* ShapeMatrix::mirror() const {
  ShapeMatrix* result = new ShapeMatrix(width, height);
  int last_index = this->height - 1;
  // copy the matrix over but mirrored row-wise
  for (int i = 0; i < this->height; ++i) {
    for (int j = 0; j < this->width; ++j) {
      result->set(last_index - i, j, this->get(i, j));
    }
  }
  return result;
} // ShapeMatrix::mirror()

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
