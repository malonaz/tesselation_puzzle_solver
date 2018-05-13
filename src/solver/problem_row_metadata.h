#ifndef APS_SOLVER_PROBLEM_ROW_METADATA_H_
#define APS_SOLVER_PROBLEM_ROW_METADATA_H_

#include "common/shape_matrix.h"

struct ProblemRowMetaData {
  ShapeMatrix shape;
  uint row;
  uint col;
};

#endif
