#include "common/memory.h"
#include "common/puzzle_board.h"
#include "common/shape_matrix.h"
#include "common/shape_matrix_io.h"
#include "common/types.h"
#include "solver.h"
#include "common/memory.h"
#include "common/puzzle_board.h"
#include "common/shape_matrix.h"
#include "common/shape_matrix_io.h"
#include "common/types.h"
#include "solver.h"
#include "common/debugger.h"

#include <iostream>
#include <unordered_set>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <string>
#include <stdio.h>
#include <sstream>
#include <iomanip>



using namespace std;

#include <openssl/sha.h>

/**** This standard sha256 initialization function was adapted from an online reference: *****/
string sha256(string str)
{
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, str.c_str(), str.size());
  SHA256_Final(hash, &sha256);
  stringstream strstream;
  for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
      strstream << hex << setw(2) << setfill('0') << (int)hash[i];
    }
  return strstream.str();
}
/**** End of referenced initialization function *****/

string intMatrixToString (int** solution, uint height, uint width) {
  string temp = "";

  for (uint r = 0; r < height; r++) {
    for (uint c = 0; c < width; c++) {
      temp+= to_string(solution[r][c]);
    }
  }
  return temp;
}


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

/**
 * Creates a PuzzleBoard objects using the pieces contained in matrices.
 *   @params: matrices: a list of ShapeMatrix representing shapes of a puzzle, including the puzzle container
 *   @params: pieces: will contain all pieces in param matrices except the container piece
 *   @params: container_area: will hold the area of the container of the puzzle board
 *   @params: pieces_area: will contain the area of all pieces of the puzzle (excluding the container's)
 */
PuzzleBoard* create_board(const vector<ShapeMatrix> &matrices, vector<ShapeMatrix> &pieces,
			  int& container_area, int& pieces_area) {

  // set output parameter to 0. used to keep track of the cumulative areas of all the pieces
  pieces_area = 0;

  // set pieces equal to matrices, then sort it in descending order
  pieces = matrices;
  std::sort(pieces.rbegin(), pieces.rend());

  // extract container (last piece) and save its area
  ShapeMatrix container = pieces[0];
  pieces.erase(pieces.begin());
  container_area = container.getShapeArea();
	       
  // find total area of pieces
  for (uint i = 0; i < pieces.size(); i++) 
    pieces_area += pieces[i].getShapeArea();
    
  // use container to create a new puzzle board
  PuzzleBoard* board = new PuzzleBoard(container);

  return board;
}


/**
 * Helper function which returns true if shape is in the given list.
 */
bool is_shape_matrix_in_list(const ShapeMatrix &shape, const vector<ShapeMatrix> &list) {

  for (uint j = 0; j < list.size(); j++) 
    if (shape == list[j])
      // the shape matrix is in the list
      return true;
  
  return false;
}



/**
 * Helper function which takes a piece and returns all possible variation of this piece,
 * rotations and flips (mirrors). Does not return any duplicates.
 */
vector<ShapeMatrix> get_variations(const ShapeMatrix &piece) {
  
  // will hold all the possible rotations and mirrors of this piece
  vector<ShapeMatrix> variations = vector<ShapeMatrix>();

  // create a copy of the given piece
  ShapeMatrix current_variation = ShapeMatrix(piece);

  // vector keeps track of duplicates within variations
  vector<int> duplicates = vector<int>();
  
  for (uint i = 0; i < 8; i++){

    if (i == 4)
      // create a flipped version of the piece
      current_variation = current_variation.flip();

    // add current variation if is is not already in the variations list
    if (!is_shape_matrix_in_list(current_variation, variations))
       variations.push_back(current_variation);
    
    // now rotate the shape
    current_variation = current_variation.rotate();
  }

  return variations;
}



/**
 * Helper function which returns the empty area near the square at (row, col) in the given board
 */
