#include "solver/imageProcessor.h"
#include "common/shape_matrix.h"
#include "common/shape_matrix_io.h"
#include "puzzle_solving/solver.h"
#include "solver/solutionProcessor.h"

#include <iostream>

using namespace std;

int main(int argc, char** argv){
//cout<<"Testing....."<<endl;
ListOfShapes shapes;
ListOfShapes* matrices = &shapes;
//cout<<"Testing 2....."<<endl;

  ShapeMatrix* shape1 = new ShapeMatrix(4,3);
  shape1->set(0,0, true);
  shape1->set(0,1, true);
  shape1->set(0,2, true);
  shape1->set(0,3, true);
  shape1->set(1,0, true);
  shape1->set(2,0, true);
  shape1 = shape1->rotate(1);
  cout<<"Shape 1....."<<endl;
  matrices->push_back(shape1);
  shape_matrix_print(shape1);

  ShapeMatrix* shape2 = new ShapeMatrix(3,1);
  shape2->set(0,0, true);
  shape2->set(0,1, true);
  shape2->set(0,2, true);
  matrices->push_back(shape2);
  cout<<"Shape 2....."<<endl;
  shape_matrix_print(shape2);

  ShapeMatrix* shape3 = new ShapeMatrix(3,1);
  shape3->set(0,0, true);
  shape3->set(0,1, true);
  shape3->set(0,2, true);
  shape3 = shape3->rotate(1);
  matrices->push_back(shape3);
  cout<<"Shape 3....."<<endl;
  shape_matrix_print(shape3);

  ShapeMatrix* shape4 = new ShapeMatrix(4,3);
  shape4->set(0,0, true);
  shape4->set(0,1, true);
  shape4->set(0,2, true);
  shape4->set(0,3, true);
  shape4->set(1,0, true);
  shape4->set(1,1, true);
  shape4->set(1,2, true);
  shape4->set(1,3, true);
  shape4->set(2,0, true);
  shape4->set(2,1, true);
  shape4->set(2,2, true);
  shape4->set(2,3, true);
  matrices->push_back(shape4);
  cout<<"Shape 4....."<<endl;
  shape_matrix_print(shape4);

  puzzleSolver(matrices);


  //shape_matrix_print(shape);
  //cout << endl << endl;

  // ShapeMatrix* r_shape;
  // r_shape = shape->rotate(2);
  // cout << endl << endl;
  // shape_matrix_print(r_shape);

  return 0;

}
