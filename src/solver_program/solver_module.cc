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
  const char* pieces_file = "src/image_processor/pieces.txt";
  cout << " Reading file ....." <<endl;
  vector<ShapeMatrix> pieces;
  shape_matrix_read(pieces_file, pieces);
  cout << "File Read Complete!" << endl;

  /* SOLVER MODULE */
  int solve_success = UNSOLVED;
  int** solution = NULL;
  uint board_height = 0;
  uint board_width = 0;
  solution = puzzleSolver(pieces, solve_success, board_height, board_width);

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
