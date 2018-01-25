#ifndef APS_PUZZLE_SOLVING_SOLVER_H_
#define APS_PUZZLE_SOLVING_SOLVER_H_

#include "common/shape_matrix.h"
#include "common/puzzle_board.h"
#include "common/types.h"


PuzzleBoard* createBoard(ListOfShapeMatrices* const matrices,
    ListOfShapeMatrices* const pieces, int& containerArea, int& totalPieceArea);

bool recursiveSolver (PuzzleBoard* board,
  ListOfShapeMatrices* const pieces,
  unsigned int currentIndex);

bool puzzleSolver(ListOfShapeMatrices* const matrices);

#endif
