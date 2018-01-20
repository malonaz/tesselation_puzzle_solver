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
