
#include <cstddef>
#include <gtest/gtest.h>
#include <vector>

#include "common/memory.h"
#include "common/point.h"
#include "common/shape_matrix.h"
#include "common/types.h"
#include "discretizer/shape_translate.h"

using std::vector;

namespace {

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
  shape2.push_back(Point(0, 3));

  ListOfPoints shape3;
  shape3.push_back(Point(0, 0));
  shape3.push_back(Point(6, 0));
  shape3.push_back(Point(6, 6));
  shape3.push_back(Point(0, 6));

  vector<ListOfPoints> listOfShapes;
  listOfShapes.push_back(shape1);
  listOfShapes.push_back(shape2);
  listOfShapes.push_back(shape3);

  vector<ShapeMatrix> matrices;

  bool op_result = shape_translate_all_shapes(listOfShapes, matrices);
  EXPECT_TRUE(op_result);

  EXPECT_EQ(9, matrices[0].getMatrixArea());
  EXPECT_EQ(9, matrices[0].getShapeArea());
  EXPECT_TRUE(matrices[0].get(0, 0));
  EXPECT_TRUE(matrices[0].get(0, 1));
  EXPECT_TRUE(matrices[0].get(0, 2));

  EXPECT_TRUE(matrices[0].get(1, 0));
  EXPECT_TRUE(matrices[0].get(1, 1));
  EXPECT_TRUE(matrices[0].get(1, 2));

  EXPECT_TRUE(matrices[0].get(2, 0));
  EXPECT_TRUE(matrices[0].get(2, 1));
  EXPECT_TRUE(matrices[0].get(2, 2));

  EXPECT_EQ(9, matrices[1].getMatrixArea());
  EXPECT_EQ(5, matrices[1].getShapeArea());
  EXPECT_TRUE(matrices[1].get(0, 0));
  EXPECT_TRUE(matrices[1].get(0, 1));
  EXPECT_TRUE(matrices[1].get(0, 2));

  EXPECT_FALSE(matrices[1].get(1, 0));
  EXPECT_FALSE(matrices[1].get(1, 1));
  EXPECT_TRUE(matrices[1].get(1, 2));

  EXPECT_FALSE(matrices[1].get(2, 0));
  EXPECT_FALSE(matrices[1].get(2, 1));
  EXPECT_TRUE(matrices[1].get(2, 2));

  EXPECT_EQ(4, matrices[2].getMatrixArea());
  EXPECT_EQ(4, matrices[2].getShapeArea());
  EXPECT_TRUE(matrices[2].get(0, 0));
  EXPECT_TRUE(matrices[2].get(0, 1));

  EXPECT_TRUE(matrices[2].get(1, 0));
  EXPECT_TRUE(matrices[2].get(1, 1));
}

TEST(ShapeTranslateTest, shape_translate_all_shapes_Empty) {
  vector<ListOfPoints> listOfShapes;
  vector<ShapeMatrix> matrices;

  bool op_result = shape_translate_all_shapes(listOfShapes, matrices);
  EXPECT_FALSE(op_result);
  EXPECT_EQ(0, matrices.size());
}

TEST(ShapeTranslateTest, shape_translate_all_shapes_InvalidShape) {
  vector<ListOfPoints> listOfShapes;
  vector<ShapeMatrix> matrices;

  ListOfPoints shape1;
  shape1.push_back(Point(0, 0));

  listOfShapes.push_back(shape1);

  bool op_result = shape_translate_all_shapes(listOfShapes, matrices);
  EXPECT_FALSE(op_result);
  EXPECT_EQ(0, matrices.size());
}

TEST(ShapeTranslateTest, shape_translate_all_shapes_InvalidShape2) {
  vector<ListOfPoints> listOfShapes;
  vector<ShapeMatrix> matrices;

  ListOfPoints shape1;
  shape1.push_back(Point(0, 0));

  ListOfPoints shape2;
  shape2.push_back(Point(0, 0));
  shape2.push_back(Point(6, 0));
  shape2.push_back(Point(6, 6));
  shape2.push_back(Point(0, 6));

  listOfShapes.push_back(shape1);
  listOfShapes.push_back(shape2);

  bool op_result = shape_translate_all_shapes(listOfShapes, matrices);
  EXPECT_FALSE(op_result);
  EXPECT_EQ(0, matrices.size());
}

TEST(ShapeTranslateTest, shape_translate_all_shapes_InvalidShape3) {
  vector<ListOfPoints> listOfShapes;
  vector<ShapeMatrix> matrices;

  ListOfPoints shape1;
  shape1.push_back(Point(0, 0));
  shape1.push_back(Point(0, 0));
  shape1.push_back(Point(0, 0));
  shape1.push_back(Point(0, 0));

  listOfShapes.push_back(shape1);

  bool op_result = shape_translate_all_shapes(listOfShapes, matrices);
  EXPECT_FALSE(op_result);
  EXPECT_EQ(0, matrices.size());
}
} // namespace
