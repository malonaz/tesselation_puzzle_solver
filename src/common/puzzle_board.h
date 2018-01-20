#ifndef APS_COMMON_PUZZLE_BOARD_H_
#define APS_COMMON_PUZZLE_BOARD_H_

#include "shape_matrix.h"

class PuzzleBoard {
private:
  ShapeMatrix* container;
  int** current_board;

public:
  PuzzleBoard(ShapeMatrix* shape);

  PuzzleBoard(const PuzzleBoard &copy);

  int getHeight();

  int getWidth();

  bool placePiece(int x, int y, int idx, ShapeMatrix* piece);

  bool removePiece(int x, int y, int indexToRemove);

  int** getCurrentBoard();

  int getRemainingArea();

  void printBoard();

  virtual ~PuzzleBoard();
};

#endif
