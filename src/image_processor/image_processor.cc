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
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

using namespace std;

string exec(const char* cmd) {
    array<char, 128> buffer;
    string result;
    shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

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

  // bin/sp directory_name state 1
  string directory_name = argv[2];
  // int n1 = strlen("bin/sp ");
  // int n2 = strlen(directory_name);
  // int n3 = (3 + container_area * 2 + 2);
  //
  // char [(n1 + n2 + n3 + 1)];
  //
  // strcpy(hashFileName, directory_name);
  // strncat(hashFileName, "/solutions/", n2);
  // strncat(hashFileName, strHashOfSoln.c_str(), n3);

  string cmd = "bin/sp ";

  cmd += directory_name;


  uint width = pieces[0].getWidth();
  uint height = pieces[0].getHeight();

  cmd += " \"";
  cmd += width + '0';
  cmd += " ";
  cmd += height + '0';

  for (uint i = 0; i < container_area; i++) {
    cmd += " 0";
  }

  cmd += "\" 1";

  cout << cmd << endl;

  int cmdLen = cmd.length();

  char charCmd[(cmdLen + 1)];

  for (int j = 0; j < cmdLen; j++) {
    charCmd[j] = cmd[j];
  }
  charCmd[cmdLen] = '\0';

  string result = exec(charCmd);

  cout << result << endl;



  return 0;
}
