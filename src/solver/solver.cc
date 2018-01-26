#include <iostream>
#include <vector>
#include "common/shape_matrix.h"
#include "common/puzzle_board.h"
#include "common/shape_matrix_io.h"
#include "common/types.h"
#include "solver.h"

using namespace std;

PuzzleBoard* createBoard(ListOfShapeMatrices* const matrices,
    ListOfShapeMatrices* const pieces, int& containerArea, int& totalPieceArea) {
  int maxArea = (*matrices)[0]->getShapeArea();
  int maxAreaIdx = 0;
  int accumArea = 0;
  int matricesSize = (int)matrices->size();

  for (int i = 1; i < matricesSize; i++) {
    int tempArea = (*matrices)[i]->getShapeArea();
    int pushIdx = i;
    if (tempArea > maxArea) {
      //places the previous largest piece into the puzzle pieces vector
      pushIdx = maxAreaIdx;
      accumArea += maxArea;
      maxArea = tempArea;
      maxAreaIdx = i;
      accumArea -= tempArea;
    }
    accumArea += tempArea;
    pieces->push_back((*matrices)[pushIdx]);
  }

  PuzzleBoard* board = new PuzzleBoard((*matrices)[maxAreaIdx]);
  containerArea = maxArea; // return container area as a reference
  totalPieceArea = accumArea; // return total area of puzzle pieces
  return board;
}

bool recursiveSolver (PuzzleBoard* board,
    ListOfShapeMatrices* const pieces,
    unsigned int currentIndex) {
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
      for (int rotation = 0; rotation < 4; rotation++) {
        ShapeMatrix* r_temp;
        r_temp = temp->rotate(rotation);
        if (board->placePiece(c, r, (currentIndex + 1), r_temp)) {
          if (recursiveSolver(board, pieces, currentIndex + 1)) {
            return true;
          }
          board->removePiece(c, r, currentIndex + 1); //reversing
        }
        delete r_temp;
      }
    }
  }

  return false;
}

int puzzleSolver(ListOfShapeMatrices* const matrices) {
  int returnCode = 0;
  ListOfShapeMatrices shapes;
  int containerArea =0;
  int totalPieceArea  =0;
  PuzzleBoard* board = createBoard(matrices, &shapes,
      containerArea, totalPieceArea);
  if (totalPieceArea > containerArea) { // case of undersized container
    return UNDERSIZED;
  }
  if (totalPieceArea < containerArea){ // case of oversized container
    return OVERSIZED;
  }
  // if puzzle pieces area == container area
  bool success = recursiveSolver(board, &shapes, 0);

  if (success) {
    returnCode = SOLVED;
    cout<<endl;
    board->printBoard();
  } else {
    returnCode = UNSOLVED;
  }
  delete board;
  return returnCode;
}
