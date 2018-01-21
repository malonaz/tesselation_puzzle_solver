#ifndef APS_DISCRETIZER_SHAPE_ROTATE_H_
#define APS_DISCRETIZER_SHAPE_ROTATE_H_

#include "common/shape_matrix.h"
#include "common/types.h"
#include <vector>


void rotate_shapes(std::vector< std::vector<Point> > list_shape_points,
		   std::vector< std::vector<Point> > list_rotated_shape_points);

void rotate_shape(const std::vector<Point> shape_points, std::vector<Point> rotated_shape_points);


void print_shape(const std::vector<Point> shape_points);

#endif

