#include "common/coordinates_io.h"
#include "common/point.h"
#include "common/puzzle_board.h"
#include "common/shape_matrix_io.h"
#include "common/shape_matrix.h"
#include "common/types.h"
#include "discretizer/shape_rotate.h"
#include "discretizer/shape_translate.h"
#include "imagereader/image_read.h"

#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

int main(int argc, char** argv) { //  ./ip <input_filename> <upload_dir>
  /* READ FILE */
  if (argc !=  3){
    cout << "Not enough arguments!" <<endl;
    return 0;
  }

  const char* image_file = argv[1];
  const char* upload_dir = argv[2];

  int length =0;
  while (upload_dir[length] != '\0') {
    length++;
  }
  vector<ListOfPoints> puzzle_pieces;
  find_coordinates(image_file, puzzle_pieces);

  cout<< "Rotating Pieces..." << endl;
  /* SHAPE TRANSLATE MODULE */
  vector<ListOfPoints> rotated_puzzle_pieces;
  rotate_shapes(puzzle_pieces, rotated_puzzle_pieces);

  cout << puzzle_pieces.size() << " Pieces rotated!" << endl;


  cout << "Translating shape to Boolean Matrix...." <<endl;
  /* DISCRETIZER MODULE */
  vector<ShapeMatrix> pieces;
  bool translate_success = shape_translate_all_shapes(rotated_puzzle_pieces, pieces);

  if (translate_success == false){
    cout << "INTERNAL ERROR: SHAPE TRANSLATE FAIL" <<endl;
    return 1;
  }

  // find container (first piece) and save its area
  std::sort(pieces.rbegin(), pieces.rend());

  uint pieces_area = 0;
  uint container_area = pieces[0].getShapeArea();
  bool correct_area = true;
  for (uint i = 1; i < pieces.size(); i++) {
    pieces_area += pieces[i].getShapeArea();
  }

  if (pieces_area != container_area) {
    correct_area = false;
  }

  const char* name = "/processing";
  char* processing_file = new char[length + 1];
  strcpy(processing_file, upload_dir);
  processing_file = strcat(processing_file, name);
  cout << "file to remove: "<< processing_file <<endl;

  if( remove( processing_file ) != 0 ){
    cout << "Error deleting file" <<endl;
    return 0;
  }
  else{
    cout<< "File successfully deleted" <<endl;
    const char* pieces_file_name = "/pieces";
    char* pieces_file = new char[length + 1];
    strcpy(pieces_file, upload_dir);
    pieces_file = strcat(pieces_file, pieces_file_name);
    shape_matrix_write(pieces_file, pieces, correct_area);
    cout << "pieces file created at " << pieces_file <<endl;
  };

  return 0;
}
