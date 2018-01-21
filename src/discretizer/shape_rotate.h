#ifndef APS_DISCRETIZER_SHAPE_ROTATE_H_
#define APS_DISCRETIZER_SHAPE_ROTATE_H_

#include "common/shape_matrix.h"
#include "common/types.h"
#include <vector>

/**
 * Helper function which rotates the given shape's coordinates so that all sides 
 * of the given shape are vertical or horizontal.
 */
void rotate_shape(const ListOfPoints* const shape);




#endif
