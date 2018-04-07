#include <iostream>
#include <vector>
#include <cmath>
#include "common/memory.h"
#include "common/puzzle_board.h"
#include "common/shape_matrix.h"
#include "common/shape_matrix_io.h"
#include "common/types.h"
#include "solver.h"
#include "common/debugger.h"

using namespace std;

bool shapeAlreadyUsed(int currentIdentifier, int* partialBoard, int sizeOfPartialBoard){
  for (int i = 2; i < sizeOfPartialBoard; i++){
    if (currentIdentifier == partialBoard[i]) {
      return true;
    }
  }
  return false;
}

PuzzleBoard* createPartialBoard(int* partialBoard, int count, const vector<ShapeMatrix> &allPieces,
      vector<ShapeMatrix> &unusedPieces) {

    int allPiecesSize = (int)allPieces.size();

    //finding the original shape of container
    int maxAreaIdx;
    int containerIdentifier;

    uint containerHeight = partialBoard[0];
    uint containerWidth = partialBoard[1];
    int sizeOfPartialBoard = count;
    cout<<"the size of partial board is " << sizeOfPartialBoard<<endl;

    for (int i = 0; i < allPiecesSize; i++) {
      if ((containerHeight == allPieces[i].getHeight()) &&
      (containerWidth == allPieces[i].getWidth())) {
        //Compares the filled area with the matrix area. Assume perfect rectangular
        if (allPieces[i].getShapeArea() == allPieces[i].getMatrixArea()) {
          maxAreaIdx = i;
          containerIdentifier = allPieces[i].getIdentifier();
          break;
        }
      }
    }
    ShapeMatrix container = allPieces[maxAreaIdx];
    int actualSizeOfPartialBoard = container.getMatrixArea();

    //copying the partialBoard into taperedPartialBoard
    int taperedPartialBoard[actualSizeOfPartialBoard];
    int difference = sizeOfPartialBoard - actualSizeOfPartialBoard;

    for (int i = 0; i< actualSizeOfPartialBoard; i++){
      taperedPartialBoard[i]= partialBoard[difference + i];
    }

    //constructs the board, and then fill in based on the partialBoard array
    PuzzleBoard* board = new PuzzleBoard(taperedPartialBoard, container);

    for (int j = 0; j < allPiecesSize; j++) {
      int currentIdentifier = allPieces[j].getIdentifier();
      if (currentIdentifier!=containerIdentifier){
        if (!shapeAlreadyUsed(currentIdentifier, partialBoard, sizeOfPartialBoard)){
          unusedPieces.push_back(allPieces[j]);
          cout <<"pushing in piece number"<<allPieces[j].getIdentifier()<<endl;
        }
      }
    }
    return board;
}

