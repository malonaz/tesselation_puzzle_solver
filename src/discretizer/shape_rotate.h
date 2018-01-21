#ifndef APS_DISCRETIZER_SHAPE_ROTATE_H_
#define APS_DISCRETIZER_SHAPE_ROTATE_H_

#include "common/shape_matrix.h"
#include "common/types.h"
#include <vector>



void rotate_shape(std::vector<Point> shape_points, std::vector<Point> rotated_shape_points);

void print_shape(std::vector<Point> shape_points);

#endif
