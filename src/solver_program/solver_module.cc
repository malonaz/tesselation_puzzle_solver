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
#include "solver/solver.h"

using namespace std;

/**
 * Partial Solver Module
 *  @shellCommandFormat: bin/sp <directory> <state> <debug>
 *  @params:
 *   <directory>
 *   <state>
 *   <debug>
 *
 * major steps:
 *  1) puzzle pieces, downloaded from <directory>:
 *   - when bin/ip (image processor is called for the first time, all the identified shape
 *     pieces would have been saved to a specific directory )
 *   - when bin/sp is called,the information on the pieces previously saved in the directory will be downloaded
 *  2) current state of the board: obtained from <state>
 *  3) puzzle pieces and current state of board are fed as inputs into partial solver.
 */
int main(int argc, char** argv) {

  //////////// PART 1: PROCESS ARGUMENTS /////////////////////////////
  if (argc < 3 || argc > 4){
    cout << "Error: provide either 3 or 4 inputs" <<endl;
    return -1;
  }

  // extract command line argument
  const string puzzle_directory(argv[1]);
  const string state(argv[2]);
  
  // process debug argument. default is to mute
  bool debug = argc == 3? true: *argv[3] != '1' ;
  
  
  //////////// PART 1: LOAD PUZZLE PIECES /////////////////////////////
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
  

  // set parameters to default values
  int return_code = UNSOLVED;
  int** solution = NULL;
  uint board_height = 0;
  uint board_width = 0;


  // extract the state into an integer array
  stringstream stream(state);
  vector<int> state_vector;
  
  do {
    int current_integer;
    stream >> current_integer;
    state_vector.push_back(current_integer);
  } while (stream);
  

  /* allocation of space for int array based on number of integers within <state> */
  int partial_board[state_vector.size()];

  // copy state vector into the partial board array
  for (uint i = 0; i < state_vector.size(); i++){
    partial_board[i] = state_vector[i];
  }

  // check that there no conflict between dimensions of container, and the number of integers within <state>*/
  if (partial_board[0] * partial_board[1] != static_cast<int>(state_vector.size() - 2)){
    if (debug) {
      cout << "Error: state input's dimensions don't match container area" << endl;
    }
    cout << INCORRECT_STATE << endl;
    return -1;
  }
  
  /* calls partial solver based on prepared inputs*/
  solution = partial_solver(puzzle_directory, partial_board, pieces, return_code, board_height, board_width, debug);

  /* Return Message */
  switch (return_code) {
    case SOLVED:
      if (debug) {
        cout << "Puzzle is solved !!" << endl;
        print_solution_board(solution, board_height, board_width);
      }

      cout << board_width << " " << board_height<< " ";

      for (uint r = 0; r < board_height; r++) {
        for (uint c = 0; c < board_width; c++) {
          cout << solution[r][c] << " ";
        }
      }
      delete_2d_array(solution, board_height);

      break;
    case UNDERSIZED:
      if (debug) {
        cout << "Puzzle pieces cannot fit the container: potentially more pieces than required!!" << endl;
      }
      cout << UNDERSIZED;
      break;
    case OVERSIZED:
      if (debug) {
        cout << "Container not fully filled: potentially less pieces than required!!" << endl;
      }
      cout << OVERSIZED;
      break;
    case UNSOLVED:
      if (debug) {
        cout << "Puzzle cannot be solved !!" << endl;
      }
      cout << UNSOLVABLE;
      break;
    default:
      if (debug) {
        cout << "INTERNAL ERROR: SOLVER ERROR" << endl;
      }
      cout << INTERNALERROR;
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