int get_adjacent_empty_area(uint row, uint col, uint height, uint width, int** board) {

  // return 0 if (row, col) is not a valid square on the board that is empty
  if (row >= height || col >= width || board[row][col] != 0)
    return 0;

  // set this square equal to -1
  board[row][col] = -1;
  
  // make recursive call
  return 1 + get_adjacent_empty_area(row, col + 1, height, width, board)
    + get_adjacent_empty_area(row, col - 1, height, width,board)
    + get_adjacent_empty_area(row + 1, col,height, width, board)
    + get_adjacent_empty_area(row - 1, col,height, width, board);
}


/**
 * Helper functions which copies into possible_areas all the possible permutations of the area of the 
 * pieces starting at current index
 */
void get_areas_permutations(unordered_set<int>& possible_areas, const vector<ShapeMatrix>& pieces, uint current_index, int sum = 0){

  // check current_index is positive
  if (current_index >= pieces.size())
    return;

  // get area of shape at current index
  int area = pieces[current_index].getShapeArea();

  // add sum + area to possible areas
  possible_areas.insert(sum + pieces[current_index].getShapeArea());

  // recursive calls. two choices: use this piece's area or not
  get_areas_permutations(possible_areas, pieces, current_index + 1, sum + area);
  get_areas_permutations(possible_areas, pieces, current_index + 1, sum);
}



/**
 * Helper function which returns a copy of the given board as a 2D integer array
 */
int** copy_board(PuzzleBoard* const board) {

  // get board dimensions
  uint height = board->getHeight();
  uint width = board->getWidth();

  // create a 2d array on the heap, with the board's dimensions
  int** board_copy = new int*[height];
  for (uint i = 0; i < height; i++)
      board_copy[i] = new int[width];
  
  // copy each square's value into the 2d array
  for (uint row = 0; row < height; row++)
    for (uint col = 0; col < width; col++ )
      board_copy[row][col]= (board->getCurrentBoard())[row][col];
     
  return board_copy;
}


/**
 * Helper function which frees a 2D array from the heap
 */
void delete_2d_array(uint height, int** copy_board) {

  // free each row
  for (uint i = 0; i < height; i++)
    delete[] copy_board[i];

  // free array
  delete[] copy_board;
}



/**
 * Helper function which checks empty areas to see if a combination of pieces can fit there.
 * used to prune search space
 */
