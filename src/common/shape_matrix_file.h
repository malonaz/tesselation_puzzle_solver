#ifndef APS_COMMON_SHAPE_MATRIX_FILE_H_
#define APS_COMMON_SHAPE_MATRIX_FILE_H_

#include <vector>
#include "ShapeMatrix.h"

using namespace std;

void shape_matrix_read(const char* filename, vector<ShapeMatrix*>* const list);

#endif