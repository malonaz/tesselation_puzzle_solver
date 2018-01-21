
#include <cstddef>
#include <gtest/gtest.h>
#include "common/point.h"
#include "common/shape_matrix.h"
#include "common/types.h"
#include "discretizer/shape_translate.h"

namespace {

TEST(ShapeTranslateTest, shape_translate_Normal) {
  ShapeMatrix* matrix = NULL;

  ListOfPoints shape;
  shape.push_back(Point(0, 0));
  shape.push_back(Point(3, 0));
  shape.push_back(Point(3, 3));
  shape.push_back(Point(2, 3));
  shape.push_back(Point(2, 2));
  shape.push_back(Point(1, 2));
  shape.push_back(Point(1, 3));
  shape.push_back(Point(0, 3));

  shape_translate(&shape, matrix);

  EXPECT_EQ(8, matrix->getShapeArea());
  EXPECT_TRUE(matrix->get(0, 0));
  EXPECT_TRUE(matrix->get(0, 1));
  EXPECT_TRUE(matrix->get(0, 2));

  EXPECT_TRUE(matrix->get(1, 0));
  EXPECT_TRUE(matrix->get(1, 1));
  EXPECT_TRUE(matrix->get(1, 2));

  EXPECT_TRUE(matrix->get(2, 0));
  EXPECT_FALSE(matrix->get(2, 1));
  EXPECT_TRUE(matrix->get(2, 2));

  delete matrix;
}

TEST(ShapeTranslateTest, shape_translate_NonNullPointer) {
  ShapeMatrix*  matrix = new ShapeMatrix(5, 5);

  ListOfPoints shape;
  shape.push_back(Point(0, 0));
  shape.push_back(Point(3, 0));
  shape.push_back(Point(3, 3));
  shape.push_back(Point(2, 3));
  shape.push_back(Point(2, 2));
  shape.push_back(Point(1, 2));
  shape.push_back(Point(1, 3));
  shape.push_back(Point(0, 3));

  // because matrix is not NULL, assert should cause this statement to fail
  EXPECT_DEATH(shape_translate(&shape, matrix),
    "Assertion.+matrix == NULL");
  delete matrix;
}

TEST(ShapeTranslateTest, shape_translate_ZeroWidth) {
  ShapeMatrix*  matrix = NULL;

  ListOfPoints shape;
  shape.push_back(Point(0, 2));

  // because matrix is not NULL, assert should cause this statement to fail
  shape_translate(&shape, matrix);
  EXPECT_EQ(NULL, matrix);
}

TEST(ShapeTranslateTest, shape_translate_ZeroHeight) {
  ShapeMatrix*  matrix = NULL;

  ListOfPoints shape;
  shape.push_back(Point(5, 0));

  // because matrix is not NULL, assert should cause this statement to fail
  shape_translate(&shape, matrix);
  EXPECT_EQ(NULL, matrix);
}

TEST(ShapeTranslateTest, shape_translate_MinPoints) {
  ShapeMatrix*  matrix = NULL;

  ListOfPoints shape;
  shape.push_back(Point(0, 0));
  shape.push_back(Point(0, 1));
  shape.push_back(Point(1, 0));

  // because matrix is not NULL, assert should cause this statement to fail
  shape_translate(&shape, matrix);
  EXPECT_EQ(NULL, matrix);
}

} // namespace
