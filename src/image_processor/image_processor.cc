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
#include "discretizer/shape_rotate.h"
#include "discretizer/shape_translate.h"
#include "imagereader/image_read.h"


using namespace std;

int main(int argc, char** argv) { //  ./demo <input_filename>
  /* READ FILE */
  vector<ListOfPoints> puzzle_pieces;

  const char* image_file = "demo_data/puzzle6.jpg";
  const char* coordinate_file = "demo_data/puzzle2.txt";

  find_coordinates(image_file, puzzle_pieces);
  debug_coordinates(image_file, puzzle_pieces);

  puzzle_pieces.clear();

  cout << " Reading file ....." <<endl;
  bool file_read = read_coordinates_file(coordinate_file, puzzle_pieces);
  if (file_read == false){
    cout << "ERROR: FILE ERROR" << endl;
    return 1;
  }
  cout << "File Read Complete!" << endl;

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
  const char* file_out = "src/image_processor/pieces.txt";
  shape_matrix_write(file_out, pieces);
  
  return 0;
}
