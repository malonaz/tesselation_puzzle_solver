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
#include <openssl/sha.h>

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
#include <glob.h>

using std::cout;
using std::endl;
using std::min;
using std::max;
using std::round;
using std::vector;
using std::unordered_set;

/**
 * Helper function which hashes a string. Adapted from an online reference.
 *  @params: 
 *   str: the string to hash
 *  @returns:
 *   the hash of the given string
 */
string sha256(string str){

  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, str.c_str(), str.size());
  SHA256_Final(hash, &sha256);
  stringstream strstream;
  
  for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    strstream << hex << setw(2) << setfill('0') << (int)hash[i];
  
  return strstream.str();
}

/**
 * Helper function which rotates a 2D-int array 90 degrees clockwise
 *  @params:
 *   board_solution: 2D-int array
 *   height: the height of the board_solution param
 *   width: the width of the board_solution param
 */
void rotate_board_solution(int** board_solution, int& height, int& width) {

  // compute the height and width of the rotated board
  int rotated_board_height = width;
  int rotated_board_width = height;

  // create a temporary 2D int array on the stack, containing the rotated values
  int rotated_board[rotated_board_width][rotated_board_height];

  for (int i = 0;  i < height; i++)
    for (int j = 0; j < width; j++)
      rotated_board[j][height - i - 1] = board_solution[i][j];
  
  // free board_solution
  for (int i = 0; i < height; i++)
    delete board_solution[i];
  delete board_solution;

  // reassign board_solution to a 2D array on the heap
  board_solution = new int*[rotated_board_height];
  
  for (int i = 0; i < rotated_board_height; i++) {

    // assign a row on the heap
    board_solution[i] = new int[rotated_board_width];

    // copy rotated board's value into board solution
    for (int j = 0; j < rotated_board_width; j++) 
      board_solution[i][j] = rotated_board[i][j];
  }

  // update the width and height
  width = rotated_board_width;
  height = rotated_board_height;

}


/**
 * Helper function which returns a string version of the given solution.
 *  @params:
 *   board_solution: a 2D-int array representing a puzzle solution
 *   height: the height of the board solution
 *   width: the width of the board solution
 */
string matrix_to_string (int** board_solution, uint height, uint width) {

  // str will be used to store the output
  string string_board_solution = "";

  // copy each cell into str
  for (uint row = 0; row < height; row++)
    for (uint col = 0; col < width; col++)
      string_board_solution += to_string(board_solution[row][col]);

  return string_board_solution;
}


/**
 * Helper function which returns true if the given identifier already exists in the partial board.
 *  @params:
 *   current_identifier: the shape identifier we want to check
 *   partial_board: integer array representing a partial board
 *   partial_board_size: size of the partial_board param
 */
bool shape_already_used(int current_identifier, int* partial_board, int partial_board_size){

  for (int i = 2; i < partial_board_size; i++)
    if (current_identifier == partial_board[i])
      return true;

  return false;
}

/**
 * Helper function which processes two main inputs :(i) all pieces, (ii) current state of board
 * and produce two main outputs: (i) all unused pieces, (ii) a partially filled board
 */
PuzzleBoard* create_partial_board(int* partial_board, int count, const vector<ShapeMatrix> &pieces,
      vector<ShapeMatrix> &unusedPieces, bool mute_debugging_messages) {

  // create copy of all_pieces and sort it in descending order
  vector<ShapeMatrix> pieces_copy = pieces;
  std::sort(pieces_copy.rbegin(), pieces_copy.rend());

  // find container (first piece within pieces_copy[]) and save its area
  ShapeMatrix container = pieces_copy[0];
  uint container_area = container.getShapeArea();

  /* obtain container identifier by iterating through pieces[] and finding the
  piece with corresponding container area. Then using getIdentifier method. Note:
  might not be safe to directly call pieces_copy[0].getIdentifier */
  int containerIdentifier;
  for (uint i = 0; i < pieces.size(); i++) {
      if (pieces[i].getShapeArea() == container_area) {
            containerIdentifier = pieces[i].getIdentifier();
        break;
      }
  }

  if (!mute_debugging_messages) {
    cout<<"container identifier is"<<containerIdentifier<<endl;
  }

  int partial_board_size = count;
  if (!mute_debugging_messages) {
    cout << "the size of partial board is " << partial_board_size << endl;
  }

  //copying the relevant integers (all excluding first few width, height) in partial_board[] into tapered_partial_board
  int tapered_partial_board[container_area];
  int difference = partial_board_size - container_area; // difference is usually 2, representing width and height

  for (uint i = 0; i < container_area; i++){
    tapered_partial_board[i] = partial_board[difference + i];
    if (!mute_debugging_messages) {
      cout << tapered_partial_board[i];
    }
  }

  //constructs the board, and then fill in based on the partial_board array
  PuzzleBoard* board = new PuzzleBoard(tapered_partial_board, container);

  for (uint j = 1; j < pieces_copy.size(); j++) {
    // get identifer of current piece
    int current_identifier = pieces[j].getIdentifier();

    /* if a piece (i) is NOT already placed on the partial board, and
    (ii) is NOT the container piece, then it will be pushed into the unusedPieces vector */
    if ((!shape_already_used(current_identifier, partial_board, partial_board_size) && (current_identifier != containerIdentifier))){
      unusedPieces.push_back(pieces[j]);
      if (!mute_debugging_messages) {
        cout <<"pushing in piece number "<<pieces[j].getIdentifier()<<endl;
      }
    }

  }
  return board;
}

