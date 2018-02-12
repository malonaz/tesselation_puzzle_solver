#ifndef APS_SOLVER_SOLVER_H_
#define APS_SOLVER_SOLVER_H_

#include "common/shape_matrix.h"
#include "common/puzzle_board.h"
#include "common/types.h"

#define UNDERSIZED -1
#define OVERSIZED -2
#define SOLVED 1
#define UNSOLVED 0

PuzzleBoard* createBoard(const vector<ShapeMatrix> &matrices,
    vector<ShapeMatrix> &pieces, int& containerArea, int& totalPieceArea);

// bool recursiveSolver (PuzzleBoard* board,
//   const vector<ShapeMatrix> &pieces,
//   uint currentIndex);

int** copyBoard(PuzzleBoard* board);

void deleteCopy(uint height, int** copyBoard);

int** puzzleSolver(const vector<ShapeMatrix> &matrices, int& returnCode,
      uint& board_height, uint& board_width);

#endif
