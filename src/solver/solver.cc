#include <iostream>
#include <vector>
#include <cmath>
#include "common/memory.h"
#include "common/puzzle_board.h"
#include "common/shape_matrix.h"
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

bool isShapeMatrixInList(const ShapeMatrix* const shape,
    const ListOfShapeMatrices* const list) {
  bool result = false;
  for (uint j = 0; j < list->size(); j++) {
    if (*shape == *((*list)[j])) {
      result = true;
      break;
    }
  }
  return result;
}

ListOfShapeMatrices* combinations(ShapeMatrix* temp) {
  ListOfShapeMatrices* combi = new ListOfShapeMatrices();
  ShapeMatrix* r_temp = temp;

  for (uint i = 0; i < 8; i++) {
    if (!isShapeMatrixInList(r_temp, combi)) {
      combi->push_back(r_temp);
    }

    r_temp = r_temp->rotate();
    if (i == 3) {
      r_temp = r_temp->mirror();
    }
  }
  return combi;
}

/* function to get empty area */
int getAdjacentEmptyArea(uint r, uint c, uint height, uint width, int** copiedBoard) {
  if (r < height && c < width) {
    if (copiedBoard[r][c] == 0) {
      copiedBoard[r][c] = -1;
      return 1 + getAdjacentEmptyArea(r, c + 1, height, width, copiedBoard)
        + getAdjacentEmptyArea(r, c - 1, height, width,copiedBoard)
        + getAdjacentEmptyArea(r + 1, c,height, width, copiedBoard)
        + getAdjacentEmptyArea(r - 1, c,height, width, copiedBoard);
    }
  }
  return 0;
}

/* function to generate all possible area combinations from remaining pieces */
void generatePossibleAreas(int* answerArray,
    long int maxCombinations,
    ListOfShapeMatrices* pieces,
    uint currentIndex) {
  int sizeArray = pieces->size() - currentIndex;
  int* generativeArray = new int[sizeArray]();
  for (long int sequencei = 0; sequencei < maxCombinations; sequencei++) {
    int answer=0;
    long int copyi = sequencei;
    for (int countj = sizeArray-1; countj >= 0; countj--) {
      generativeArray[countj] = copyi % 2 ;
      copyi /= 2;
    }
    for (int countk = 0; countk < sizeArray; countk++) {
      if (generativeArray[countk]) {
        answer += (*pieces)[countk + currentIndex]->getShapeArea(); // size instead}
      }
    }
    answerArray[sequencei] = answer;
    for (int countl = 0; countl < sizeArray; countl++) {
      generativeArray[countl] =0;
    }
  }
  delete[] generativeArray;
}

/* helper function to copy current state of board into 2D array */
int** copyBoard(PuzzleBoard* board) {
  int** copiedBoard = new int*[board->getHeight()];
  for (uint i = 0; i < board->getHeight(); i++){
      copiedBoard[i] = new int[board->getWidth()];
  }
  for (uint i = 0; i < board->getHeight(); i++){
    for (uint j = 0; j < board->getWidth(); j++ ){
      copiedBoard[i][j]= (board->getCurrentBoard())[i][j];
    }
  }
  return copiedBoard;
}

/* helper function to delete dynamically allocated 2D array */
void deleteCopy(PuzzleBoard* board, int** copyBoard) {
  for (uint i = 0; i < board->getHeight(); i++){
    delete[] copyBoard[i];
  }
  delete[] copyBoard;
}

/* function to check remaining area if the pieces can fit */
bool solvableConfig(PuzzleBoard* board,
    ListOfShapeMatrices* const pieces,
    uint currentIndex) {
  uint b_height = board->getHeight();
  uint b_width = board->getWidth();
  //preparation to generate an array all possible area combinations.
  int numRemainingPieces = pieces->size() - currentIndex;
  long int maxCombinations = pow(2, numRemainingPieces);
  int* answerArray = new int[maxCombinations]();
  generatePossibleAreas(answerArray, maxCombinations, pieces, currentIndex);

  int** copiedBoard = copyBoard(board);

  for (uint r = 0; r < b_height; r++) {
    for (uint c = 0; c < b_width; c++) {
      //GETTING ADJACENT AREA OF CURRENT SLOT
      int area = getAdjacentEmptyArea(r, c, b_height, b_width, copiedBoard);
      bool areaImpossible = true;

      //IF AREA IS ZERO, JUST BREAK OUT OF THIS FOR LOOP
      if (!area) {
        break;
      } else {
        for (long int sequencei = 0; sequencei < maxCombinations; sequencei++) {
          if (area == answerArray[sequencei]) {
            areaImpossible = false;
            break; //BREAK OUT OF SEARCH LOOP IF POSSIBLE COMBINATION OF SHAPES FOUND
          }
        }
      }
      if (areaImpossible) {
        deleteCopy(board, copiedBoard);
        delete[] answerArray;
        return false;
      }
    }
  }
  deleteCopy(board, copiedBoard);

  delete[] answerArray;
  return true;
}

/* function for recursive solving */
bool recursiveSolver (PuzzleBoard* board,
    ListOfShapeMatrices* const pieces,
    uint currentIndex,
    int& iterations) {

  iterations++;
  if (board->getRemainingArea() == 0 || currentIndex >= pieces->size()) {
    //the board is complete, and no more remaining pieces
    return true;
  }
  if (!solvableConfig(board, pieces, currentIndex)) {
    return false;
  }
  ShapeMatrix* temp = (*pieces)[currentIndex];
  ListOfShapeMatrices* shapesList = combinations(temp);
  int nextIndex = currentIndex + 1;

  for (uint r = 0; r < board->getHeight(); r++) {
    for (uint c = 0; c < board->getWidth(); c++) {
      for (uint counteri = 0; counteri < shapesList->size(); counteri++) {
        ShapeMatrix* r_temp = (*shapesList)[counteri];
        if (board->placePiece(c, r, nextIndex, r_temp)) {
          if (recursiveSolver(board, pieces, nextIndex,iterations)) {
            return true;
          }
          board->removePiece(c, r, nextIndex, r_temp); // revert
        }
      }
    }
  }
  //cleanup_list(shapesList);
  // delete shapesList;
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
  int iterations =0;
  bool success = recursiveSolver(board, &shapes, 0, iterations);

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
