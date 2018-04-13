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
#include <sstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

using namespace std;

/* This is a helper function to call shell commands. */
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


int main(int argc, char** argv) { // FORMAT:  bin/ip <input_filename> <upload_dir>
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

  const char* pieces_file_name = "/pieces";
  int length3 = strlen(pieces_file_name);
  char* pieces_file = new char[length + length3 + 1];
  strcpy(pieces_file, upload_dir);
  pieces_file = strcat(pieces_file, pieces_file_name);


  string str_processing_file = argv[2];
  str_processing_file += "/processing";

  int process_file_len = str_processing_file.length();
  char processing_file [(process_file_len + 1)];
  strcpy (processing_file, str_processing_file.c_str());

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
    ofstream output_file;
    output_file.open(pieces_file);
    output_file << -2 << endl;
    output_file.close();

    if( remove( processing_file ) != 0 ){
        cout << "Error deleting file" <<endl;
        return 0;
    }
    return 1;
  }

  // find container (first piece) and save its area as container_area
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

  cout << "here" <<endl;

  // const char* name = "/processing";
  // int length2 = strlen(name);
  // char* processing_file = new char[length + length2 + 1];
  // strcpy(processing_file, upload_dir);
  // processing_file = strcat(processing_file, name);

  cout << "file to remove: "<< processing_file <<endl;

    cout<< "File successfully deleted" <<endl;
    shape_matrix_write(pieces_file, pieces, correct_area);
    cout << "pieces file created at " << pieces_file <<endl;


  bool solution_exists = false;

  if (correct_area){
    // make a call to sp module via bin/sp directory_name state 1

    /**** string manipulation functions to prepare cmd which will be used as
    input for shell command***/
    string directory_name = argv[2];
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
    /**** end of string manipulation***/

    cout << result << endl;
    stringstream ansstream(result);
    int first_int;
    //while (ansstream){
    ansstream >> first_int;
    cout << "first integer is: " << first_int << endl;
    // }
    if (first_int > 0) {
      solution_exists = true;
    }

    if (!solution_exists){
      ofstream output_file;
      output_file.open(pieces_file);
      output_file << -3 << endl;
      output_file.close();
    }
  }

  if( remove( processing_file ) != 0 ){
      cout << "Error deleting file" <<endl;
      return 0;
  }

  return 0;
}
