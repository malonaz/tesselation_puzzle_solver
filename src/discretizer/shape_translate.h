#ifndef APS_DISCRETIZER_SHAPE_TRANSLATE_H_
#define APS_DISCRETIZER_SHAPE_TRANSLATE_H_

#include "common/shape_matrix.h"
#include "common/types.h"

#include <vector>

using std::vector;

/**
  Translate a given shape (defined by a list of (x, y)-coordinates) into
  a ShapeMatrix (defined by a logical matrix).

  \param shape The list of int[2] that defines the shape.
  \param matrix The pointer to the output matrix.
    Must be set to NULL before calling this method.
*/
void shape_translate(const vector<Point>* const shape, ShapeMatrix* &matrix);

/**
  Translate a list of shapes (each defined by a list of (x, y)-coordinates)
  into a list of ShapeMatrix (each defined by a logical matrix).

  \param shapes The list of shapes (each is a list of int[2]) that defines all
    the puzzle pieces in the image.
  \param matrices The list of output matrices translated from the list of shapes.
*/
bool shape_translate_all_shapes(const vector<vector<Point>*>* const shapes,
  vector<ShapeMatrix*>* const matrices);

#endif
