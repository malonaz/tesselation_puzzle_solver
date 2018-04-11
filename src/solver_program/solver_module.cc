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

int main(int argc, char** argv) { //  bin/sp directory state mute_debugging_messages
  /* READ FILE */
  if (argc != 4){
    cout << "Error: input arguments not 4" <<endl;
    return 0;
  }
  int temp = *(argv[3]) - int('0');
  bool mute_debugging_messages = temp; //argv[3];

  //Read and load information of the puzzle pieces
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

  /* SOLVER MODULE */
  int solve_success = UNSOLVED;
  int** solution = NULL;
  uint board_height = 0;
  uint board_width = 0;

/***********Reading  and converting 1D array from text file *******************
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

  stringstream stream(argv[2]);

  int count = 0;
  while(stream){
      int n;
      stream>>n;
  count++;
  }

  count--;
  int partial_board[count];

  stringstream stream1(argv[2]);

  for (int i = 0; i < count; i++){
      stream1 >> partial_board[i];
      if (!mute_debugging_messages) {
        cout << partial_board[i] << endl;
      }
  }

  solution = partial_solver(argv[1], partial_board, count, allPieces, solve_success, board_height, board_width, mute_debugging_messages);

  /* Return Message */
  switch (solve_success) {
    case SOLVED:
      // cout << endl;
      // cout << "Puzzle is solved !!" << endl;

      if (!mute_debugging_messages) {
        print_solution_board(solution, board_height, board_width);
      }

      for (uint r = 0; r < board_height; r++) {
        for (uint c = 0; c < board_width; c++) {
          cout << solution[r][c] << " ";
        }
      }
      //cout << endl;
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
  //
  // if (solve_success == SOLVED) {
  //   if (!mute_debugging_messages) {
  //     print_solution_board(solution, board_height, board_width);
  //   }
  //
  //   for (uint r = 0; r < board_height; r++) {
  //     for (uint c = 0; c < board_width; c++) {
  //       cout << solution[r][c] << " ";
  //     }
  //   }
  //
  //   delete_2d_array(board_height, solution);
  // }
  return 0;
}
