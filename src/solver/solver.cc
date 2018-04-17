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
      vector<ShapeMatrix> &unusedPieces, bool debug) {

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

  if (debug) {
    cout<<"container identifier is"<<containerIdentifier<<endl;
  }

  int partial_board_size = count;
  if (debug) {
    cout << "the size of partial board is " << partial_board_size << endl;
  }

  //copying the relevant integers (all excluding first few width, height) in partial_board[] into tapered_partial_board
  int tapered_partial_board[container_area];
  int difference = partial_board_size - container_area; // difference is usually 2, representing width and height

  for (uint i = 0; i < container_area; i++){
    tapered_partial_board[i] = partial_board[difference + i];
    if (debug) {
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
      if (debug) {
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
 *  @params:
 *   row: row of the square
 *   col: column of the square
 *   board: 2D-int array representing the puzzle board
 *   height: height of the board
 *   width: width of the board
 */
int get_adjacent_empty_area(uint row, uint col, int** board, uint height, uint width){

  // return 0 if (row, col) is not a valid square on the board that is empty
  if (row >= height || col >= width || board[row][col] != 0)
    return 0;

  // set this square equal to -1
  board[row][col] = -1;

  // make recursive call
  return 1 + get_adjacent_empty_area(row, col + 1, board, height, width)
    + get_adjacent_empty_area(row, col - 1, board, height, width)
    + get_adjacent_empty_area(row + 1, col, board, height, width)
    + get_adjacent_empty_area(row - 1, col, board, height, width);
}


/**
 * Helper functions which generates all the permutations of the areas of the pieces starting at
 * the given current_index.
 *  @params:
 *   possible_areas: output parameter which will contain all the permutations of areas
 *   pieces: the pieces of the board
 *   current_index: any pieces at a lower index has already been placed and should no be considered
 *   sum: used for recursion. Do not use it. 
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
 *  @params:
 *   board: the board that we want to copy
 *  @returns:
 *   a 2D-int array copy of the given board, created on the heap. Caller is responsible 
 *   for deletion of board copy.
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
 *  @params:
 *   array: 2D array we wish to free
 *   height: height of the array
 */
void delete_2d_array(int** array, uint height) {

  // free each row
  for (uint i = 0; i < height; i++)
    delete[] array[i];

  // free array
  delete[] array;
}



/**
 * Helper function which checks empty areas to see if a combination of pieces can fit there.
 * used to prune search space
 *  @params:
 *   board: board state
 *   pieces: the pieces of the board
 *   current_index: Index of the next piece to be placed. Any piece with a lower index should be 
 *                 disregarded
 */
bool solvable_config(PuzzleBoard* board, const vector<ShapeMatrix> &pieces, uint current_index) {

  // get board dimensions
  uint height = board->getHeight();
  uint width = board->getWidth();

  // get all possible area variations.
  unordered_set<int> possible_areas = unordered_set<int>();
  get_areas_permutations(possible_areas, pieces, current_index);

  // get a copy of the board's 2D array
  int** board_copy = copy_board(board);

  for (uint row = 0; row < height; row++)
    for (uint col = 0; col < width; col++) {

      // get area near current square
      int area = get_adjacent_empty_area(row, col, board_copy, height, width);

      // if area is zero, continue
      if (area == 0)
	continue;

      // if area is not in the possible permutations of area, then config is unsolvable
      if (std::find(possible_areas.begin(), possible_areas.end(), area) == possible_areas.end()){
	delete_2d_array(board_copy, height);
	return false;
      }
    }

  // free board copy from the heap
  delete_2d_array(board_copy, height);
  
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
 * Helper function which recursively attempts to place a piece to solve the puzzle, in a depth-first search.
 *  @params: 
 *   board: board of the puzzle
 *   pieces: the pieces of the puzzle as a list of logical matrices
 *   current_index: index of the piece that we will place next
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
 * Function which attempts to solve the puzzle implied by the given shapes.
 *  @params:
 *   matrices: list of logical matrices representing a puzzle's pieces and container
 *   return_code:
 *     - SOLVED if the puzzle can be solved.
 *     - UNSOLVED if the puzzle cannot be solved
 *     - UNDERSIZED if the puzzle's container is too small for the pieces
 *     - OVERSIZED is the puzzle's container is too large for the pieces
 *   board_height: height of the board implied by the given matrices
 *   board_width: width of the board implied by the given matrices
 *  @returns: the solution of the board as a 2D array.
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
 * the files stored within a directory. Inspired from an online 
 * reference.
 *  @params: 
 *   path: the path to the directory we wish to search
 *  @returns: 
 *   the paths of the files inside the directory
 */
vector<string> glob(const string& path){
  
  glob_t glob_result;
  glob(path.c_str(), GLOB_TILDE, NULL, &glob_result);

  // used to store the matched files paths
  vector<string> file_paths;

  // get the files that match
  for(uint i = 0; i < glob_result.gl_pathc; i++)
    file_paths.push_back(string(glob_result.gl_pathv[i]));

  // free the glob result object from the heap
  globfree(&glob_result);
  
  return file_paths;
}



/**
 * Helper function which reads the solution at the given filename and compares it 
 * to the given current board.
 *  @params:
 *   puzzle: the current puzzle
 *   solution_filename: the filename of a solution we wish to compare this to
 *   solution: int array in which the solution will be copied if it is consistent with
 *             the board
 *  @returns:
 *   true if the solution at the given filename is consistent with the board
 */
bool solution_is_consistent_with_board(PuzzleBoard* puzzle, string solution_filename,
				       int* board_solution){

  // get current board as a 2D int array
  int** board = puzzle->getCurrentBoard();

  // get height and width of board
  uint height = puzzle->getHeight();
  uint width = puzzle->getWidth();
  
  // open current solution and copy it into existing_solution
  ifstream solution_fstream(solution_filename);

  // used for return value
  bool solution_is_consistent = true;
  
  for (uint i = 0; i < height * width; i++){
    
    // copy next integer into existing solution
    solution_fstream >> board_solution[i];
    
    // next integer must be unfilled (0) or equal to the current board state's same integer
    if (board_solution[i] != 0 && board_solution[i] != board[i/width][i%width]){
      solution_is_consistent = false;
      break;
    }
  }

  // close input stream
  solution_fstream.close();
  
  return solution_is_consistent;
}


/**
 * Helper function to search whether a given board state has a solution that has already
 * been solved within the solutions directory.
 *  @params:
 *   board: current board state of the puzzle
 *   puzzle_hash: hash of the boad
 *  @returns:
 *   true if there exists a solution to this board
 */
bool search_existing_solutions(PuzzleBoard* board, char* puzzle_hash, bool debug) {

  // find all solution files
  string search_pattern = puzzle_hash + string("/solution/*");
  vector <string> solutions_filenames = glob(search_pattern);

  // if no files match, return false as there are no solutions to this puzzle
  if (solutions_filenames.size() == 0)
    return false;
  
  // will hold the existing solution if it exists
  int existing_solution[board->getHeight() * board->getWidth()];
  
  // read through each existing solution file within folder
  for (uint file_num = 0; file_num < solutions_filenames.size(); file_num++){
    if (solution_is_consistent_with_board(board, solutions_filenames[file_num], existing_solution)){
      
      // get container of board
      ShapeMatrix container = board->getContainer();
      
      // free incomplete board from the heap
      delete board;
      
      // point board to a new PuzzleBoard with the correct solution
      board = new PuzzleBoard(existing_solution, container);

      return true;
    }
  }
  
  return false;
}

  
/**
 * Partial solver is called by bin/sp module, and finds solution given a partially solved state
 */

int** partial_solver(char* directory_name, int* partial_board, int count, const vector<ShapeMatrix> &pieces, int& returnCode,
      uint& board_height, uint& board_width, bool debug) {
  returnCode = 0;
  vector<ShapeMatrix> unusedPieces;
  int** board_solution = NULL;

  /* calls create_partial_board to produce two major outputs: (i) unusedPieces
  (vector of all unused shapes), (ii) initialization of 'board' based on current
  state of the puzzle board  */
  PuzzleBoard* board = create_partial_board(partial_board, count, pieces, unusedPieces, debug);

  /* prints debugging messages */
  if (debug) {
    for (uint i = 0; i < unusedPieces.size(); i++) {
      cout << "unused piece with identifier:" << unusedPieces[i].getIdentifier() << endl;
      shape_matrix_print(unusedPieces[i]);
    }

    cout<< "The current board is:"<<endl;
    print_solution_board(board->getCurrentBoard(), board->getHeight(), board->getWidth());
  }

  //Strategy 1: look in the existing repository
  bool success = false;
  success = search_existing_solutions(board, directory_name, debug);

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
