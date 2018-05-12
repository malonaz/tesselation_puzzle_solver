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

  bool placePiece(int x, int y, const ShapeMatrix &piece);

  bool removePiece(int x, int y, const ShapeMatrix &piece);

  int** getCurrentBoard() const;

  uint getRemainingArea() const;

  ShapeMatrix getContainer() const;

  PuzzleBoard& operator=(const PuzzleBoard& rhs);

  virtual ~PuzzleBoard();
};

#endif
