#include "common/coordinates_io.h"
#include "common/debugger.h"
#include "common/memory.h"
#include "common/point.h"
#include "common/puzzle_board.h"
#include "common/shape_matrix_io.h"
#include "common/shape_matrix.h"
#include "common/types.h"
#include "discretizer/shape_rotate.h"
#include "discretizer/shape_translate.h"
#include "imagereader/image_read.h"
#include "solver/solver.h"

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char** argv) { //  ./demo <input_filename>
  /* READ FILE */
  vector<ListOfPoints> puzzle_pieces;

  const char* image_file = argv[1];

  find_coordinates(image_file, puzzle_pieces);
  debug_coordinates(image_file, puzzle_pieces);

  cout<< "Rotating Pieces..." << endl;
  /* SHAPE TRANSLATE MODULE */
  vector<ListOfPoints> rotated_puzzle_pieces;
  rotate_shapes(puzzle_pieces, rotated_puzzle_pieces);

  print_list_of_shapes(rotated_puzzle_pieces);

  cout << puzzle_pieces.size() << " Pieces rotated!" << endl;


  cout << "Translating shape to Boolean Matrix...." <<endl;
  /* DISCRETIZER MODULE */
  vector<ShapeMatrix> pieces;
  bool translate_success = shape_translate_all_shapes(rotated_puzzle_pieces, pieces);

  if (translate_success == false){
    cout << "INTERNAL ERROR: SHAPE TRANSLATE FAIL" <<endl;
    return 1;
  }

  print_area(pieces);

  cout << "Translation complete!" << endl;

  cout << "Solving Puzzle...." <<endl;

  /* SOLVER MODULE */
  int solve_success = UNSOLVED;
  int** solution = NULL;
  uint board_height = 0;
  uint board_width = 0;
  solution = puzzle_solver(pieces, solve_success, board_height, board_width);

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
    delete_copy(board_height, solution);
  }
  return 0;
}
