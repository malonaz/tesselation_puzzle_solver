#ifndef APS_PUZZLE_SOLVING_SOLVER_H
#define APS_PUZZLE_SOLVING_SOLVER_H

#include <vector>
#include "../common/shape_matrix.h"
#include "../common/puzzle_board.h"

using namespace std;

PuzzleBoard* createBoard(vector<ShapeMatrix*>* const matrices,
  vector<ShapeMatrix*>* const pieces);

bool recursiveSolver (PuzzleBoard* board,
  vector<ShapeMatrix*>* const pieces,
  unsigned int currentIndex);

bool puzzleSolver(vector<ShapeMatrix*>* const matrices);

#endif
