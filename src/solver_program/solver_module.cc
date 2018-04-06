#include <fstream>
#include <iostream>
#include <vector>
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

int main(int argc, char** argv) { //  ./demo <input_filename>
  /* READ FILE */

  //argv[1] append to the pieces.txt
  const char* pieces_file = "output_data/pieces.txt";
  cout << " Reading file ....." <<endl;
  vector<ShapeMatrix> allPieces;
  shape_matrix_read(pieces_file, allPieces);
  cout << "File Read Complete!" << endl;

  /* SOLVER MODULE */
  int solve_success = UNSOLVED;
  int** solution = NULL;
  uint board_height = 0;
  uint board_width = 0;

  //createPartialBoard based on argv[2]

//  solution = puzzleSolver(pieces, solve_success, board_height, board_width);
  ifstream input_file;
  input_file.open(argv[2]);
  if (input_file.fail()){
    cout<<"Failed to open the partialBoard file!!!";
    return 1;
  }

  int count = 0;
  while (!input_file.eof()){
    int temp;
    input_file >> temp;
    cout << temp;
    count++;
  }
  count--;
  cout << "There are " << count << " elements in file";
  input_file.close();

  input_file.open(argv[2]);
  if (input_file.fail()){
    cout<<"Failed to open the partialBoard file!!!";
    return 1;
  }

  int partialBoard[count];
  int counter = 0;
  while (!input_file.eof()&&counter<count){
    input_file >> partialBoard[counter];
    cout << counter << ":" << partialBoard[counter] << endl;
    counter++;
  }
  input_file.close();

  solution = partialSolver(partialBoard, count, allPieces, solve_success, board_height, board_width);

  /* Return Message */
  switch (solve_success) {
    case SOLVED:
      cout << endl;
      cout << "Puzzle is solved !!" << endl;
      break;
    case UNDERSIZED:
      cout << "Puzzle pieces cannot fit the container: potentially more pieces than required!!" << endl;
      break;
    case OVERSIZED:
      cout << "Container not fully filled: potentially less pieces than required!!" << endl;
      break;
    case UNSOLVED:
      cout << "Puzzle cannot be solved !!" << endl;
      break;
    default:
      cout << "INTERNAL ERROR: SOLVER ERROR" << endl;
  }

  if (solve_success == SOLVED) {
    print_solution_board(solution, board_height, board_width);
    deleteCopy(board_height, solution);
  }
  return 0;
}
