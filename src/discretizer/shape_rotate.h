#ifndef APS_DISCRETIZER_SHAPE_ROTATE_H_
#define APS_DISCRETIZER_SHAPE_ROTATE_H_

#include "common/shape_matrix.h"
#include "common/types.h"

#include <vector>

using std::vector;

/**
 * This function rotates these shapes such that the shapes's sides are vertical or horizontal. 
 * At this stage, shapes are represented by a list of of point coordinates. 
 *  @params:
 *   shapes: the shapes to be rotated
 *   rotated_shapes: output parameter where rotated shapes will be stored
 */
void rotate_shapes(const vector<ListOfPoints> &shapes, vector<ListOfPoints> &rotated_shapes);

#endif
