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

/*************** Overview of solver module ***************

shell command format : bin/sp <directory> <state> <mute_debugging_messages>

Major steps:
  1) puzzle pieces, downloaded from <directory>:
    - when bin/ip (image processor is called for the first time, all the identified shape
    pieces would have been saved to a specific directory )
    - when bin/sp is called,the information on the pieces previously saved in the directory will be downloaded

  2) current state of the board: obtained from <state>

  3) puzzle pieces and current state of board are fed as inputs into partial solver.

*************** End of overview ***************/

int main(int argc, char** argv) {
  /* READ FILE */
  if (argc < 3 || argc > 4){
    cout << "Error: provide either 3 or 4 inputs" <<endl;
    return 0;
  }

  /*This is a switch for whether debugging messages should be printed. Provided by argv[3] */
  bool mute_debugging_messages; //argv[3];

  if (argc ==3) {
    mute_debugging_messages = false; //if user does not indicate 4th input, default is to NOT mute (ie print messages)
  } else {
    int temp = *(argv[3]) - int('0');
    mute_debugging_messages = temp;
  }

  /* Read and load information of ALL puzzle pieces from argv[1] */
  int length = strlen(argv[1]);
  char pieces_file[(length + 1)];
  strcpy(pieces_file, argv[1]);
  strcat(pieces_file, "/pieces");

  if (!mute_debugging_messages) {
    cout << " Reading file ....." <<endl;
  }
  vector<ShapeMatrix> allPieces;
  shape_matrix_read(pieces_file, allPieces, mute_debugging_messages);

  if (!mute_debugging_messages) {
    cout << " File read complete!" <<endl;
  }
  /* End of reading and loading of information of ALL puzzle pieces from argv[1] */

  /* Initialization of parameters to default values*/
  int solve_success = UNSOLVED;
  int** solution = NULL;
  uint board_height = 0;
  uint board_width = 0;

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

  /* Use stringstream to convert argv[2] to int array partial board */

  /* count number of integers within <state> */
  stringstream stream(argv[2]);
  int count = 0;

  while(stream) {
      int n;
      stream >> n;
      count++;
  }

  count--;   // adjustment for double count of last input.

  /* allocation of space for int array based on number of integers within <state> */
  int partial_board[count];
  stringstream stream1(argv[2]);

  /* copying of <state> into partial_board (as int array) */
  for (int i = 0; i < count; i++){
    stream1 >> partial_board[i];
    if (!mute_debugging_messages) {
      cout << partial_board[i] << endl;
    }
  }

  /* checking there is no conflict between dimensions of container, and the number of integers within <state>*/
  if ((partial_board[0]*partial_board[1]) != (count - 2)){
    if (!mute_debugging_messages) {
      cout << "Error with current state provided!" << endl;
    }
    cout << INCORRECT_STATE << endl;
    return 0;
  }

  /* calls partial solver based on prepared inputs*/
  solution = partial_solver(argv[1], partial_board, count, allPieces, solve_success, board_height, board_width, mute_debugging_messages);

  /* Return Message */
  switch (solve_success) {
    case SOLVED:
      if (!mute_debugging_messages) {
        cout << "Puzzle is solved !!" << endl;
        print_solution_board(solution, board_height, board_width);
      }

      cout << board_width << " " << board_height<< " ";

      for (uint r = 0; r < board_height; r++) {
        for (uint c = 0; c < board_width; c++) {
          cout << solution[r][c] << " ";
        }
      }
      delete_2d_array(board_height, solution);

      break;
    case UNDERSIZED:
      if (!mute_debugging_messages) {
        cout << "Puzzle pieces cannot fit the container: potentially more pieces than required!!" << endl;
      }
      cout << UNDERSIZED;
      break;
    case OVERSIZED:
      if (!mute_debugging_messages) {
        cout << "Container not fully filled: potentially less pieces than required!!" << endl;
      }
      cout << OVERSIZED;
      break;
    case UNSOLVED:
      if (!mute_debugging_messages) {
        cout << "Puzzle cannot be solved !!" << endl;
      }
      cout << UNSOLVABLE;
      break;
    default:
      if (!mute_debugging_messages) {
        cout << "INTERNAL ERROR: SOLVER ERROR" << endl;
      }
      cout << INTERNALERROR;
  }

  return 0;
}
