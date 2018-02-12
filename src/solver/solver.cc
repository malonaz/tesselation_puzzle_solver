#include <iostream>
#include <vector>
#include <cmath>
#include "common/debugger.h"
#include "common/memory.h"
#include "common/puzzle_board.h"
#include "common/shape_matrix.h"
#include "common/shape_matrix_io.h"
#include "common/types.h"
#include "solver.h"
#include <time.h>

using namespace std;

PuzzleBoard* createBoard(const vector<ShapeMatrix> &matrices,
    vector<ShapeMatrix> &pieces, int& containerArea, int& totalPieceArea) {
  int maxArea = matrices[0].getShapeArea();
  int maxAreaIdx = 0;
  int accumArea = 0;
  int matricesSize = (int)matrices.size();

  for (int i = 1; i < matricesSize; i++) {
    int tempArea = matrices[i].getShapeArea();
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
    pieces.push_back(matrices[pushIdx]);
  }
  ShapeMatrix container = matrices[maxAreaIdx];
  PuzzleBoard* board = new PuzzleBoard(container);
  containerArea = maxArea; // return container area as a reference
  totalPieceArea = accumArea; // return total area of puzzle pieces
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

/*Function creates a list containing the unique orientations of each shape temp*/
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

/* function to get an adjacent block of empty cells */
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

/* pruning function: scans through board for every adjacent block of empty
cells and checks if a combination of the remaining pieces can be fit into each empty block */
bool solvableConfig(PuzzleBoard* board,
    const vector<ShapeMatrix> &pieces,
    uint currentIndex) {
  uint b_height = board->getHeight();
  uint b_width = board->getWidth();
  //preparation to generate an array all possible area combinations.
  int numRemainingPieces = pieces.size() - currentIndex;
  long int maxCombinations = pow(2, numRemainingPieces);
  int* answerArray = new int[maxCombinations]();
  /*helper function which returns an int array of all possible areas that
  can be formed with the remaining pieces*/
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
    int& iterations,
    int& solutionNum,
    time_t start) {
  iterations++;

  /*base case: if the board is already fully filled with there being no remaining pieces left*/
  if (board->getRemainingArea() == 0 || currentIndex >= pieces.size()) {
    //the board is complete, and no more remaining pieces
    return true;
  }
  /*pruning function: checks if there are "unsolvable empty blocks" on the board
  for example, if there is an empty block of 3-unit squares, but there is no way
  we can form an area of 3 using any combination of the remaining pieces.  */
  if (!solvableConfig(board, pieces, currentIndex)) {
    return false;
  }
  uint height = board->getHeight();
  uint width = board->getWidth();

  //Extract the current piece that we will try to place, and find the different
  //orientations for that shape
  ShapeMatrix temp = pieces[currentIndex];
  vector<ShapeMatrix*>* shapesList = combinations(temp);
  int nextIndex = currentIndex + 1;

  /*For the current piece that we are trying to place into the board,
  1) first try to find a suitable location in the board that we might be able to
  place it, 2) try different orientations
  */
  for (uint r = 0; r < height; r++) {
    for (uint c = 0; c < width; c++) {
      //try differnet orientations of the same shape
      for (uint counteri = 0; counteri < shapesList->size(); counteri++) {
        ShapeMatrix* r_temp = (*shapesList)[counteri];
        if (board->placePiece(c, r, nextIndex, *r_temp)) {
          if (recursiveSolver(board, pieces, nextIndex,iterations, solutionNum, start)) {
            //TO CHANGE: SAVE THIS SOLUTION AND FIND NEXT, INSTEAD OF RETURN
              solutionNum++;
              time_t elapsed = time(0) - start;
              std::cout<<"Solution number "<<solutionNum;
              std::cout<<". After "<< iterations<<" number of iterations. ";
              std::cout<<"Elapsed time: " << elapsed<<" seconds."<<std::endl;
              int** board_solution = copyBoard(board);
              print_solution_board(board_solution, height, width);
            //return true;
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
  int iterations =0;
  int solutionNum = 0;
  time_t start = time(0);
  bool success = recursiveSolver(board, shapes, 0, iterations, solutionNum, start);

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