/**
 * Creates a PuzzleBoard objects using the pieces contained in matrices.
 *   @params
 *    matrices: a list of ShapeMatrix representing shapes of a puzzle, including the puzzle container
 *    pieces: will contain all pieces in param matrices except the container piece
 *    container_area: will hold the area of the container of the puzzle board
 *    pieces_area: will contain the area of all pieces of the puzzle (excluding the container's)
 *   @returns: 
 *    A pointer to a PuzzleBoard object constructed as implied by the pieces. Object is constructed 
 *    on the heap. Caller is responsible for deletion of object off the heap.
 */
PuzzleBoard* create_board(const vector<ShapeMatrix> &matrices, vector<ShapeMatrix> &pieces,
			  int& container_area, int& pieces_area) {

  // set output parameter to 0. used to keep track of the cumulative areas of all the pieces
  pieces_area = 0;

  // set pieces equal to matrices, then sort it in descending order
  pieces = matrices;
  std::sort(pieces.rbegin(), pieces.rend());

  // extract container (first piece) and save its area
  ShapeMatrix container = pieces[0];
  pieces.erase(pieces.begin());
  container_area = container.getShapeArea();

  // find total area of pieces and set output parameter
  for (uint i = 0; i < pieces.size(); i++)
    pieces_area += pieces[i].getShapeArea();

  // use container to create a new puzzle board
  PuzzleBoard* board = new PuzzleBoard(container);

  return board;
}


/**
 * Helper function which returns true if shape is in the given list.
 *  @params:
 *   shape: shape we wish to search for in the list
 *   list: a list of shapes 
 */
bool is_shape_matrix_in_list(const ShapeMatrix &shape, const vector<ShapeMatrix> &list) {

  for (uint j = 0; j < list.size(); j++)
    if (shape == list[j])
      // the shape matrix is in the list
      return true;

  return false;
}



/**
 * Helper function which takes a shape and returns all possible variation of this shape,
 * rotations and flips (mirrors). Does not return any duplicates.
 *  @params: 
 *   shape: the shape we wish to rotate
 */
