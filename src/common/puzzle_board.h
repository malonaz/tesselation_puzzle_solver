#ifndef APS_COMMON_PUZZLE_BOARD_H_
#define APS_COMMON_PUZZLE_BOARD_H_

#include "shape_matrix.h"
#include <fstream>
#include <iostream>


class PuzzleBoard {
private:
  ShapeMatrix* container;
  int** current_board;
  int remainingArea;

public:
  PuzzleBoard(ShapeMatrix* shape);

  PuzzleBoard(const PuzzleBoard &copy);

  int getHeight() const;

  int getWidth() const;

  bool placePiece(int x, int y, int idx, ShapeMatrix* piece);

  bool removePiece(int x, int y, int indexToRemove, ShapeMatrix* piece);

  int** getCurrentBoard() const;

  int getRemainingArea() const;

  void printBoard() const;

  void printBoard(std::ostream& os) const;

  PuzzleBoard& operator=(const PuzzleBoard& rhs);

  virtual ~PuzzleBoard();
};

#endif
