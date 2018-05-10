#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <sstream>

#include "common/coordinates_io.h"
#include "common/debugger.h"
#include "common/memory.h"
#include "common/point.h"
#include "common/puzzle_board.h"
#include "common/shape_matrix_io.h"
#include "common/shape_matrix.h"
#include "common/types.h"
#include "common/utils.h"
#include "solver/solver.h"


#define EXPECTED_ARG_NUM {2, 3}
#define MAX_ARGS 3
#define ARG_FORMAT "bin/sp <puzzle_directory> <state> <debug>"

using namespace std;

/**
 * Partial Solver Module
 *  @steps:
 *   1) process arguments
 *   2) load puzzle pieces
 *   3) process board state
 *   4) find solution using partial solver
 *   5) process error code
 *  @shellCommandFormat: bin/sp <puzzle_directory> <state> <debug>
 *  @params:
 *   <puzzle_directory>: path of directory of puzzle
 *   <state>: a state representing a board as width, height, sq1, sq2 etc...
 *   <debug>: prints out debug information if true
 */
int main(int argc, char** argv) {

  //////////// PART 1: PROCESS ARGUMENTS /////////////////////////////

  if (!valid_args(argc, EXPECTED_ARG_NUM, ARG_FORMAT)){
    cout<<"argv 0 is: "<<argv[0]<<endl;
    cout<<"argv 1 is: "<<argv[1]<<endl;
    cout<<"argv 2 is: "<<argv[2]<<endl;
    return -1;
  }


  // extract command line argument
  const string puzzle_directory(argv[1]);
  const string state(argv[2]);

  // process debug argument. default is true
  bool debug = argc - 1 == MAX_ARGS? *argv[3] == '1': true ;


  //////////// PART 2: LOAD PUZZLE PIECES /////////////////////////////
  // read and load information of all puzzle pieces
  string pieces_filename = puzzle_directory + string("/pieces");

  if (debug) {
    cout << " Reading file:" << pieces_filename <<  endl;
  }

  vector<ShapeMatrix> pieces;
  shape_matrix_read(pieces_filename.c_str(), pieces, debug);

  if (debug) {
    cout << " File read complete!" <<endl;
  }

  // PART 2a: Sort the matrices





  //////////// PART 3: PROCESS BOARD STATE /////////////////////////////
  // extract the state (which was passed in by ARGV[2]) into an integer array
  stringstream stream(state);
  vector<int> state_vector;

  int temp_integer;
  while (stream >> temp_integer){
    state_vector.push_back(temp_integer);
  } // counts the size of the state (int array)

  /* allocation of space for int array based on number of integers within <state> */
  int partial_board[state_vector.size()];

  // copy state vector into the partial board array
  for (uint i = 0; i < state_vector.size(); i++){
    partial_board[i] = state_vector[i];
  }

  // check that there no conflict between dimensions of container, and the number of integers within <state>*/

  if (partial_board[0] * partial_board[1] != static_cast<int>(state_vector.size()) - 2){
    if (debug) {
      cout << "Error: state input's dimensions don't match container area" << endl;
      cout << "You have passed in - partial_board[0]: " << partial_board[0]
      << ", partial_board[1]: " << partial_board[1] << ". While the state_vector size is"
      << static_cast<int>(state_vector.size()) - 2<<endl;
      cout<<"argv 0 is: "<<argv[0]<<endl;
      cout<<"argv 1 is: "<<argv[1]<<endl;
      cout<<"argv 2 is: "<<argv[2]<<endl;
    }
    cout << INCORRECT_STATE << endl;
    return -1;
  }

  //////////// PART 4: CALL PARTIAL SOLVER /////////////////////////////
  // Initialize parameters for partial solver call
  int return_code;
  int** solution;
  uint board_height;
  uint board_width;

  // calls partial solver based on prepared inputs
  solution = partial_solver(puzzle_directory, partial_board, pieces, return_code, board_height, board_width, debug);

  //////////// PART 5: PROCESS RETURN CODE /////////////////////////////
  switch (return_code) {

    case SOLVED:
      if (debug) {
        cout << "Puzzle is solved !!" << endl;
        print_solution_board(solution, board_height, board_width);
      }

      cout << board_width << " " << board_height<< " ";

      for (uint row = 0; row < board_height; row++) {
        for (uint col = 0; col < board_width; col++) {
          cout << solution[row][col] << " ";
        }
      }
      delete_2d_array(solution, board_height);
      break;

    case UNDERSIZED:
      if (debug) {
        cout << "Puzzle pieces cannot fit the container: potentially more pieces than required!!" << endl;
      }
      cout << UNDERSIZED;
      return -1;

    case OVERSIZED:
      if (debug) {
        cout << "Container not fully filled: potentially less pieces than required!!" << endl;
      }
      cout << OVERSIZED;
      return -1;

    case UNSOLVED:
      if (debug) {
        cout << "Puzzle cannot be solved !!" << endl;
      }
      cout << UNSOLVABLE;
      return -1;

    default:
      if (debug) {
        cout << "INTERNAL ERROR: SOLVER ERROR" << endl;
      }
      cout << INTERNALERROR;
      return -1;
  }

  return 0;
}


/*********** Reading  and converting 1D array from text file (NOT IN USE FOR NOW) *******************
  ifstream input_file;
  input_file.open(argv[2]);
  if (input_file.fail()) {
    cout<<"Failed to open the partialBoard file!!!";
    return 1;
  }

  int count = 0;
  while (!input_file.eof()) {
    int temp;
    input_file >> temp;
    cout << temp;
    count++;
  }
  count--; //Fix this, related to EOF issue.
  cout << "There are " << count << " elements in file";
  input_file.close();

  input_file.open(argv[2]);
  if (input_file.fail()) {
    cout<<"Failed to open the partialBoard file!!!";
    return 1;
  }

  int partialBoard[count];
  int counter = 0;
  while (!input_file.eof() && counter < count) {
    input_file >> partialBoard[counter];
    cout << counter << ":" << partialBoard[counter] << endl;
    counter++;
  }
  input_file.close();
***************End of read/convert 1D array from text file *******************/
