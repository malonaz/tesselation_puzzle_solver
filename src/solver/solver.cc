#include "common/memory.h"
#include "common/puzzle_board.h"
#include "common/shape_matrix.h"
#include "common/shape_matrix_io.h"
#include "common/types.h"
#include "solver.h"

#include <iostream>
#include <unordered_set>
#include <vector>
#include <cmath>
#include <algorithm>

using std::min;
using std::max;
using std::round;
using std::vector;
using std::unordered_set;

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
