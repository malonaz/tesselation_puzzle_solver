#include "solver.h"
#include "../common/puzzle_board.h"
#include "../common/shape_matrix.h"

/*
1) Have a double-for loop that cycles through the coordinates of the current puzzleboard
2) Try PuzzleBoard::placePiece(int x, int y, int idx, ShapeMatrix* piece),
if false, rotate (up to 3 times) and try again,
if all 4 orientations return false, go to the next coordinate on the board and try again

3) if placePiece is successful,


*/

PuzzleBoard* createBoard(vector<ShapeMatrix*>* const matrices, vector<ShapeMatrix*>* const pieces) {
  int maxArea = matrices[0]->getShapeArea();
  int maxAreaIdx = 0;

  for (int i = 1; i < matrices->size(); i++ ) {
    int tempArea = matrices[i]->getShapeArea();
    int pushIdx = i;
    if (tempArea > maxArea) {
      //places the previous largest piece into the puzzle pieces vector
      pushIdx = maxAreaIdx;
      maxArea = tempArea;
      maxAreaIdx = i;
    }
    pieces->push_back(matrices[pushIdx]);
  }

  PuzzleBoard* board = new PuzzleBoard(matrices[maxAreaIdx]);

  return board;
}

bool recursiveSolver (PuzzleBoard* board, vector<ShapeMatrix*>* const pieces, int currentIndex, int x, int y) {
  if (board->getRemainingArea() == 0 && currentIndex == pieces->size()) {
    //the board is complete, and no more remaining pieces
    return true;
  }
  if (board->getRemainingArea() == 0 && currentIndex < pieces->size()) {
    //the board is complete, but there are remaining pieces
    return false;
  }
  if (board->getRemainingArea() > 0 && currentIndex == pieces->size()) {
    //the board is incomplete, but there are no more remaining pieces
    return false;
  }

  for (int r = 0; r < board->height ; r){

    for () {
    }
  }



  return false;
}

bool puzzleSolver(vector<ShapeMatrix*>* const matrices) {

  vector<ShapeMatrix*>* pieces;
  PuzzleBoard* board = createBoard(matrices, pieces);
  bool success = recursiveSolver(board, pieces, 0,0);

  return success;
}
