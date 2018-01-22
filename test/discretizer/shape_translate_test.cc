
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

TEST(ShapeTranslateTest, shape_translate_all_shapes_Normal) {
  ListOfPoints shape1;
  shape1.push_back(Point(0, 0));
  shape1.push_back(Point(9, 0));
  shape1.push_back(Point(9, 9));
  shape1.push_back(Point(0, 9));

  ListOfPoints shape2;
  shape2.push_back(Point(0, 0));
  shape2.push_back(Point(9, 0));
  shape2.push_back(Point(9, 9));
  shape2.push_back(Point(6, 9));
  shape2.push_back(Point(6, 3));
  shape2.push_back(Point(6, 0));

  ListOfPoints shape3;
  shape3.push_back(Point(0, 0));
  shape3.push_back(Point(6, 0));
  shape3.push_back(Point(6, 6));
  shape3.push_back(Point(0, 6));

  ListOfShapes listOfShapes;
  listOfShapes.push_back(&shape1);
  listOfShapes.push_back(&shape2);
  listOfShapes.push_back(&shape3);
  
  ListOfShapeMatrices matrices;
  
  bool op_result = shape_translate_all_shapes(&listOfShapes, &matrices);
  EXPECT_TRUE(op_result);
}
} // namespace
