#ifndef APS_DISCRETIZER_SHAPE_ROTATE_H_
#define APS_DISCRETIZER_SHAPE_ROTATE_H_

#include <vector>
#include "common/shape_matrix.h"
#include "common/types.h"

using std::vector;

void rotate_shapes(vector< vector<Point> > list_shape_points,
		   vector< vector<Point> > list_rotated_shape_points);

void rotate_shape(const vector<Point> shape_points, vector<Point> rotated_shape_points);


void print_shape(const vector<Point> shape_points);

#endif

