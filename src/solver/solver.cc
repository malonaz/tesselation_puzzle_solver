#include "solver.h"
#include "common/memory.h"
#include "common/puzzle_board.h"
#include "common/shape_matrix.h"
#include "common/shape_matrix_io.h"
#include "common/types.h"
#include "common/debugger.h"
#include "dlx/problem.h"
#include "problem_row_metadata.h"
#include "dlx/search.h"

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
 * Helper function which hashes a string. Standard template adapted from an online reference.
 *  @params:
 *   str: the string to hash
 *  @returns:
 *   the hash of the given string
 */
string sha256(string str) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, str.c_str(), str.size());
  SHA256_Final(hash, &sha256);
  stringstream strstream;

  for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    strstream << hex << setw(2) << setfill('0') << (int)hash[i];
  }

  return strstream.str();
}

/**
 * Helper function which rotates a 2D-int array 90 degrees clockwise
 * this is used to rotate a found solution (such that 4 solutions will be available with a single solution)
 *  @params:
 *   board_solution: 2D-int array
 *   height: the height of the board_solution param
 *   width: the width of the board_solution param
 */
void rotate_board_solution(int** &board_solution, int& height, int& width, bool debug) {

  // compute the height and width of the rotated board
  int rotated_board_height = width;
  int rotated_board_width = height;

  // create a temporary 2D int array on the stack, containing the rotated values
  int rotated_board[rotated_board_width][rotated_board_height];

  for (int i = 0;  i < height; i++)
    for (int j = 0; j < width; j++)
      rotated_board[j][height - i - 1] = board_solution[i][j];

  // free board_solution
  delete_2d_array(board_solution, height);

  // reassign board_solution to a newly-created 2D array on the heap
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

  if (debug) {
    cout << "Inside rotate_board_solution function" << endl;
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        cout<< board_solution[i][j]<<" ";
      }
      cout<<endl;
    }
  }


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
bool shape_already_used(int current_identifier, int* partial_board, int partial_board_size) {

  for (int i = 2; i < partial_board_size; i++)
    if (current_identifier == partial_board[i])
      return true;

  return false;
}

/**
 * Helper function which recreates a board from an input array of integers
 *  @params:
 *   board_input: an array width, height, sq1, sq1, ... etc representing a board
 *   pieces: a list of puzzle pieces as logical matrices
 *   unused_pieces: output parameter which will contain all pieces that are not used in the puzzle board
 *                  implied by the board input parameter.
 *   debug: prints debugging information if true
 *  @returns:
 *   a pointer to a PuzzleBoard on the heap. Caller is responsible for deletion from the heap
 */