vector<ShapeMatrix> get_variations(const ShapeMatrix &shape) {

  // will hold all the possible rotations and mirrors of this shape
  vector<ShapeMatrix> variations = vector<ShapeMatrix>();

  // create a copy of the given shape
  ShapeMatrix current_variation = ShapeMatrix(shape);

  // vector keeps track of duplicates within variations
  vector<int> duplicates = vector<int>();

  for (uint i = 0; i < 8; i++){

    if (i == 4)
      // create a flipped version of the shape
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


/**
 * Helper function which writes solution in a 1-D int array into <file_name>
 */
void test_print(int** board_solution, char* file_name, uint height, uint width){
  ofstream out(file_name);
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
  if (pieces_area < container_area) {
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

/**
 * Glob is a file management helper function which returns the name of all
 * the files stored within a directory pattern
 */
inline vector<string> glob(const string& pat){
  glob_t glob_result;
  glob(pat.c_str(),GLOB_TILDE,NULL,&glob_result);
  vector<string> ret;
  for(unsigned int i=0;i<glob_result.gl_pathc;++i){
      ret.push_back(string(glob_result.gl_pathv[i]));
  }
  globfree(&glob_result);
  return ret;
}

/**
 * Helper function to search whether the existing state of the board has a
 * congruent solution that has already been solved before within the solutions directory
 */
bool search_existing_solutions(PuzzleBoard* board, char* directory_name, bool mute_debugging_messages) {
  uint height = board-> getHeight();
  uint width = board-> getWidth();
  int matrixSize = height * width;

  bool success = false;

  /*Based on directory_name, create a vector containing all the file names
  existing in that <directory>/solutions. This is importanct because each solution
  file is named based on a hash, and not a pre-set pattern */
  vector <string> all_solutions_file;
  string search_pattern = directory_name;
  search_pattern += "/solutions/*";
  all_solutions_file = glob (search_pattern);

  if (all_solutions_file.size() == 0 ) {
    return success;
  }

  int existingSolution[matrixSize];

  int** currentBoard = copy_board(board);

  /* Read through each existing solution file within folder:  */
  for (uint file_num = 0; file_num < all_solutions_file.size(); file_num++ ){
    if (success) {
      break;
    }
    if (!mute_debugging_messages){
      cout <<"checking file named: "<< all_solutions_file[file_num] <<endl;
    }

    // for each solution file, read content and check for consistency:
    ifstream input(all_solutions_file[file_num]);
    for (int i = 0; i < matrixSize; i++){
      int temp;
      input >> temp;
      uint rowNum = i / width;
      uint colNum = i % width;
      int difference = temp - currentBoard[rowNum][colNum];

      /* Consistency: The value in the specified position of the currentBoard should either
      be zero (unfilled), or the same as the corresponding value in solutions file */
      if (difference == 0 || difference == temp) {
        existingSolution[i] = temp;
        //if ALL the numbers in thec currentBoard are consistent with the solution file
        if (i == (matrixSize - 1)) {
            success = true;
            if (!mute_debugging_messages){
              cout<<"Found in this file!"<<endl;
            }
          PuzzleBoard* tempBoard = new PuzzleBoard (existingSolution, board->getContainer()); // work on this.
          *board = *tempBoard; //copy assignment operator
          delete tempBoard;
        }
      } else {
          if (!mute_debugging_messages){
            cout<<"For element number: " << i <<"current board's element is " <<  currentBoard[rowNum][colNum] << ", while files's element is " << temp <<endl;
            cout<<"Difference is "<< difference;
            cout<<". Solution is not in this file."<<endl;
          }
        break;
      }
    }
    input.close();
  }
  delete_2d_array(height, currentBoard);

  return success;

}

  // ifstream input("output_data/solution.txt");
  // if (input.fail()){
  //   cout<< "Solution file does not exist yet!"<<endl;
  //   return false;
  // }


  //
  //
  // while (!input.eof() && !success){
  //   bool correctRow = true;
  //   cout << "At existing sols row number: " << row <<endl;
  //   for (int i = 0; i < matrixSize; i++){
  //     int temp;
  //     input >> temp;
  //     uint rowNum = i / width;
  //     uint colNum = i % width;
  //     int difference = temp - currentBoard[rowNum][colNum];
  //     if (difference == 0 || difference == temp) {
  //       previousSolution[i] = temp;
  //     } else {
  //       correctRow = false;
  //     }
  //   }
  //   if (correctRow) {
  //     input.close();
  //     cout << "Existing solution found~~~~~~~~: " << endl;
  //     success = true;
  //     PuzzleBoard* tempBoard = new PuzzleBoard (previousSolution, board->getContainer()); // work on this.
  //     *board = *tempBoard; //copy assignment operator
  //
  //     delete_2d_array(height, currentBoard);
  //     delete tempBoard;
  //     return true;
  //   }
  //   row ++;
  // }
  // input.close();
  // delete_2d_array(height, currentBoard);
  // cout << "Existing solution not found~~~~~~~~: " << endl;


/**
 * Partial solver is called by bin/sp module, and finds solution given a partially solved state
 */

int** partial_solver(char* directory_name, int* partial_board, int count, const vector<ShapeMatrix> &pieces, int& returnCode,
      uint& board_height, uint& board_width, bool mute_debugging_messages) {
  returnCode = 0;
  vector<ShapeMatrix> unusedPieces;
  int** board_solution = NULL;

  /* calls create_partial_board to produce two major outputs: (i) unusedPieces
  (vector of all unused shapes), (ii) initialization of 'board' based on current
  state of the puzzle board  */
  PuzzleBoard* board = create_partial_board(partial_board, count, pieces, unusedPieces, mute_debugging_messages);

  /* prints debugging messages */
  if (!mute_debugging_messages) {
    for (uint i = 0; i < unusedPieces.size(); i++) {
      cout << "unused piece with identifier:" << unusedPieces[i].getIdentifier() << endl;
      shape_matrix_print(unusedPieces[i]);
    }

    cout<< "The current board is:"<<endl;
    print_solution_board(board->getCurrentBoard(), board->getHeight(), board->getWidth());
  }

  //Strategy 1: look in the existing repository
  bool success = false;
  success = search_existing_solutions(board, directory_name, mute_debugging_messages);

  bool writeNewSolnFlag = false;

  //Strategy 2: if there is no existing solution available, try to solve and produce a solution!
  if (!success){
    int iterations = 0;
    success = recursive_solver(board, unusedPieces, 0, iterations);
    writeNewSolnFlag = true;
  }

  // return code
  if (success) {
    returnCode = SOLVED;
    board_height = board->getHeight(); //returns height of board
    board_width = board->getWidth(); // returns width of board
    board_solution = copy_board(board); // returns a 2D int array of board (w soln)

    //HASH AND WRITE TO THE SOLUTIONS DIRECTORY
    if (writeNewSolnFlag){
      int n1 = strlen(directory_name);
      int n2 = strlen("/solutions/");
      int copy_height = board_height;
      int copy_width = board_width;
      for (int i = 0; i < 4; i++){
        rotate_board_solution(board_solution, copy_height, copy_width);
        string strHashOfSoln = sha256(matrix_to_string(board_solution,copy_height,copy_width));
        int n3 = strHashOfSoln.length();
        char hashFileName[(n1 + n2 + n3 + 1)];
        strcpy(hashFileName, directory_name);
        strncat(hashFileName, "/solutions/", n2);
        strncat(hashFileName, strHashOfSoln.c_str(), n3);

        test_print(board_solution, hashFileName, copy_height, copy_width);
      }
    }
  } else {
    returnCode = UNSOLVED;
  }
  delete board;
  return board_solution;
}
