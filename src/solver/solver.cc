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


/**
 * Creates a PuzzleBoard objects using the pieces contained in matrices.
 *   @params: matrices: a list of ShapeMatrix representing shapes of a puzzle, including the puzzle container
 *   @params: pieces: will contain all pieces in param matrices except the container piece
 *   @params: container_area: will hold the area of the container of the puzzle board
 *   @params: pieces_area: will contain the area of all pieces of the puzzle (excluding the container's)
 */
PuzzleBoard* create_board(const vector<ShapeMatrix> &matrices,
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

/**
 * Helper function which returns true if shape is in the given list.
 */
bool is_shape_matrix_in_list(const ShapeMatrix &shape, const vector<ShapeMatrix*> &list) {

  for (uint j = 0; j < list.size(); j++) 
    if (shape == *(list[j]))
      // the shape matrix is in the list
      return true;
  
  return false;
}

/**
 * Helper function which takes a piece and returns all possible variation of this piece,
 * rotations and flips (mirrors). Does not return any duplicates.
 */
vector<ShapeMatrix*>* variations(const ShapeMatrix &piece) {
  
  // will hold all the possible rotations and mirrors of this piece
  vector<ShapeMatrix*>* variations = new vector<ShapeMatrix*>();

  // create a copy of the given piece
  ShapeMatrix* current_variation = new ShapeMatrix(piece);

  // vector keeps track of duplicates within variations
  vector<int> duplicates = vector<int>();
  
  for (uint i = 0; i < 8; i++){

    if (i == 4)
      // create a flipped version of the piece
      current_variation = current_variation->mirror();

    // check if current variation is already in the variations list
    if (is_shape_matrix_in_list(*current_variation, *variations))
      duplicates.push_back(i);
    
    // add the current variation
    variations->push_back(current_variation);

    // now rotate the shape
    current_variation = current_variation->rotate();
  }

  
  // free duplicates from the heap and remove them from the variations list
  for (int i = duplicates.size() -1 ; i != -1; i--){
    delete (*variations)[duplicates[i]];
    variations->erase(variations->begin() + duplicates[i]);
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


/* function to generate all possible area variations from remaining pieces */
void generate_possible_areas(int* answer_array, long int max_variations,
			     const vector<ShapeMatrix> &pieces, uint current_index) {

  
  int size_array = pieces.size() - current_index;
  int* generative_array = new int[size_array]();

  for (long int sequencei = 0; sequencei < max_variations; sequencei++) {
    int answer=0;
    long int copyi = sequencei;
    for (int countj = size_array-1; countj >= 0; countj--) {
      generative_array[countj] = copyi % 2 ;
      copyi /= 2;
    }
    for (int countk = 0; countk < size_array; countk++) {
      if (generative_array[countk]) {
        answer += pieces[countk + current_index].getShapeArea(); // size instead}
      }
    }
    answer_array[sequencei] = answer;
    for (int countl = 0; countl < size_array; countl++) {
      generative_array[countl] =0;
    }
  }
  delete[] generative_array;
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

/* helper function to delete dynamically allocated 2D array */
void delete_copy(uint height, int** copy_board) {
  for (uint i = 0; i < height; i++){
    delete[] copy_board[i];
  }
  delete[] copy_board;
}

/* function to check remaining area if the pieces can fit */
bool solvable_config(PuzzleBoard* board,
    const vector<ShapeMatrix> &pieces,
    uint currentIndex) {
  uint b_height = board->getHeight();
  uint b_width = board->getWidth();
  //preparation to generate an array all possible area variations.
  int numRemainingPieces = pieces.size() - currentIndex;
  long int maxVariations = pow(2, numRemainingPieces);
  int* answerArray = new int[maxVariations]();
  generate_possible_areas(answerArray, maxVariations, pieces, currentIndex);

  int** copiedBoard = copy_board(board);

  for (uint r = 0; r < b_height; r++) {
    for (uint c = 0; c < b_width; c++) {
      //GETTING ADJACENT AREA OF CURRENT SLOT
      int area = get_adjacent_empty_area(r, c, b_height, b_width, copiedBoard);
      bool areaImpossible = true;

      //IF AREA IS ZERO, JUST BREAK OUT OF THIS FOR LOOP
      if (!area) {
        break;
      } else {
        for (long int sequencei = 0; sequencei < maxVariations; sequencei++) {
          if (area == answerArray[sequencei]) {
            areaImpossible = false;
            break; //BREAK OUT OF SEARCH LOOP IF POSSIBLE COMBINATION OF SHAPES FOUND
          }
        }
      }
      if (areaImpossible) {
        delete_copy(b_height, copiedBoard);
        delete[] answerArray;
        return false;
      }
    }
  }
  delete_copy(b_height, copiedBoard);

  delete[] answerArray;
  return true;
}

/* function for recursive solving */
bool recursive_solver (PuzzleBoard* board,
    const vector<ShapeMatrix> pieces,
    uint currentIndex,
    int& iterations) {
  iterations++;
  if (board->getRemainingArea() == 0 || currentIndex >= pieces.size()) {
    //the board is complete, and no more remaining pieces
    return true;
  }
  if (!solvable_config(board, pieces, currentIndex)) {
    return false;
  }
  uint height = board->getHeight();
  uint width = board->getWidth();
  ShapeMatrix temp = pieces[currentIndex];
  vector<ShapeMatrix*>* shapesList = variations(temp);
  int nextIndex = currentIndex + 1;

  for (uint r = 0; r < height; r++) {
    for (uint c = 0; c < width; c++) {
      for (uint counteri = 0; counteri < shapesList->size(); counteri++) {
        ShapeMatrix* r_temp = (*shapesList)[counteri];
        if (board->placePiece(c, r, nextIndex, *r_temp)) {
          if (recursive_solver(board, pieces, nextIndex,iterations)) {
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


int** puzzle_solver(const vector<ShapeMatrix> &matrices, int& returnCode,
      uint& board_height, uint& board_width) {
  returnCode = 0;
  vector<ShapeMatrix> shapes;
  int** board_solution = NULL;
  int containerArea = 0;
  int totalPieceArea = 0;
  PuzzleBoard* board = create_board(matrices, shapes,
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
  bool success = recursive_solver(board, shapes, 0, iterations);

  if (success) {
    returnCode = SOLVED;
    board_height = board->getHeight(); //returns height of board
    board_width = board->getWidth(); // returns width of board
    board_solution = copy_board(board); // returns a 2D int array of board (w soln)
  } else {
    returnCode = UNSOLVED;
  }
  delete board;
  return board_solution;
}
