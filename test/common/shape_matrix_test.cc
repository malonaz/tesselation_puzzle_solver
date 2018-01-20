#include "gtest/gtest.h"
#include "common/shape_matrix.h"

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
