
#include <cstddef>
#include <gtest/gtest.h>
#include "common/shape_matrix.h"

namespace {
TEST(ShapeMatrixTest, DataStructure) {
  int width = 10;
  int height = 5;
  ShapeMatrix* matrix = new ShapeMatrix(width, height);

  EXPECT_EQ(height, matrix->getHeight());
  EXPECT_EQ(width, matrix->getWidth());
  EXPECT_EQ(height * width, matrix->getMatrixArea());
  EXPECT_EQ(0, matrix->getShapeArea());

  delete matrix;
}

TEST(ShapeMatrixTest, SetGet) {
  int width = 10;
  int height = 5;
  ShapeMatrix* matrix = new ShapeMatrix(width, height);

  EXPECT_EQ(0, matrix->getShapeArea());
  matrix->set(0, 0, true);
  EXPECT_EQ(1, matrix->getShapeArea());
  EXPECT_TRUE(matrix->get(0, 0));

  matrix->set(0, 0, false);
  EXPECT_EQ(0, matrix->getShapeArea());
  EXPECT_FALSE(matrix->get(0, 0));

  delete matrix;
}

TEST(ShapeMatrixTest, RotateOnce) {
  int width = 2;
  int height = 3;
  ShapeMatrix* matrix = new ShapeMatrix(width, height);
  ShapeMatrix* rotatedMatrix = NULL;

  matrix->set(0, 0, true);
  matrix->set(0, 1, true);
  matrix->set(0, 2, true);
  matrix->set(1, 2, true);
  // an L shape

  rotatedMatrix = matrix->rotate();
  EXPECT_EQ(height, rotatedMatrix->getWidth());
  EXPECT_EQ(width, rotatedMatrix->getHeight());
  delete rotatedMatrix;

  delete matrix;
}

} // namespace
