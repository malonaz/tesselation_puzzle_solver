#ifndef APS_COMMON_PUZZLE_BOARD_H_
#define APS_COMMON_PUZZLE_BOARD_H_

#include "shape_matrix.h"

class PuzzleBoard {
private:
  ShapeMatrix* container;
  int** current_board;

public:
  PuzzleBoard(ShapeMatrix* shape);

  bool placePiece(int x, int y, int idx, ShapeMatrix* piece);

  int** getCurrentBoard();

  int getRemainingArea();

  virtual ~PuzzleBoard();
};

#endif