#ifndef APS_DISCRETIZER_SHAPE_TRANSLATE_H_
#define APS_DISCRETIZER_SHAPE_TRANSLATE_H_

#include <vector>
#include "../common/shape_matrix.h"

using namespace std;

typedef vector<int*> ListOfPoints;
typedef vector<int*> ListOfEdges;
typedef vector<ListOfPoints*> ListOfShapes;

void shape_translate(ListOfPoints* const shape, ShapeMatrix* &matrix);

#endif