bool solvable_config(PuzzleBoard* board, const vector<ShapeMatrix> &pieces, uint currentIndex) {

  // get board dimensions
  uint height = board->getHeight();
  uint width = board->getWidth();
  
  // get all possible area variations.
  unordered_set<int> possible_areas = unordered_set<int>();
  get_areas_permutations(possible_areas, pieces, currentIndex);

  // get a copy of the board's 2D array
  int** board_copy = copy_board(board);

  for (uint row = 0; row < height; row++) 
    for (uint col = 0; col < width; col++) {
      
      // get area near current square
      int area = get_adjacent_empty_area(row, col, height, width, board_copy);

      // if area is zero, continue
      if (area == 0)
	continue; 

      // if area is not in the possible permutations of area, then config is unsolvable
      if (std::find(possible_areas.begin(), possible_areas.end(), area) == possible_areas.end()){
	delete_2d_array(height, board_copy);
	return false;
      }
    }

  delete_2d_array(height, board_copy);
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


/**
 * Helper function which recursively attempts to place a piece to solve the puzzle. depth-first!
 */
bool recursive_solver (PuzzleBoard* board, const vector<ShapeMatrix> pieces, uint current_index, int& iterations) {

  // increment the iterations number
  iterations++;

  // check if board is complete
  if (board->getRemainingArea() == 0 || current_index >= pieces.size()) 
    return true;

  // check if board is solvable
  if (!solvable_config(board, pieces, current_index)) 
    return false;

  // get board dimensions
  uint height = board->getHeight();
  uint width = board->getWidth();

  // get the current piece's variations (flips and rotations)
  ShapeMatrix current_piece = pieces[current_index];
  vector<ShapeMatrix> current_piece_variations = get_variations(current_piece);

  for (uint row = 0; row < height; row++) 
    for (uint col = 0; col < width; col++) 
      for (uint i = 0; i < current_piece_variations.size(); i++) {

	// get the ith variation of the current piece
        ShapeMatrix current_piece_variation = current_piece_variations[i];

	// if this variation cannot be placed, try the next
        if (!board->placePiece(col, row, current_index + 1, current_piece_variation))
	  continue;

	// check that puzzle can be solved after placing the piece
	if (recursive_solver(board, pieces, current_index + 1,iterations))
	  return true;

	// backtrack
	board->removePiece(col, row, current_index + 1, current_piece_variation);
        }
      
  return false;
}


/* function for recursive solving */
bool recursiveSolver_old (PuzzleBoard* board,
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



/**
 * Function which attempts to solve the puzzle implied by the give matrices.
 *   @params:
 *     return_code:
 *       - SOLVED if the puzzle can be solved.
 *       - UNSOLVED if the puzzle cannot be solved
 *       - UNDERSIZED if the puzzle's container is too small for the pieces
 *       - OVERSIZED is the puzzle's container is too large for the pieces
 *     board_height: height of the board implied by the given matrices
 *     board_width: width of the board implied by the given matrices
 *   @returns: the solution of the board found as a 2D array.
 * Encodes the solution found into
 */ 
int** puzzle_solver(const vector<ShapeMatrix> &matrices, int& return_code, uint& board_height, uint& board_width) {
  
  vector<ShapeMatrix> shapes;

  // set variables used by recursive solver
  int container_area = 0;
  int pieces_area = 0;

  // create a board
  PuzzleBoard* board = create_board(matrices, shapes, container_area, pieces_area);
  
  // check for undersized container case
  if (pieces_area > container_area) { 
    return_code = UNDERSIZED;
    return NULL;
  }

  // check for oversized container case
  if (pieces_area < container_area) { // case of oversized container
    return_code = OVERSIZED;
    return NULL;
  }

  // attempt to solve the puzzle recusively
  int iterations = 0;
  bool success = recursive_solver(board, shapes, 0, iterations);

  // set output parameters
  return_code = success? SOLVED: UNSOLVED;
  board_height = board->getHeight(); 
  board_width = board->getWidth();

  // set return param
  int** board_solution = success? copy_board(board): NULL;

  // free board
  delete board;

  // return solution
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

int** partialSolver(char* directoryName, int* partialBoard, int count, const vector<ShapeMatrix> &allPieces, int& returnCode,
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
//  success = searchExistingSolutions(board);
  bool writeNewSolnFlag = false;

  //Strategy 2: look in the existing repository
  if (!success){
    int iterations = 0;
    success = recursiveSolver(board, unusedPieces, 0, iterations);
    writeNewSolnFlag = true;
  }

  if (success) {
    returnCode = SOLVED;
    board_height = board->getHeight(); //returns height of board
    board_width = board->getWidth(); // returns width of board
    board_solution = copyBoard(board); // returns a 2D int array of board (w soln)

    //HASH AND WRITE TO THE SOLUTIONS DIRECTORY
    if (writeNewSolnFlag){
      string strHashOfSoln = sha256(intMatrixToString(board_solution,board_height,board_width));
      int n1 = strlen(directoryName);
      int n2 = strlen("/Solutions/");
      int n3 = strHashOfSoln.length();
      char hashFileName[(n1 + n2 + n3 + 1)];
      strcpy(hashFileName, directoryName);
      strncat(hashFileName, "/Solutions/", n2);
      strncat(hashFileName, strHashOfSoln.c_str(), n3);

      test_print(board_solution, hashFileName, board_height, board_width);

    }
  } else {
    returnCode = UNSOLVED;
  }
  delete board;
  return board_solution;
}
