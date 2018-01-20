#include "solver.h"
#include <vector>
#include "../common/shape_matrix.h"
#include "../common/puzzle_board.h"
#include "../common/shape_matrix_io.h"
#include <iostream>

using namespace std;

PuzzleBoard* createBoard(vector<ShapeMatrix*>* const matrices, vector<ShapeMatrix*>* const pieces) {
  int maxArea = (*matrices)[0]->getShapeArea();
  int maxAreaIdx = 0;

  for (int i = 1; i < matrices->size(); i++ ) {
    int tempArea = (*matrices)[i]->getShapeArea();
    int pushIdx = i;
    if (tempArea > maxArea) {
      //places the previous largest piece into the puzzle pieces vector
      pushIdx = maxAreaIdx;
      maxArea = tempArea;
      maxAreaIdx = i;
    }
    pieces->push_back((*matrices)[pushIdx]);
  }

  PuzzleBoard* board = new PuzzleBoard((*matrices)[maxAreaIdx]);
  return board;
}

bool recursiveSolver (PuzzleBoard* board, vector<ShapeMatrix*>* const pieces, int currentIndex) {
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

  for (int r = 0; r < board->getHeight(); r++) {
    for (int c = 0; c < board->getWidth(); c++) {
      ShapeMatrix* temp = (*pieces)[currentIndex];
      for (int rotation = 0; rotation < 4; rotation ++){
        ShapeMatrix* r_temp;
        r_temp = temp->rotate(rotation);
        if (board->placePiece(c,r,(currentIndex+1),r_temp)) {
          if(recursiveSolver(board,pieces, currentIndex+1)) {
            return true;
          }
        board->placePiece(c, r, 0, r_temp); //reversing
        }
      }
    }
  }


  return false;
}

bool puzzleSolver(vector<ShapeMatrix*>* const matrices) {
  ListOfShapes shapes;
  ListOfShapes* pieces = &shapes;

  PuzzleBoard* board = createBoard(matrices, pieces);
      board->printBoard();
  bool success = recursiveSolver(board, pieces, 0);

  if (success){
    board->printBoard();
  }
      board->printBoard();
  cout<<success<<"  test 5....."<<endl;
  return success;
}
