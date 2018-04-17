#ifndef APS_DISCRETIZER_SHAPE_TRANSLATE_H_
#define APS_DISCRETIZER_SHAPE_TRANSLATE_H_

#include "common/shape_matrix.h"
#include "common/types.h"

#include <vector>

using std::vector;


/**
 * Translate a list of shapes (each represented as a list of points) 
 * into a list of ShapeMatrix (each defined by a logical matrix).
 *  @params:
 *   shapes: list of shapes to be translated, as list of points.
 *   matrices: output parameter containing the translated shapes as logical matrices.
 */
bool shape_translate_all_shapes(const vector<ListOfPoints>& shapes,
				vector<ShapeMatrix>& matrices);

#endif