//this is only called by when the solver is used for the first time.
PuzzleBoard* createBoard(const vector<ShapeMatrix> &matrices,
    vector<ShapeMatrix> &pieces, int& containerArea, int& totalPieceArea) {
  int maxArea = matrices[0].getShapeArea();
  int maxAreaIdx = 0;
  int accumArea = 0;
  int matricesSize = (int)matrices.size();

  //Saving the pieces to a folder called "pieces", the name of the folder needs to be updated
    shape_matrix_write("output_data/pieces.txt", matrices);

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
    //pushIdx will either be the current i, or the previous maxArea
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

void test_print(int** board_solution, char* file_name, uint height, uint width){
  ofstream out(file_name, ios_base::app);
  for (uint r = 0; r < height; r++) {
    for (uint c = 0; c < width; c++) {
      out << board_solution[r][c] << " ";
      // if (board_solution[r][c]<10){
      //   out<<" ";
      // }
    }
//    out << endl;
  }
  out << endl;
  out.close();
}

/* function for recursive solving */
bool recursiveSolver (PuzzleBoard* board,
    const vector<ShapeMatrix> pieces,
    uint currentIndex,
    int& iterations) {
  iterations++;
  uint height = board->getHeight();
  uint width = board->getWidth();
  // int** board_solution = copyBoard(board);
  // test_print(board_solution,"output_data/solution.txt", height, width);
  // deleteCopy(height, board_solution);

  if (board->getRemainingArea() == 0 || currentIndex >= pieces.size()) {
    //the board is complete, and no more remaining pieces

	  /******Uncomment/ comment out these lines below to include/ remove all solutions search: *****/
	  // int** board_solution = copyBoard(board);
	  // print_solution_board(board_solution, height, width); //TO PRINT ON TERMINAl
	  // deleteCopy(height, board_solution);

    /*************************************************************/
    // int** board_solution = copyBoard(board);
    // test_print(board_solution,"output_data/solution.txt", height, width);
    // deleteCopy(height, board_solution);

    return true;
  }
  if (!solvableConfig(board, pieces, currentIndex)) {
    return false;
  }

  ShapeMatrix temp = pieces[currentIndex];
  vector<ShapeMatrix*>* shapesList = combinations(temp);
  int nextIndex = currentIndex+1;

  for (uint r = 0; r < height; r++) {
    for (uint c = 0; c < width; c++) {
      for (uint counteri = 0; counteri < shapesList->size(); counteri++) {
        ShapeMatrix* r_temp = (*shapesList)[counteri];

        if (board->placePiece(c, r, nextIndex, *r_temp)) {
          if (recursiveSolver(board, pieces, nextIndex,iterations)) {
	    /******Comment/ Un-comment "return true;" to include/remove all solutions search: *****/
      //cout<< "Solution found after " << iterations << " iterations."<< endl;
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


bool searchExistingSolutions(PuzzleBoard* board) {
  uint height = board-> getHeight();
  uint width = board-> getWidth();
  int matrixSize = height*width;

  ifstream input("output_data/solution.txt");
  if (input.fail()){
    cout<< "Solution file does not exist yet!"<<endl;
    return false;
  }

  int previousSolution[matrixSize];

  int** currentBoard = copyBoard(board);

  bool success = false;

  int row = 0;

  while (!input.eof() && !success){
    bool correctRow = true;
    cout << "At existing sols row number: " << row <<endl;
    for (int i = 0; i < matrixSize; i++){
      int temp;
      input >> temp;
      uint rowNum = i / width;
      uint colNum = i % width;
      int difference = temp - currentBoard[rowNum][colNum];
      if (difference == 0 || difference == temp) {
        previousSolution[i] = temp;
      } else {
        correctRow = false;
      }
    }
    if (correctRow) {
      input.close();
      cout << "Existing solution found~~~~~~~~: " << endl;
      success = true;
      PuzzleBoard* tempBoard = new PuzzleBoard (previousSolution, board->getContainer()); // work on this.
      *board = *tempBoard; //copy assignment operator

      deleteCopy(height, currentBoard);
      delete tempBoard;
      return true;
    }
    row ++;
  }
  input.close();
  deleteCopy(height, currentBoard);
  cout << "Existing solution not found~~~~~~~~: " << endl;
  return false;
}

int** partialSolver(int* partialBoard, int count, const vector<ShapeMatrix> &allPieces, int& returnCode,
      uint& board_height, uint& board_width) {
  returnCode = 0;
  vector<ShapeMatrix> unusedPieces;
  int** board_solution = NULL;

  PuzzleBoard* board = createPartialBoard(partialBoard, count, allPieces, unusedPieces);

  for (int i = 0; i < unusedPieces.size(); i++) {
    cout << unusedPieces[i].getIdentifier() << endl;
    shape_matrix_print(unusedPieces[i]);
  }

  print_solution_board(board->getCurrentBoard(), board->getHeight(), board->getWidth());

  //Strategy 1: look in the existing repository
  bool success = false;
  success = searchExistingSolutions(board);

  //Strategy 2: look in the existing repository
  if (!success){
    int iterations = 0;
    success = recursiveSolver(board, unusedPieces, 0, iterations);
  }

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
