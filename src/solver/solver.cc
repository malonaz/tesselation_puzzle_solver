#include <iostream>
#include <fstream>
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

ListOfShapeMatrices* combinations(ShapeMatrix* temp, int& num_orientations) {
  ListOfShapeMatrices* combi = new ListOfShapeMatrices();
  ShapeMatrix* r_temp = temp;

  for (uint i = 0; i < 8; i++) {
    bool doAddTheShape = true;
    for (uint j = 0; j < combi->size(); j++) {
      if (*r_temp == *((*combi)[j])) {
        doAddTheShape = false;
        break;
      }
    } //rotate and check

    if (doAddTheShape) {
      combi->push_back(r_temp);
    }

    r_temp = r_temp->rotate();
    if (i == 3) {
      r_temp = r_temp->mirror();
    }
  }
  num_orientations = combi->size();
  return combi;
}



int getAdjacentEmptyArea(int r, int c, int height, int width, int** copiedBoard){
  if (r >= 0 && r < height && c >= 0 && c < width) {
    if (copiedBoard[r][c] == 0) {
      copiedBoard[r][c] = -1;
      return 1 + getAdjacentEmptyArea(r, c + 1, height, width, copiedBoard) + getAdjacentEmptyArea(r, c - 1, height, width,copiedBoard) +getAdjacentEmptyArea(r + 1, c,height, width, copiedBoard) + getAdjacentEmptyArea(r - 1, c,height, width, copiedBoard);
    }
  }
  return 0;
}

void generatePossibleAreas(int* answerArray, long int maxCombinations, ListOfShapeMatrices* pieces, uint currentIndex){
      int sizeArray = pieces->size()-currentIndex;
      int generativeArray [sizeArray]= {0};

      for (long int sequencei = 0; sequencei <maxCombinations; sequencei++) {
        int answer=0;

        long int copyi = sequencei;

        for (int countj = sizeArray-1; countj>= 0; countj--) {
          generativeArray[countj] = copyi%2;
          copyi/=2;
        }

        for (int countk = 0; countk<sizeArray; countk++) {
          if (generativeArray[countk]) {
            answer+= (*pieces)[countk+currentIndex]->getShapeArea(); // size instead}
          }
        }

        answerArray[sequencei]=answer;

        for (int countl = 0; countl<sizeArray; countl++) {
          generativeArray[countl] =0;
        }
   }

   // for (long int counti = 0; counti <maxCombinations; counti++) {
   //   cout <<answerArray[counti]<<" ";
   // } //to check whether code is sound
}

int** copyBoard(PuzzleBoard* board){
  int** copiedBoard = new int*[board->getHeight()];
  for (int i = 0; i < board->getHeight(); i++){
      copiedBoard[i] = new int[board->getWidth()];
  }
  for (int i = 0; i < board->getHeight(); i++){
    for (int j = 0; j < board->getWidth(); j++ ){
      copiedBoard[i][j]= (board->getCurrentBoard())[i][j];
      //cout<<copiedBoard[i][j]<<endl;
    }
  }
  return copiedBoard;
}

void deleteCopy(PuzzleBoard* board, int** copyBoard){
  for (int i = 0; i < board->getHeight(); i++){
    delete [] copyBoard[i];
  }
  delete [] copyBoard;
}

//WORK IN PROGRESS
bool solvableConfig(PuzzleBoard* board,
  ListOfShapeMatrices* const pieces,
  uint currentIndex){
  int b_height = board->getHeight();
  int b_width = board->getWidth();
  //preparation to generate an array all possible area combinations.
  int numRemainingPieces = pieces->size() - currentIndex;
  long int maxCombinations = pow(2, numRemainingPieces);
  int answerArray[maxCombinations]={0};
  generatePossibleAreas(answerArray, maxCombinations, pieces, currentIndex);

  for (int r = 0; r < b_height; r++) {
    for (int c = 0; c < b_width; c++) {

      int** copiedBoard = copyBoard(board);

      //GETTING ADJACENT AREA OF CURRENT SLOT
      int area = getAdjacentEmptyArea(r,c,b_height,b_width, copiedBoard);
      bool areaImpossible = true;

      //IF AREA IS ZERO, JUST BREAK OUT OF THIS FOR LOOP
      if (!area) {
        break;
      } else {
        for (long int sequencei = 0; sequencei < maxCombinations; sequencei++) {
          if (area==answerArray[sequencei]) {
            areaImpossible = false;
            break; //BREAK OUT OF SEARCH LOOP IF POSSIBLE COMBINATION OF SHAPES FOUND
          }
        }
      }
      if (areaImpossible){
        deleteCopy(board, copiedBoard);
        return false;
      }
      deleteCopy(board, copiedBoard);
    }
  }
  return true;
}


bool recursiveSolver (PuzzleBoard* board,
    ListOfShapeMatrices* const pieces,
    uint currentIndex,
    ostream& output,
    int& iterations) {

  iterations++;
  output<<"============Now in iteration no. "<< iterations <<"============= \n";
  if (board->getRemainingArea() == 0 || currentIndex >= pieces->size()) {
    //the board is complete, and no more remaining pieces
    return true;
  }

  if (!solvableConfig(board, pieces, currentIndex)) {
    output<<"Check 1"<<endl;
    output<<"Current configuration failed at unsolvable config."<<endl;
    board->printBoard(output);
    return false;
  }
  output<<"Check 2"<<endl;

  ShapeMatrix* temp = (*pieces)[currentIndex];
  int num_orientations = 0;
  ListOfShapeMatrices* shapesList = combinations(temp, num_orientations);
  int nextIndex = currentIndex + 1;

  for (uint r = 0; r < board->getHeight(); r++) {
    for (uint c = 0; c < board->getWidth(); c++) {
      for (uint counteri = 0; counteri < num_orientations; counteri++) {
          ShapeMatrix* r_temp = (*shapesList)[counteri];
          output<<r<<c<<". Index num:"<<nextIndex<<". orientation number:"<<counteri<<"/"<<num_orientations<<" Check 3"<<endl;
        if (board->placePiece(c, r, nextIndex, r_temp)) {
          output<<" ***********Placed piece success********** "<<endl;
          board->printBoard(output);
          output<<" ***************************************** "<<endl;
          if (recursiveSolver(board, pieces, nextIndex,output,iterations)) {
            return true;
          }
          // output<<"Current configuration failed at recursive area."<<endl;
          // board->printBoard(output);
          output<<"Returned false after recursive solve. Iteration: "<< iterations <<"\n";
          output<<"Check 4"<<endl;
          board->removePiece(c, r, nextIndex, r_temp); // revert
        }
      }
    }
  }
  output<<"exit?"<<endl;
  // cleanup_list(shapesList);
  // delete shapesList;
  output<<"Failed after all for loop. Iteration: "<< iterations <<"\n";

  board->printBoard(output);

  output<<"Check 5"<<endl;

  return false;
}


int puzzleSolver(ListOfShapeMatrices* const matrices) {
  shape_matrix_print((*matrices)[9]);
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
  ofstream output;
  output.open("out.txt");
  int iterations =0;
  bool success = recursiveSolver(board, &shapes, 0, output, iterations);
  output.close();

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
