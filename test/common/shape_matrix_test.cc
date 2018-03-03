
#include <cstddef>
#include <gtest/gtest.h>
#include "common/shape_matrix.h"

namespace {

TEST(ShapeMatrixTest, DataStructure) {
  int identifier = 2;
  int width = 10;
  int height = 5;
  ShapeMatrix* matrix = new ShapeMatrix(identifier, width, height);

  EXPECT_EQ(identifier, matrix->getIdentifier());
  EXPECT_EQ(height, matrix->getHeight());
  EXPECT_EQ(width, matrix->getWidth());
  EXPECT_EQ(height * width, matrix->getMatrixArea());
  EXPECT_EQ(0, matrix->getShapeArea());

  delete matrix;
}

TEST(ShapeMatrixTest, CopyConstructor) {
  int identifier = 2;
  int width = 10;
  int height = 5;
  ShapeMatrix* matrix1 = new ShapeMatrix(identifier, width, height);
  ShapeMatrix* matrix2 = new ShapeMatrix(*matrix1);

  EXPECT_NE(matrix1, matrix2);

  delete matrix1;
  delete matrix2;
}

TEST(ShapeMatrixTest, SetGet) {
  int identifier = 2;
  int width = 10;
  int height = 5;
  ShapeMatrix* matrix = new ShapeMatrix(identifier, width, height);

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
  int identifier = 2;
  int width = 2;
  int height = 3;
  ShapeMatrix* matrix = new ShapeMatrix(identifier, width, height);
  ShapeMatrix* rotatedMatrix = NULL;

  matrix->set(0, 0, true);
  matrix->set(1, 0, true);
  matrix->set(2, 0, true);
  matrix->set(2, 1, true);
  // an L shape

  rotatedMatrix = matrix->rotate();
  EXPECT_EQ(height, rotatedMatrix->getWidth());
  EXPECT_EQ(width, rotatedMatrix->getHeight());
  EXPECT_TRUE(rotatedMatrix->get(0, 0));
  EXPECT_TRUE(rotatedMatrix->get(0, 1));
  EXPECT_TRUE(rotatedMatrix->get(0, 2));
  EXPECT_TRUE(rotatedMatrix->get(1, 0));
  delete rotatedMatrix;

  delete matrix;
}

TEST(ShapeMatrixTest, RotateOnce2) {
  int identifier = 2;
  int width = 2;
  int height = 3;
  ShapeMatrix* matrix = new ShapeMatrix(identifier, width, height);
  ShapeMatrix* rotatedMatrix = NULL;

  matrix->set(0, 0, true);
  matrix->set(1, 0, true);
  matrix->set(2, 0, true);
  matrix->set(2, 1, true);
  // an L shape

  rotatedMatrix = matrix->rotate(1);
  EXPECT_EQ(height, rotatedMatrix->getWidth());
  EXPECT_EQ(width, rotatedMatrix->getHeight());
  EXPECT_TRUE(rotatedMatrix->get(0, 0));
  EXPECT_TRUE(rotatedMatrix->get(0, 1));
  EXPECT_TRUE(rotatedMatrix->get(0, 2));
  EXPECT_TRUE(rotatedMatrix->get(1, 0));
  delete rotatedMatrix;

  delete matrix;
}

TEST(ShapeMatrixTest, RotateNegative) {
  int identifier = 2;
  int width = 2;
  int height = 3;
  ShapeMatrix* matrix = new ShapeMatrix(identifier, width, height);
  ShapeMatrix* rotatedMatrix = NULL;

  matrix->set(0, 0, true);
  matrix->set(1, 0, true);
  matrix->set(2, 0, true);
  matrix->set(2, 1, true);
  // an L shape

  rotatedMatrix = matrix->rotate(-1);
  EXPECT_EQ(NULL, rotatedMatrix);

  delete matrix;
}

TEST(ShapeMatrixTest, NoRotate) {
  int identifier = 2;
  int width = 2;
  int height = 3;
  ShapeMatrix* matrix = new ShapeMatrix(identifier, width, height);
  ShapeMatrix* rotatedMatrix = NULL;

  matrix->set(0, 0, true);
  matrix->set(1, 0, true);
  matrix->set(2, 0, true);
  matrix->set(2, 1, true);
  // an L shape

  rotatedMatrix = matrix->rotate(0);
  EXPECT_NE(nullptr, rotatedMatrix);
  EXPECT_NE(matrix, rotatedMatrix);
  EXPECT_EQ(width, rotatedMatrix->getWidth());
  EXPECT_EQ(height, rotatedMatrix->getHeight());

  int area = width * height;
  for (int i = 0; i < area; ++i) {
    EXPECT_EQ(matrix->get(i), rotatedMatrix->get(i));
  }

  delete rotatedMatrix;
  delete matrix;
}

TEST(ShapeMatrixTest, RotateTwice) {
  int identifier = 2;
  int width = 2;
  int height = 3;
  ShapeMatrix* matrix = new ShapeMatrix(identifier, width, height);
  ShapeMatrix* rotatedMatrix = NULL;

  matrix->set(0, 0, true);
  matrix->set(1, 0, true);
  matrix->set(2, 0, true);
  matrix->set(2, 1, true);
  // an L shape

  rotatedMatrix = matrix->rotate(2);
  EXPECT_EQ(width, rotatedMatrix->getWidth());
  EXPECT_EQ(height, rotatedMatrix->getHeight());
  EXPECT_TRUE(rotatedMatrix->get(0, 0));
  EXPECT_TRUE(rotatedMatrix->get(0, 1));
  EXPECT_TRUE(rotatedMatrix->get(1, 1));
  EXPECT_TRUE(rotatedMatrix->get(2, 1));
  delete rotatedMatrix;

  delete matrix;
}

TEST(ShapeMatrixTest, RotateThrice) {
  int identifier = 2;
  int width = 2;
  int height = 3;
  ShapeMatrix* matrix = new ShapeMatrix(identifier, width, height);
  ShapeMatrix* rotatedMatrix = NULL;

  matrix->set(0, 0, true);
  matrix->set(1, 0, true);
  matrix->set(2, 0, true);
  matrix->set(2, 1, true);
  // an L shape

  rotatedMatrix = matrix->rotate(3);
  EXPECT_EQ(height, rotatedMatrix->getWidth());
  EXPECT_EQ(width, rotatedMatrix->getHeight());
  EXPECT_TRUE(rotatedMatrix->get(1, 0));
  EXPECT_TRUE(rotatedMatrix->get(1, 1));
  EXPECT_TRUE(rotatedMatrix->get(1, 2));
  EXPECT_TRUE(rotatedMatrix->get(0, 2));
  delete rotatedMatrix;

  delete matrix;
}

TEST(ShapeMatrixTest, RotateFourTimes) {
  int identifier = 2;
  int width = 2;
  int height = 3;
  ShapeMatrix* matrix = new ShapeMatrix(identifier, width, height);
  ShapeMatrix* rotatedMatrix = NULL;

  matrix->set(0, 0, true);
  matrix->set(1, 0, true);
  matrix->set(2, 0, true);
  matrix->set(2, 1, true);
  // an L shape

  rotatedMatrix = matrix->rotate(4);
  EXPECT_NE(nullptr, rotatedMatrix);
  EXPECT_NE(matrix, rotatedMatrix);
  EXPECT_EQ(width, rotatedMatrix->getWidth());
  EXPECT_EQ(height, rotatedMatrix->getHeight());

  int area = width * height;
  for (int i = 0; i < area; ++i) {
    EXPECT_EQ(matrix->get(i), rotatedMatrix->get(i));
  }

  delete rotatedMatrix;
  delete matrix;
}

TEST(ShapeMatrixTest, Mirror) {
  int identifier = 2;
  int width = 2;
  int height = 3;
  ShapeMatrix* matrix = new ShapeMatrix(identifier, width, height);
  ShapeMatrix* mirroredMatrix = NULL;

  matrix->set(0, 0, true);
  matrix->set(1, 0, true);
  matrix->set(2, 0, true);
  matrix->set(2, 1, true);
  // an L shape

  mirroredMatrix = matrix->mirror();
  EXPECT_EQ(width, mirroredMatrix->getWidth());
  EXPECT_EQ(height, mirroredMatrix->getHeight());
  EXPECT_EQ(matrix->getMatrixArea(), mirroredMatrix->getMatrixArea());
  EXPECT_EQ(matrix->getShapeArea(), mirroredMatrix->getShapeArea());

  EXPECT_TRUE(mirroredMatrix->get(0, 0));
  EXPECT_TRUE(mirroredMatrix->get(0, 1));
  EXPECT_TRUE(mirroredMatrix->get(1, 0));
  EXPECT_TRUE(mirroredMatrix->get(2, 0));
}

TEST(ShapeMatrixTest, AssignmentCopy) {
  int identifier = 2;
  int width = 2;
  int height = 3;

  // ON STACK
  ShapeMatrix matrix1(identifier, width, height);

  matrix1.set(0, 0, true);
  matrix1.set(1, 0, true);
  matrix1.set(2, 0, true);
  matrix1.set(2, 1, true);
  // an L shape

  ShapeMatrix matrix2(identifier, width, height);
  EXPECT_NE(&matrix1, &matrix2);

  matrix2 = matrix1;
  EXPECT_NE(&matrix1, &matrix2);

  EXPECT_EQ(width, matrix2.getWidth());
  EXPECT_EQ(height, matrix2.getHeight());
  int area = width * height;
  for (int i = 0; i < area; ++i) {
    EXPECT_EQ(matrix1.get(i), matrix2.get(i));
  }
}

TEST(ShapeMatrixTest, AssignmentSelf) {
  int identifier = 2;
  int width = 2;
  int height = 3;

  // ON STACK
  ShapeMatrix matrix1(identifier, width, height);

  matrix1.set(0, 0, true);
  matrix1.set(1, 0, true);
  matrix1.set(2, 0, true);
  matrix1.set(2, 1, true);
  // an L shape

  // self assignment
  matrix1 = matrix1;
}

TEST(ShapeMatrixTest, EqualityCheckSelf) {
  int identifier = 2;
  int width = 2;
  int height = 3;

  // ON STACK
  ShapeMatrix matrix1(identifier, width, height);

  matrix1.set(0, 0, true);
  matrix1.set(1, 0, true);
  matrix1.set(2, 0, true);
  matrix1.set(2, 1, true);
  // an L shape

  // self equality check
  EXPECT_TRUE(matrix1 == matrix1);
  EXPECT_FALSE(matrix1 != matrix1);
}

TEST(ShapeMatrixTest, EqualityCheckCopy) {
  int identifier = 2;
  int width = 2;
  int height = 3;

  // ON STACK
  ShapeMatrix matrix1(identifier, width, height);

  matrix1.set(0, 0, true);
  matrix1.set(1, 0, true);
  matrix1.set(2, 0, true);
  matrix1.set(2, 1, true);
  // an L shape

  ShapeMatrix matrix2(matrix1);

  // equality with copy
  EXPECT_TRUE(matrix1 == matrix2);
  EXPECT_FALSE(matrix1 != matrix2);
}

TEST(ShapeMatrixTest, EqualityCheckIdentical) {
  int identifier = 2;
  int width = 2;
  int height = 3;

  // ON STACK
  ShapeMatrix matrix1(identifier, width, height);

  matrix1.set(0, 0, true);
  matrix1.set(1, 0, true);
  matrix1.set(2, 0, true);
  matrix1.set(2, 1, true);
  // an L shape

  ShapeMatrix matrix2(identifier, width, height);

  matrix2.set(0, 0, true);
  matrix2.set(1, 0, true);
  matrix2.set(2, 0, true);
  matrix2.set(2, 1, true);

  // equality with copy
  EXPECT_TRUE(matrix1 == matrix2);
  EXPECT_FALSE(matrix1 != matrix2);
}

TEST(ShapeMatrixTest, EqualityCheckDifferentSize) {
  int identifier = 2;
  int width = 2;
  int height = 3;

  // ON STACK
  ShapeMatrix matrix1(identifier, width, height);

  matrix1.set(0, 0, true);
  matrix1.set(1, 0, true);
  matrix1.set(2, 0, true);
  matrix1.set(2, 1, true);
  // an L shape

  ShapeMatrix matrix2(identifier, height, width);

  // inequality with different size
  EXPECT_FALSE(matrix1 == matrix2);
  EXPECT_TRUE(matrix1 != matrix2);
}

TEST(ShapeMatrixTest, EqualityCheckDifferentShapeArea) {
  int identifier = 2;
  int width = 2;
  int height = 3;

  // ON STACK
  ShapeMatrix matrix1(identifier, width, height);

  matrix1.set(0, 0, true);
  matrix1.set(1, 0, true);
  matrix1.set(2, 0, true);
  matrix1.set(2, 1, true);
  // an L shape

  ShapeMatrix matrix2(identifier, width, height);

  // inequality with different shape area
  EXPECT_FALSE(matrix1 == matrix2);
  EXPECT_TRUE(matrix1 != matrix2);
}

TEST(ShapeMatrixTest, EqualityCheckDifferentShape) {
  int identifier = 2;
  int width = 2;
  int height = 3;

  // ON STACK
  ShapeMatrix matrix1(identifier, width, height);

  matrix1.set(0, 0, true);
  matrix1.set(1, 0, true);
  matrix1.set(2, 0, true);
  matrix1.set(2, 1, true);
  // an L shape

  ShapeMatrix matrix2(identifier, width, height);

  matrix2.set(0, 0, true);
  matrix2.set(1, 0, true);
  matrix2.set(2, 0, true);
  matrix2.set(1, 1, true);

  // inequality with different shape
  EXPECT_FALSE(matrix1 == matrix2);
  EXPECT_TRUE(matrix1 != matrix2);
}

} // namespace
