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

/**
 * Helper function used to call shell commands.
 *  @params:
 *   command: the command to execute
 *  @returns:
 *   a string containing the output of the command
 */
string execute_command(const char* command) {

  // setup buffer
  array<char, 128> buffer;

  // contains the outputs
  string output;

  // attempt ot run the command
  shared_ptr<FILE> pipe(popen(command, "r"), pclose);  
  if (!pipe) throw std::runtime_error("popen() failed!");

  // push output of command into buffer
  while (!feof(pipe.get())) {
    if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
      output += buffer.data();
  }
  
  return output;
}

/**
 * Image Processor Module
 *  @shellCommandFormat: bin/ip <image_filename> <upload_directory>
 *  @params:
 *   <image_filename>: the path to the image we wish to process
 *   <upload_directory>: the directory to which we will store puzzle information
 */
int main(int argc, char** argv) {

  //////////// PART 1: PROCESS ARGUMENTS /////////////////////////////
  if (argc !=  3){
    cout << "There should be 3 arguments only!" << endl;
    return -1;
  }

  // extract command line arguments
  const string image_filename(argv[1]);
  const string upload_directory(argv[2]);

  // compute pieces filename
  const string pieces_filename = upload_directory + string("/pieces");

  // compute processing flag filename
  string processing_flag_filename = upload_directory + string("/processing");


  /////////// PART 2: EXTRACT PIECES FROM THE IMAGE ///////////////////
  // get the puzzle's pieces from the image
  vector<ListOfPoints> puzzle_pieces;
  find_coordinates(image_filename.c_str(), puzzle_pieces);


  /////////// PART 3: DISCRETIZE PIECES ///////////////////
  // rotate pieces
  cout << "Rotating Pieces..." << endl;
  vector<ListOfPoints> rotated_puzzle_pieces;
  rotate_shapes(puzzle_pieces, rotated_puzzle_pieces);
  cout << puzzle_pieces.size() << " Pieces rotated!" << endl;

  // translate pieces
  cout << "Translating shape to Boolean Matrix...." << endl;
  vector<ShapeMatrix> pieces;
  bool translate_success = shape_translate_all_shapes(rotated_puzzle_pieces, pieces);

  if (translate_success == false){
    cout << "INTERNAL ERROR: SHAPE TRANSLATE FAIL" << endl;
    ofstream output_file(pieces_filename.c_str());
    output_file << -2 << endl;
    output_file.close();

    if(!remove(processing_flag_filename.c_str())){
      cout << "Error deleting file" << endl;
      return -1;
    }
    
    return -1;
  }

  
  /////////// PART 4: SAVE DISCRETIZED PIECES INFO ///////////////////
  // find container (first piece) and extract its height, width and area 
  std::sort(pieces.rbegin(), pieces.rend());
  uint container_height = pieces[0].getHeight();
  uint container_width = pieces[0].getWidth();
  uint container_area = pieces[0].getShapeArea();

  // compute area of the other pieces
  uint pieces_area = 0;
  for (uint i = 1; i < pieces.size(); i++) {
    pieces_area += pieces[i].getShapeArea();
  }

  // check if areas match
  bool areas_match = pieces_area == container_area? true: false;

  // write pieces to the pieces file
  shape_matrix_write(pieces_filename.c_str(), pieces, areas_match);
  cout << "pieces file created at " << pieces_filename << endl;
  

  /////////// PART 5: FIND SOLUTIONS ///////////////////
  if (areas_match){
    // make a call to sp module via bin/sp directory_name state 1

    // quote sign will be used a lot
    string quote(" \" ");
    
    // compute command
    string command = string("bin/sp ") + upload_directory;
    command += quote + to_string(container_width) + " " + to_string(container_height);
    for (uint i = 0; i < container_area; i++) {
      command += " 0";
    }
    command += quote +  to_string(1);

    // print command to std output stream
    cout << command << endl;
    
    // make the call to sp module, and send the output to std output stream
    string output = execute_command(command.c_str());
    cout << output << endl;

    // create a stringstream using the output
    stringstream ansstream(output);
    int first_integer;

    ansstream >> first_integer;
    cout << "first integer is: " << first_integer << endl;

    // if first integer is not positive, there is no solution
    if (first_integer <= 0){
      ofstream output_stream(pieces_filename.c_str());
      output_stream << -3 << endl;
      output_stream.close();
    }
  }

  // remove processing file
  cout << "attempting to remove file: " << processing_flag_filename << endl;
  if (remove(processing_flag_filename.c_str()) != 0){
    cout << "Error deleting file" << endl;
    return -1;
  } 
  cout<< "File successfully deleted" << endl;
  
 
  return 0;
}
