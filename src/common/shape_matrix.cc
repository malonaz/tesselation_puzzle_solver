#include "shape_matrix.h"
#include "types.h"

#include <cassert>
#include <cstddef>


ShapeMatrix::ShapeMatrix(int identifier, int width, int height):
  identifier(identifier),
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
  identifier(copy.identifier),
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

uint ShapeMatrix::getIdentifier() const{
  return this->identifier;
}

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

ShapeMatrix ShapeMatrix::rotate() const {
  ShapeMatrix rotated = ShapeMatrix(identifier, height, width);
  rotated.shapeArea = this->shapeArea;

  for(uint i = 0;  i < rotated.height; i++){
    for(uint j = 0; j < rotated.width; j++){
      rotated.shape[i * rotated.width + j] =
        this->shape[(this->height - j - 1) * this->width + i];
    }
  }
  return rotated;
} // ShapeMatrix::rotate()


ShapeMatrix ShapeMatrix::flip() const {
  ShapeMatrix flipped = ShapeMatrix(identifier, width, height);
  int last_index = this->height - 1;
  // copy the matrix over but mirrored row-wise
  for (uint i = 0; i < this->height; ++i) {
    for (uint j = 0; j < this->width; ++j) {
      flipped.set(last_index - i, j, this->get(i, j));
    }
  }
  return flipped;
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


bool operator< (const ShapeMatrix& a, const ShapeMatrix& b){
  return a.getShapeArea() < b.getShapeArea();
}
