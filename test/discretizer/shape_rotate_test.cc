#include <cstddef>
#include <gtest/gtest.h>
#include "common/point.h"
#include "common/types.h"
#include "image_processor/discretizer/shape_rotate.h"

namespace {

TEST(ShapeRotateTest, shape_rotate_fig1) {

  ListOfPoints shape;
  shape.push_back(Point(0, 0));
  shape.push_back(Point(1, 1));
  shape.push_back(Point(2, 0));
  shape.push_back(Point(1, -1));

  ListOfShapes shapes;
  shapes.push_back(&shape);
  ListOfShapes rotated_shapes;

  rotate_shapes(&shapes, &rotated_shapes);

  ListOfPoints* rotated_shape = rotated_shapes[0];
  EXPECT_TRUE((*rotated_shape)[0] == Point(0, 0));
  EXPECT_TRUE((*rotated_shape)[1] == Point(1, 0));
  EXPECT_TRUE((*rotated_shape)[2] == Point(1, -1));
  EXPECT_TRUE((*rotated_shape)[3] == Point(0, -1));
}


TEST(ShapeRotateTest, shape_rotate_fig2) {
  ListOfPoints shape;
  shape.push_back(Point(0, 0));
  shape.push_back(Point(-2, -2));
  shape.push_back(Point(-1, -3));
  shape.push_back(Point(-2, -4));
  shape.push_back(Point(-4, -2));
  shape.push_back(Point(-1, 1));


  ListOfShapes shapes;
  shapes.push_back(&shape);
  ListOfShapes rotated_shapes;
  rotate_shapes(&shapes, &rotated_shapes);

  ListOfPoints* rotated_shape = rotated_shapes[0];
  EXPECT_TRUE((*rotated_shape)[0] == Point(0, 0));
  EXPECT_TRUE((*rotated_shape)[1] == Point(3, 0));
  EXPECT_TRUE((*rotated_shape)[2] == Point(3, 1));
  EXPECT_TRUE((*rotated_shape)[3] == Point(4, 1));
  EXPECT_TRUE((*rotated_shape)[4] == Point(4, -2));
  EXPECT_TRUE((*rotated_shape)[5] == Point(0, -2));
}


TEST(ShapeRotateTest, already_rotated_shape) {
  ListOfPoints shape;
  shape.push_back(Point(0, 0));
  shape.push_back(Point(3, 0));
  shape.push_back(Point(3, 1));
  shape.push_back(Point(4, 1));
  shape.push_back(Point(4, -2));
  shape.push_back(Point(0, -2));


  ListOfShapes shapes;
  shapes.push_back(&shape);
  ListOfShapes rotated_shapes;
  rotate_shapes(&shapes, &rotated_shapes);

  ListOfPoints* rotated_shape = rotated_shapes[0];
  EXPECT_TRUE((*rotated_shape)[0] == Point(0, 0));
  EXPECT_TRUE((*rotated_shape)[1] == Point(3, 0));
  EXPECT_TRUE((*rotated_shape)[2] == Point(3, 1));
  EXPECT_TRUE((*rotated_shape)[3] == Point(4, 1));
  EXPECT_TRUE((*rotated_shape)[4] == Point(4, -2));
  EXPECT_TRUE((*rotated_shape)[5] == Point(0, -2));
}


} // namespace
