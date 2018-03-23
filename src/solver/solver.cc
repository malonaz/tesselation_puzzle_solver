#include "common/memory.h"
#include "common/puzzle_board.h"
#include "common/shape_matrix.h"
#include "common/shape_matrix_io.h"
#include "common/types.h"
#include "solver.h"

#include <iostream>
#include <vector>
#include <cmath>

using std::min;
using std::max;
using std::round;
using std::vector;


PuzzleBoard* createBoard(const vector<ShapeMatrix> &matrices,
    vector<ShapeMatrix> &pieces, int& container_area, int& pieces_area) {

  // set output parameter to 0. used to keep track of the cumulative areas of all the pieces
  pieces_area = 0;
  
  // set the container area and index to zero. notice container area is an output parameter
  int container_index = -1;
  container_area = -1;

  // process all pieces
  for (uint i = 0; i < matrices.size(); i++) {

    // add the piece to the pieces vector
    pieces.push_back(matrices[i]);
    
    // get area of current matrix
    int current_area = matrices[i].getShapeArea();

    // add area to cumulative area
    pieces_area += current_area;
    
    if (current_area > container_area) {
      // update output parameters with current piece's dimensions 
      container_index = i;
      container_area  = current_area;
    } 
  }
  
  // remove the container area from the pieces' area and from the pieces vector
  pieces_area -= container_area;
  pieces.erase(pieces.begin() + container_index);

  // extract the container and use it to create a new puzzle board
  ShapeMatrix container = matrices[container_index];
  PuzzleBoard* board = new PuzzleBoard(container);

  return board;
}

bool isShapeMatrixInList(const ShapeMatrix &shape,
    const vector<ShapeMatrix*> &list) {
  bool result = false;
  for (uint j = 0; j < list.size(); j++) {
    if (shape == *(list[j])) {
      result = true;
      break;
    }
  }
  return result;
}

vector<ShapeMatrix*>* combinations(const ShapeMatrix &temp) {
  vector<ShapeMatrix*>* combi = new vector<ShapeMatrix*>();
  ShapeMatrix* r_temp = new ShapeMatrix(temp);

  for (uint i = 0; i < 8; i++) {
    if (!isShapeMatrixInList(*r_temp, *combi)) {
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
    const vector<ShapeMatrix> &pieces,
    uint currentIndex) {
  int sizeArray = pieces.size() - currentIndex;
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
        answer += pieces[countk + currentIndex].getShapeArea(); // size instead}
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
int** copyBoard(PuzzleBoard* const board) {
  uint height = board->getHeight();
  uint width = board->getWidth();
  int** copiedBoard = new int*[height];
  for (uint i = 0; i < height; i++){
      copiedBoard[i] = new int[width];
  }
  for (uint i = 0; i < height; i++){
    for (uint j = 0; j < width; j++ ){
      copiedBoard[i][j]= (board->getCurrentBoard())[i][j];
    }
  }
  return copiedBoard;
}

/* helper function to delete dynamically allocated 2D array */
void deleteCopy(uint height, int** copyBoard) {
  for (uint i = 0; i < height; i++){
    delete[] copyBoard[i];
  }
  delete[] copyBoard;
}

/* function to check remaining area if the pieces can fit */
bool solvableConfig(PuzzleBoard* board,
    const vector<ShapeMatrix> &pieces,
    uint currentIndex) {
  uint b_height = board->getHeight();
  uint b_width = board->getWidth();
  //preparation to generate an array all possible area combinations.
  int numRemainingPieces = pieces.size() - currentIndex;
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
        deleteCopy(b_height, copiedBoard);
        delete[] answerArray;
        return false;
      }
    }
  }
  deleteCopy(b_height, copiedBoard);

  delete[] answerArray;
  return true;
}

/* function for recursive solving */
bool recursiveSolver (PuzzleBoard* board,
    const vector<ShapeMatrix> pieces,
    uint currentIndex,
    int& iterations) {
  iterations++;
  if (board->getRemainingArea() == 0 || currentIndex >= pieces.size()) {
    //the board is complete, and no more remaining pieces
    return true;
  }
  if (!solvableConfig(board, pieces, currentIndex)) {
    return false;
  }
  uint height = board->getHeight();
  uint width = board->getWidth();
  ShapeMatrix temp = pieces[currentIndex];
  vector<ShapeMatrix*>* shapesList = combinations(temp);
  int nextIndex = currentIndex + 1;

  for (uint r = 0; r < height; r++) {
    for (uint c = 0; c < width; c++) {
      for (uint counteri = 0; counteri < shapesList->size(); counteri++) {
        ShapeMatrix* r_temp = (*shapesList)[counteri];
        if (board->placePiece(c, r, nextIndex, *r_temp)) {
          if (recursiveSolver(board, pieces, nextIndex,iterations)) {
            return true;
          }
          board->removePiece(c, r, nextIndex, *r_temp); // revert
        }
      }
    }
  }
  cleanup_list(shapesList);
  return false;
}


int** puzzleSolver(const vector<ShapeMatrix> &matrices, int& returnCode,
      uint& board_height, uint& board_width) {
  returnCode = 0;
  vector<ShapeMatrix> shapes;
  int** board_solution = NULL;
  int containerArea = 0;
  int totalPieceArea = 0;
  PuzzleBoard* board = createBoard(matrices, shapes,
      containerArea, totalPieceArea);
  if (totalPieceArea > containerArea) { // case of undersized container
    returnCode = UNDERSIZED;
    return board_solution;
  }
  if (totalPieceArea < containerArea) { // case of oversized container
    returnCode = OVERSIZED;
    return board_solution;
  }
  // if puzzle pieces area == container area
  int iterations = 0;
  bool success = recursiveSolver(board, shapes, 0, iterations);

  if (success) {
    returnCode = SOLVED;
    board_height = board->getHeight(); //returns height of board
    board_width = board->getWidth(); // returns width of board
    board_solution = copyBoard(board); // returns a 2D int array of board (w soln)
  } else {
    returnCode = UNSOLVED;
  }
  delete board;
  return board_solution;
}
