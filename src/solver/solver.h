#ifndef APS_SOLVER_SOLVER_H_
#define APS_SOLVER_SOLVER_H_

#include "common/shape_matrix.h"
#include "common/puzzle_board.h"
#include "common/types.h"

#define UNDERSIZED -1
#define OVERSIZED -2
#define SOLVED 1
#define UNSOLVED 0

PuzzleBoard* createBoard(ListOfShapeMatrices* const matrices,
    ListOfShapeMatrices* const pieces, int& containerArea, int& totalPieceArea);

bool recursiveSolver (PuzzleBoard* board,
  ListOfShapeMatrices* const pieces,
  unsigned int currentIndex);

int** copyBoard(PuzzleBoard* board);

void deleteCopy(uint height, int** copyBoard);

int** puzzleSolver(ListOfShapeMatrices* const matrices, int& returnCode,
      uint& board_height, uint& board_width);

#endif
