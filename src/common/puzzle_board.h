#ifndef APS_COMMON_PUZZLE_BOARD_H_
#define APS_COMMON_PUZZLE_BOARD_H_

#include "shape_matrix.h"
#include <fstream>
#include <iostream>


class PuzzleBoard {
private:
  ShapeMatrix container;
  int** current_board;
  uint remainingArea;

public:
  PuzzleBoard(const ShapeMatrix &shape);

  PuzzleBoard(int* intArray, const ShapeMatrix &shape);

  PuzzleBoard(const PuzzleBoard &copy);

  uint getHeight() const;

  uint getWidth() const;

  bool placePiece(int x, int y, int idx, const ShapeMatrix &piece);

  bool removePiece(int x, int y, int indexToRemove, const ShapeMatrix &piece);

  int** getCurrentBoard() const;

  uint getRemainingArea() const;

  PuzzleBoard& operator=(const PuzzleBoard& rhs);

  virtual ~PuzzleBoard();
};

#endif
