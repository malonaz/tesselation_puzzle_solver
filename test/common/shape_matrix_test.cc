
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

TEST(ShapeMatrixTest, CopyConstructor) {
  int width = 10;
  int height = 5;
  ShapeMatrix* matrix1 = new ShapeMatrix(width, height);
  ShapeMatrix* matrix2 = new ShapeMatrix(*matrix1);

  EXPECT_NE(matrix1, matrix2);

  delete matrix1;
  delete matrix2;
}

TEST(ShapeMatrixTest, SetGet) {
  int width = 10;
  int height = 5;
  ShapeMatrix* matrix = new ShapeMatrix(width, height);

  EXPECT_EQ(0, matrix->getShapeArea());

  // set a previously false location true
  matrix->set(0, 0, true);
  EXPECT_EQ(1, matrix->getShapeArea());
  EXPECT_TRUE(matrix->get(0, 0));

  // set a previously true location true
  matrix->set(0, 0, true);
  EXPECT_EQ(1, matrix->getShapeArea());
  EXPECT_TRUE(matrix->get(0, 0));

  matrix->set(0, 0, false);
  EXPECT_EQ(0, matrix->getShapeArea());
  EXPECT_FALSE(matrix->get(0, 0));

  int area = width * height;
  for (int i = 0; i < area; i++) {
    EXPECT_FALSE(matrix->get(i));
  }

  delete matrix;
}

TEST(ShapeMatrixTest, RotateOnce) {
  int width = 2;
  int height = 3;
  ShapeMatrix* matrix = new ShapeMatrix(width, height);
  ShapeMatrix* rotatedMatrix = NULL;

  matrix->set(0, 0, true);
  matrix->set(1, 0, true);
  matrix->set(2, 0, true);
  matrix->set(2, 1, true);
  // an L shape

  rotatedMatrix = matrix->rotate();
  EXPECT_EQ(height, rotatedMatrix->getWidth());
  EXPECT_EQ(width, rotatedMatrix->getHeight());
  delete rotatedMatrix;

  delete matrix;
}

} // namespace
