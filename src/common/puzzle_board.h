#ifndef APS_COMMON_PUZZLE_BOARD_H_
#define APS_COMMON_PUZZLE_BOARD_H_

#include "shape_matrix.h"
#include "types.h"

class PuzzleBoard {
private:
  ShapeMatrix* container;
  int** current_board;
  uint64 bitset[64];
  int remainingArea;

public:
  PuzzleBoard(ShapeMatrix* shape);

  PuzzleBoard(const PuzzleBoard &copy);

  uint getHeight() const;

  uint getWidth() const;

  bool placePiece(int x, int y, uint idx, uint64 mask[64], ShapeMatrix* piece);

  bool removePiece(int x, int y, uint64 mask[64], ShapeMatrix* piece);

  int** getCurrentBoard() const;

  uint getRemainingArea() const;

  void printBoard() const;

  PuzzleBoard& operator=(const PuzzleBoard& rhs);

  virtual ~PuzzleBoard();
};

#endif
