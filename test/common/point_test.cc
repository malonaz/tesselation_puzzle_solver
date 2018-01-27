
#include <cstddef>
#include <gtest/gtest.h>
#include "common/point.h"

namespace {

//same distance, in opposite direction to that
TEST(PointTest, EqualDistOppDir) {
  int x1 = 8;
  int y1 = 24;
  int x2 = 1;
  int y2 = 3;
  Point* point1 = new Point(x1, y1);
  Point* point2 = new Point(x2, y2);
  float dist1 = point1->distanceTo(Point(7,21));
  float dist2 = point2->distanceTo(Point(2,6));

  EXPECT_EQ(dist1, dist2);

  delete point1;
  delete point2;
}

//same distance, in same direction to that
TEST(PointTest, EqualDistSaneDir) {
  int x1 = 7;
  int y1 = 21;
  int x2 = 1;
  int y2 = 3;
  Point* point1 = new Point(x1, y1);
  Point* point2 = new Point(x2, y2);
  float dist1 = point1->distanceTo(Point(8,24));
  float dist2 = point2->distanceTo(Point(2,6));

  EXPECT_EQ(dist1, dist2);

  delete point1;
  delete point2;
}

//same distance, negative coordinates
TEST(PointTest, EqualDistNegCoord) {
  int x1 = -1;
  int y1 = -3;
  int x2 = 1;
  int y2 = 3;
  Point* point1 = new Point(x1, y1);
  Point* point2 = new Point(x2, y2);
  float dist1 = point1->distanceTo(Point(0,0));
  float dist2 = point2->distanceTo(Point(0,0));

  EXPECT_EQ(dist1, dist2);

  delete point1;
  delete point2;
}

//different distance
TEST(PointTest, DiffDist) {
  int x1 = 8;
  int y1 = 24;
  int x2 = 1;
  int y2 = 3;
  Point* point1 = new Point(x1, y1);
  Point* point2 = new Point(x2, y2);
  float dist1 = point1->distanceTo(Point(0,0));
  float dist2 = point2->distanceTo(Point(13,7));

  EXPECT_NE(dist1, dist2);

  delete point1;
  delete point2;
}

//same slope, negative direction to that
TEST(PointTest, EqualSlopeOppDir) {
  int x1 = 8;
  int y1 = 24;
  int x2 = 1;
  int y2 = 3;
  Point* point1 = new Point(x1, y1);
  Point* point2 = new Point(x2, y2);
  float slope1 = point1->slopeTo(Point(0,0));
  float slope2 = point2->slopeTo(Point(2,6));

  EXPECT_EQ(slope1, slope2);

  delete point1;
  delete point2;
}

//same slope, negative coordinates
TEST(PointTest, EqualSlopeNegCoord) {
  int x1 = -1;
  int y1 = -3;
  int x2 = 1;
  int y2 = 3;
  Point* point1 = new Point(x1, y1);
  Point* point2 = new Point(x2, y2);
  float slope1 = point1->slopeTo(Point(0,0));
  float slope2 = point2->slopeTo(Point(0,0));

  EXPECT_EQ(slope1, slope2);

  delete point1;
  delete point2;
}

//reflected slope, negative coordinates
TEST(PointTest, EqualSlopeReflectNegated) {
  int x1 = -1;
  int y1 = 3;
  int x2 = 1;
  int y2 = 3;
  Point* point1 = new Point(x1, y1);
  Point* point2 = new Point(x2, y2);
  float slope1 = point1->slopeTo(Point(0,0));
  float slope2 = point2->slopeTo(Point(0,0));

  EXPECT_EQ(-slope1, slope2);

  delete point1;
  delete point2;
}

//reflected slope, negative coordinates
TEST(PointTest, DiffSlopeReflect) {
  int x1 = -1;
  int y1 = 3;
  int x2 = 1;
  int y2 = 3;
  Point* point1 = new Point(x1, y1);
  Point* point2 = new Point(x2, y2);
  float slope1 = point1->slopeTo(Point(0,0));
  float slope2 = point2->slopeTo(Point(0,0));

  EXPECT_NE(slope1, slope2);

  delete point1;
  delete point2;
}

//different slope, negative coordinates
TEST(PointTest, DiffSlope) {
  int x1 = 2;
  int y1 = 7;
  int x2 = 1;
  int y2 = 3;
  Point* point1 = new Point(x1, y1);
  Point* point2 = new Point(x2, y2);
  float slope1 = point1->slopeTo(Point(0,0));
  float slope2 = point2->slopeTo(Point(0,0));

  EXPECT_NE(slope1, slope2);

  delete point1;
  delete point2;
}


} // namespace
