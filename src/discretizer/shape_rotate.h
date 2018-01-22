#ifndef APS_DISCRETIZER_SHAPE_ROTATE_H_
#define APS_DISCRETIZER_SHAPE_ROTATE_H_

#include "common/shape_matrix.h"
#include "common/types.h"
#include <vector>


/**
 * At this stage, shapes are represented by a list of of point coordinates. This function
 * rotates these shapes such that the shapes's sides are vertical or horizontal. These
 * new shapes are stored in output parameter rotated shapes.
 */
void rotate_shapes(std::vector< std::vector<Point> > &shapes,
		   std::vector< std::vector<Point> > &rotated_shapes);


#endif

