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

#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

int main(int argc, char** argv) { //  ./ip <input_filename> <upload_dir>
  /* READ FILE */
  vector<ListOfPoints> puzzle_pieces;

  const char* image_file = argv[1];
  const char* upload_dir = argv[2];

  int length =0;
  while (upload_dir[length] != '\0') {
    length++;
  }

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

  const char* pieces_file_name = "/pieces";
  char* pieces_file = new char[length + 1];
  strcpy(pieces_file, upload_dir);
  pieces_file = strcat(pieces_file, pieces_file_name);
  shape_matrix_write(pieces_file, pieces);

  return 0;
}
