#ifndef APS_DISCRETIZER_SHAPE_TRANSLATE_H_
#define APS_DISCRETIZER_SHAPE_TRANSLATE_H_

#include <vector>
#include "common/shape_matrix.h"
#include "common/types.h"

using namespace std;

void shape_translate(const ListOfPoints* const shape, ShapeMatrix* &matrix);
bool shape_translate_all_shapes(ListOfShapes* const shapes,
  vector<ShapeMatrix*>* const matrices);

#endif
