#include "solver/imageProcessor.h"
#include "common/shape_matrix.h"
#include "common/shape_matrix_io.h"
#include "solver/solver.h"
#include "solver/solutionProcessor.h"

#include <iostream>

using namespace std;

int main(int argc, char** argv){
  ShapeMatrix* shape = new ShapeMatrix(4,3);
  shape->set(0,0, true);
  shape->set(0,1, true);
  shape->set(0,2, true);
  shape->set(0,3, true);
  shape->set(1,0, true);
  shape->set(2,0, true);

  shape_matrix_print(shape);
  cout << endl << endl;

  ShapeMatrix* r_shape;
  r_shape = shape->rotate(2);
  cout << endl << endl;
  shape_matrix_print(r_shape);

  return 0;

}