PuzzleBoard* create_partial_board(int* board_state, const vector<ShapeMatrix> &pieces,
          vector<ShapeMatrix> &unused_pieces, bool debug) {

  // create copy of all_pieces and sort it in descending order
  vector<ShapeMatrix> pieces_copy = pieces;
  std::sort(pieces_copy.rbegin(), pieces_copy.rend());

  // find container and extract its area and identifier
  ShapeMatrix container = pieces_copy[0];
  int container_identifier = container.getIdentifier();

  // compute size of board state
  int board_state_size = board_state[0] * board_state[1] + 2;

  if (debug) {
    cout << "container identifier is " << container_identifier << endl;
    cout << "size of the int array provided is " << board_state_size << endl;
  }

  // NOTE: the pointer arithmetic described by below three lines are temporarily removed
  // difference is usually 2, representing width and height
  // int board_state_extra_params = board_state_size - container_area;
  // constructs the board. notice we have incremented board_state to skip the first parameters

  if (debug) {
    int width = board_state[0], height = board_state[1];
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        cout << "$$$$" << i * width + j + 2 << ": " <<board_state[(i * width + j + 2)] << endl;
      }
    }
  }

  PuzzleBoard* board = new PuzzleBoard(board_state + 2, container);

  for (uint j = 1; j < pieces_copy.size(); j++) {
    // get identifer of current piece
    int current_identifier = pieces[j].getIdentifier();

    // add piece to current piece if it is not used
    if (!shape_already_used(current_identifier, board_state, board_state_size)) {
      unused_pieces.push_back(pieces[j]);

      if (debug) {
         cout << "pushing in piece number " << pieces[j].getIdentifier() << endl;
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

  for (uint i = 0; i < 8; i++) {
    if (i == 4) {
      // create a flipped version of the shape
      current_variation = current_variation.flip();
    }

    // add current variation if is is not already in the variations list
    if (!is_shape_matrix_in_list(current_variation, variations)) {
       variations.push_back(current_variation);
    }

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
int get_adjacent_empty_area(bool** visited, int row, int col, int** board, uint height, uint width) {

  // return 0 if board[row][col] is NOT (i) a valid square within boundaries of the board AND (ii) is empty
  if (row < 0 || col < 0) {
    return 0;
  }
  if (row >= (int)height || col >= (int)width || board[row][col] != 0 || visited[row][col]) {
    return 0;
  }

  // mark this empty square equal to -1
  visited[row][col] = true;

  // make recursive call to check the remaining adjacent area
  return 1 + get_adjacent_empty_area(visited, row, col + 1, board, height, width)
    + get_adjacent_empty_area(visited, row, col - 1, board, height, width)
    + get_adjacent_empty_area(visited, row + 1, col, board, height, width)
    + get_adjacent_empty_area(visited, row - 1, col, board, height, width);
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
void get_areas_permutations(unordered_set<int>& possible_areas, const vector<ShapeMatrix>& pieces, uint current_index, int sum = 0) {
  // check current_index is positive
  if (current_index >= pieces.size()) {
    return;
  }

  // get area of shape at current index
  const ShapeMatrix* shape = &pieces[current_index];
  int area = shape->getShapeArea();

  // add sum + area to possible areas
  possible_areas.insert(sum + shape->getShapeArea());

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
  for (uint i = 0; i < height; i++) {
      board_copy[i] = new int[width];
  }

  // copy each square's value into the 2d array
  int** existing_board = board->getCurrentBoard();
  for (uint row = 0; row < height; row++) {
    for (uint col = 0; col < width; col++) {
      board_copy[row][col]= existing_board[row][col];
    }
  }

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
  for (uint i = 0; i < height; i++) {
    delete[] array[i];
  }

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
bool solvable_config(PuzzleBoard* board, const vector<vector<ShapeMatrix>> &pieces, const vector<unordered_set<int>>& permutations, uint current_index) {

  // get board dimensions
  uint height = board->getHeight();
  uint width = board->getWidth();
  vector<int> areas;
  int** board_space = board->getCurrentBoard();

  int min_piece_area = board->getWidth() * board->getHeight(); // start with a maximum first
  for (uint i = current_index; i < pieces.size(); ++i) {
    min_piece_area = min(min_piece_area, (int)pieces[i][0].getShapeArea());
  }

  bool** visited = new bool*[height];
  for (uint i = 0; i < height; ++i) {
    visited[i] = new bool[width];
    for (uint j = 0; j < width; ++j) {
      visited[i][j] = false;
    }
  }

  // int min_board_space_area = board->getWidth() * board->getHeight(); // start with a maximum first
  for (uint row = 0; row < height; row++) {
    for (uint col = 0; col < width; col++) {
      // get area near current square
      int area = get_adjacent_empty_area(visited, row, col, board_space, height, width);

      // if area is zero, continue
      if (area == 0) {
         continue;
      }
      areas.push_back(area);
      // min_board_space_area = min(min_board_space_area, area);
    }
  }

  for (uint i = 0; i < height; ++i) {
    delete[] visited[i];
  }
  delete[] visited;

  // if (min_piece_area > min_board_space_area) {
  //   return false;
  // }

  // get all possible area variations.
  const unordered_set<int>* possible_areas = &permutations[current_index];

  for (uint i = 0; i < areas.size(); ++i) {
    // if area is not in the possible permutations of area, then config is unsolvable
    if (std::find(possible_areas->begin(), possible_areas->end(), areas[i]) == possible_areas->end()) {
      return false;
    }
  }

  return true;
}


/**
 * Helper function which writes solution in a 1-D int array into <file_name>
 *  @params:
 *   board_solution: 2D-int array representing a board
 *   filename: filename to which we want to write the board to
 *   height: height of the board
 *   width: width of the board
 */
void write_board_to_file(int** board_solution, string filename, uint height, uint width) {

  // open stream
  ofstream out(filename.c_str());

  // send board data to stream
  for (uint row = 0; row < height; row++) {
    for (uint col = 0; col < width; col++) {
      out << board_solution[row][col] << " ";
    }
  }

  // add line break and close stream
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
bool recursive_solver (PuzzleBoard* board, const vector<vector<ShapeMatrix>> &pieces, const vector<unordered_set<int>>& permutations, uint current_index, int& iterations) {
  // increment the iterations number
  iterations++;

  // check if board is complete
  if (board->getRemainingArea() == 0 || current_index >= pieces.size()) {
    return true;
  }

  // check if board is solvable
  if (!solvable_config(board, pieces, permutations, current_index)) {
    return false;
  }

  // get board dimensions
  uint height = board->getHeight();
  uint width = board->getWidth();

  int** currentBoard = board->getCurrentBoard();

  // get the current piece's variations (flips and rotations)
  vector<ShapeMatrix> current_piece_variations = pieces[current_index];
  for (uint row = 0; row < height; row++) {
    for (uint col = 0; col < width; col++) {
      if (currentBoard[row][col] != 0) {
        continue;
      }
      for (uint i = 0; i < current_piece_variations.size(); i++) {
         // get the ith variation of the current piece
        ShapeMatrix current_piece_variation = current_piece_variations[i];

         // if this variation cannot be placed, try the next
        if (!board->placePiece(col, row, current_piece_variation)) {
          continue;
        }

        // check that puzzle can be solved after placing the piece
        if (recursive_solver(board, pieces, permutations, current_index + 1, iterations)) {
          return true;
        }

        // backtrack
        board->removePiece(col, row, current_piece_variation);
      }
    }
  }

  return false;
}

/**
 * File management helper function which returns the name of all
 * the files stored within a directory. Inspired from an online
 * reference.
 *  @params:
 *   path: the path to the directory we wish to search
 *  @returns:
 *   the paths of the files inside the directory
 */
vector<string> get_filenames(const string& path) {

  glob_t glob_result;
  glob((path + "*").c_str(), GLOB_TILDE, NULL, &glob_result);

  // used to store the matched files paths
  vector<string> filenames;

  // get the files that match
  for(uint i = 0; i < glob_result.gl_pathc; i++) {
    filenames.push_back(string(glob_result.gl_pathv[i]));
  }

  // free the glob result object from the heap
  globfree(&glob_result);

  return filenames;
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
               int* board_solution) {

  // get current board as a 2D int array
  int** board = puzzle->getCurrentBoard();

  // get height and width of board
  uint height = puzzle->getHeight();
  uint width = puzzle->getWidth();

  // open current solution and copy it into existing_solution
  ifstream solution_fstream(solution_filename);

  // used for return value
  bool solution_is_consistent = true;

  for (uint i = 0; i < height * width; i++) {

    // copy next board integer into existing solution
    solution_fstream >> board_solution[i];

    // get corresponding integer from the puzzle's board
    int board_integer = board[i / width][i % width];

    // if the board_integer is not zero, it must match the potential solution's integer
    if (board_integer && board_integer != board_solution[i]) {
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
 *   puzzle_directory: hash of the boad
 *  @returns:
 *   true if there exists a solution to this board
 */
bool search_existing_solutions(PuzzleBoard* board, string puzzle_directory, bool debug) {

  // find all solution files
  string search_pattern = puzzle_directory + string("/solutions/");
  vector <string> solutions_filenames = get_filenames(search_pattern);

  // if no files match, return false as there are no solutions to this puzzle
  if (solutions_filenames.size() == 0) {
    if (debug) {
      cout << "there are no solution filenames matching this hash" << endl;
    }
    return false;
  }

  // will hold the existing solution if it exists
  int existing_solution[board->getHeight() * board->getWidth()];

  // read through each existing solution file within folder
  for (uint file_num = 0; file_num < solutions_filenames.size(); file_num++) {
    if (debug) {
      cout << "solution consistency check for file: " << solutions_filenames[file_num] << endl;
    }

    if (solution_is_consistent_with_board(board, solutions_filenames[file_num], existing_solution)) {

      if (debug) {
         cout << "solution found in " << puzzle_directory << endl;
      }

      // get container of board
      ShapeMatrix container = board->getContainer();

      // free incomplete board from the heap
      delete board;

      // point board to a new PuzzleBoard with the correct solution
      board = new PuzzleBoard(existing_solution, container);

      return true;
    }
  }

  if (debug)
    cout << "no consistent solution found" << endl;

  return false;
}

/**
 * Helper function is called to produce and write all four orientations of a found solution into cache
 *  @params:
 *   puzzle_directory: hash of the puzzle, used as directory
 *   board: puzzleboard containing the found solution
 *   board_height: height of the board
 *   board_width: width of the board
 */
void update_solutions_cache(PuzzleBoard* board, string puzzle_directory, bool debug) {
  //we are making a copy of board_solution here because we try not to rotate the original board_solution.
  int** board_solution_copy  = copy_board(board);
  int copy_height = board->getHeight();
  int copy_width = board->getWidth();

  for (int i = 0; i < 4; i++) {
    if (debug) {
      cout << "Beginning rotation at orientation: " << i << endl;
    }

    // hash the solution
    string solution_hash = sha256(matrix_to_string(board_solution_copy, copy_height, copy_width));

    if (debug) {
      for (int i = 0; i < copy_height; i++) {
        for (int j = 0; j < copy_width; j++) {
          cout << board_solution_copy[i][j] << " ";
        }
        cout << endl;
      }
    }

    if (debug) {
      cout << "part 2 " << i << endl;
    }

    // compute the filename
    string filename = puzzle_directory + string("/solutions/") + solution_hash;
    if (debug) {
      cout<<"part 3 "<< i << endl;
    }

    // write the solution to the filename
    write_board_to_file(board_solution_copy, filename, copy_height, copy_width);

    if (debug) {
      cout<<"part 4 "<< i << endl;
    }


    // rotate the board
    if (i != 3) {
      // rotate board only if it's not the last iteration
      rotate_board_solution(board_solution_copy, copy_height, copy_width, debug);
    }

    if (debug) {
      cout<<"Rotated orientation number: " << i << endl;
      cout << copy_height << endl;
      cout << copy_width << endl;
      cout << "first: " << board_solution_copy[0][0] << endl;
      for (int i = 0; i < copy_height; i++) {
        for (int j = 0; j < copy_width; j++) {
          cout << board_solution_copy[i][j] << " ";
        }
        cout << endl;
      }
    }
  }
  //free board_solution_copy from heap;
  delete_2d_array(board_solution_copy, copy_height);
  board_solution_copy = NULL;
}

bool can_place_on(const PuzzleBoard* board, const ShapeMatrix& piece, uint r, uint c) {
  if (piece.getWidth() + c > board->getWidth()) {
    return false;
  }

  if (piece.getHeight() + r > board->getHeight()) {
    return false;
  }

  int** current_board = board->getCurrentBoard();
  for (uint i = 0; i < piece.getHeight(); ++i) {
    for (uint j = 0; j < piece.getWidth(); ++j) {
      if (!piece.get(i, j)) {
        continue;
      }

      if (current_board[r + i][c + j] != 0) {
        return false;
      }
    }
  }
  return true;
}

Problem* build_problem(const PuzzleBoard* board, const vector<ShapeMatrix> &unused_pieces, vector<ProblemRowMetaData> &metadata) {
  // choice of unused pieces + (area of already placed or non-placable)
  uint size = board->getContainer().getMatrixArea();
  Problem* problem = new Problem((uint)unused_pieces.size() + size);

  uint board_height = board->getHeight();
  uint board_width = board->getWidth();

  int** current_board = board->getCurrentBoard();
  int** board_index = new int*[board_height];
  uint board_cell_index = 0;

  for (uint i = 0; i < board_height; ++i) {
    board_index[i] = new int[board_width];
    for (uint j = 0; j < board_width; ++j) {
      if (current_board[i][j] == 0) {
        board_index[i][j] = board_cell_index;
        ++board_cell_index;
      }
    }
  }

  for (uint i = 0; i < unused_pieces.size(); ++i) {
    vector<ShapeMatrix> variations = get_variations(unused_pieces[i]);

    for (ShapeMatrix shape: variations) {
      uint shape_width = shape.getWidth();
      for (uint r = 0; r < board_height; ++r) {
        for (uint c = 0; c < board_width; ++c) {
          if (!can_place_on(board, shape, r, c)) {
            continue;
          }
          vector<uint> row;
          /* each 'row' contains information corresponding to a possible placement
          by a specific piece, in a specific orientation(variation), onto a
          specific location on the puzzleboard, and all the corresponding board cells (x,y)
          which would be covered as a result of the placement */
          for (uint k = 0; k < shape.getMatrixArea(); ++k) {
            if (!shape.get(k)) {
              continue;
            }
            uint dr = k / shape_width;
            uint dc = k % shape_width;
            row.push_back(board_index[r + dr][c + dc]);
          }
          row.push_back(size + i);
          metadata.push_back({shape, r, c});
          problem->add_row(row);
          // shape identifier information will also be part of the row
        }
      }
    }
  }
  return problem;
}

/**
 * Partial solver is called by bin/sp module. Finds a solution given a partially solved state.
 *  @params:
 *   puzzle_directory: hash of the puzzle, used as directory
 *   board_state: an integer array representing a board state
 *   pieces: the pieces of the puzzle
 *   return_code:
 *     - SOLVED if the puzzle can be solved.
 *     - UNSOLVED if the puzzle cannot be solved
 *   board_height: output parameter height of the board
 *   board_width: output parameter width of the board
 *   debug: if true, prints debugging messages
 *  @returns
 *   a 2-d array represenging a solution of size board_height x board_width or NULL if no solution exists.
 */
int** partial_solver(string puzzle_directory, int* board_state, const vector<ShapeMatrix> &pieces, int& return_code,
         uint& board_height, uint& board_width, bool debug) {

  // used by create_partial_board() call. will hold the unused pieces
  vector<ShapeMatrix> unused_pieces;
  vector<ProblemRowMetaData> metadata;

  // create partial board, which will initialize unused pieces
  PuzzleBoard* board = create_partial_board(board_state, pieces, unused_pieces, debug);

  bool hasExistingSolutionFound = search_existing_solutions(board, puzzle_directory, debug);
  if (hasExistingSolutionFound) {
    int** current_board = board->getCurrentBoard();
    delete board;
    return current_board;
  }

  Problem* problem = build_problem(board, unused_pieces, metadata);
  Search _search(problem);
  vector<uint> stack;

  uint result = _search.search(stack);

  if (result != 1) {
    return_code = UNSOLVED;
    return NULL;
  }

  for (uint i: stack) {
    ProblemRowMetaData data = metadata[i];
    board->placePiece(data.col, data.row, data.shape);
  }

  update_solutions_cache(board, puzzle_directory, debug);
  int** board_solution_copy  = copy_board(board);
  board_height = board->getHeight();
  board_width = board->getWidth();

  // free board from the heap
  delete board;
  delete problem;

  return_code = SOLVED;
  return board_solution_copy; //success? board_solution:
}
