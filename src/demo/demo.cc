#include "common/coordinates_io.h"
#include "common/debugger.h"
#include "common/memory.h"
#include "common/point.h"
#include "common/puzzle_board.h"
#include "common/shape_matrix_io.h"
#include "common/shape_matrix.h"
#include "common/types.h"
#include "common/utils.h"
#include "discretizer/shape_rotate.h"
#include "discretizer/shape_translate.h"
#include "imagereader/image_read.h"
#include "solver/solver.h"

#include <fstream>
#include <iostream>
#include <vector>

#define EXPECTED_ARG_NUM 1
#define ARG_FORMAT "bin/demo <image_filename>"

using namespace std;

/**
 * Demo Module
 * solves a puzzle from an image if possible.
 *  @steps:
 *   1) process arguments
 *  @shellCommandFormat: bin/demo <image_filename> 
 *  @params:
 *   <image_filename>: the path to the image we wish to process
 */
int main(int argc, char** argv){
  
  //////////// PART 1: PROCESS ARGUMENTS /////////////////////////////
  if (!valid_args(argc, EXPECTED_ARG_NUM, ARG_FORMAT))
    return -1;

  const string image_filename(argv[1]);


  //////////// PART 2: GET PIECES FROM IMAGE /////////////////////////////
  vector<ListOfPoints> puzzle_pieces;
  find_coordinates(image_filename.c_str(), puzzle_pieces);
  debug_coordinates(image_filename.c_str(), puzzle_pieces);


  //////////// PART 3: DISCRETIZE PIECES /////////////////////////////
  // rotate pieces
  cout<< "Rotating Pieces..." << endl;
  vector<ListOfPoints> rotated_puzzle_pieces;
  rotate_shapes(puzzle_pieces, rotated_puzzle_pieces);
  print_list_of_shapes(rotated_puzzle_pieces);
  cout << puzzle_pieces.size() << " Pieces rotated!" << endl;

  // translate pieces
  cout << "Translating shapes to Boolean Matrix...." << endl;
  vector<ShapeMatrix> pieces;
  bool translate_success = shape_translate_all_shapes(rotated_puzzle_pieces, pieces);
  if (translate_success == false){
    cout << "INTERNAL ERROR: SHAPE TRANSLATE FAIL" << endl;
    return -1;
  }
  print_area(pieces);
  cout << "Translation complete!" << endl;

  //////////// PART 4: SOLVE PUZZLE /////////////////////////////
  cout << "Solving Puzzle...." <<endl;
  int return_code;
  int** solution;
  uint board_height, board_width;
  solution = puzzle_solver(pieces, return_code, board_height, board_width);


  //////////// PART 5: INTERPRET RETURN CODE /////////////////////////////
  switch (return_code) {
    case SOLVED:
      cout << "Puzzle is solved !!" << endl;
      break;
    case UNDERSIZED:
      cout << "Puzzle pieces cannot fit the container: potentially more pieces than required!!" << endl;
      return -1;
    case OVERSIZED:
      cout << "Container not fully filled: potentially less pieces than required!!" << endl;
      return -1;
    case UNSOLVED:
      cout << "Puzzle cannot be solved !!" << endl;
      return -1;
    default:
      cout << "INTERNAL ERROR: SOLVER ERROR" << endl;
      return -1;
  }

  if (return_code == SOLVED) {
    print_solution_board(solution, board_height, board_width);
    delete_2d_array(solution, board_height);
  }
  
  return 0;
}
