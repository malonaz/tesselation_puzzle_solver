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

ListOfShapeMatrices* combinations(ShapeMatrix* temp, int& num_orientations) {
  ListOfShapeMatrices* combi = new ListOfShapeMatrices;
  ShapeMatrix* r_temp = temp;

  for (int i = 0; i < 8; i++) {
    bool doAddTheShape = true;
    for (int j = 0; j < num_orientations; j++) {
      if (*r_temp == *((*combi)[j])) {
        doAddTheShape = false;
        break;
      }
    } //rotate and check

    if (!doAddTheShape) {
      continue;
    }

    combi->push_back(r_temp);
    r_temp = r_temp->rotate();
    if (i == 3) {
      r_temp = r_temp->mirror();
    }
  }

  num_orientations = combi->size();
  return combi;
}

bool recursiveSolver (PuzzleBoard* board,
    ListOfShapeMatrices* const pieces,
    uint currentIndex) {
  if (board->getRemainingArea() == 0 || currentIndex >= pieces->size()) {
    //the board is complete, and no more remaining pieces
    return true;
  }

  ShapeMatrix* temp = (*pieces)[currentIndex];
  int num_orientations = 0;
  ListOfShapeMatrices* shapesList = combinations(temp, num_orientations);
  int nextIndex = currentIndex + 1;

  for (int r = 0; r < board->getHeight(); r++) {
    for (int c = 0; c < board->getWidth(); c++) {
      for (int counteri = 0; counteri < num_orientations; counteri++) {
        ShapeMatrix* r_temp = (*shapesList)[counteri];
        if (board->placePiece(c, r, nextIndex, r_temp)) {
          if (recursiveSolver(board, pieces, nextIndex)) {
            return true;
          }
          board->removePiece(c, r, nextIndex, r_temp); // revert
        }
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
  if (totalPieceArea < containerArea) { // case of oversized container
    return OVERSIZED;
  }
  // if puzzle pieces area == container area
  bool success = recursiveSolver(board, &shapes, 0);

  if (success) {
    returnCode = SOLVED;
    // cout << endl;
    board->printBoard();
  } else {
    returnCode = UNSOLVED;
  }
  delete board;
  return returnCode;
}
