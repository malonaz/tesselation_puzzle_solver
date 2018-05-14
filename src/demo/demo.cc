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

#define EXPECTED_ARG_NUM {1}
#define ARG_FORMAT "bin/demo <image_filename>"
#define NO_ERROR 0
#define ERROR_DELETING_TEMP_FILE -1

using namespace std;

/**
 * Helper function which deletes the processing flag
 *  @params:
 *   processing_flag_filename: the filepath of the processing flag to delete
 *  @returns:
 *   NO_ERROR or ERROR_DELETING_PROCESSING_FLAG
 */
int delete_temp_file(string pieces_filename){

  cout << "attempting to remove file: " << pieces_filename << endl;
  if (remove(pieces_filename.c_str()) != 0){
    cerr << "Error deleting file" << endl;
    return ERROR_DELETING_TEMP_FILE;
  }

  cout << "File successfully deleted" << endl;

  return NO_ERROR;
}

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
 * Demo Module
 * solves a puzzle from an image if possible.
 *  @steps:
 *   1) process arguments
 *   2) extract pieces from image
 *   3) discretize pieces
 *   4) solve puzzle
 *   5) interpret return code
 *  @shellCommandFormat: bin/demo <image_filename>
 *  @params:
 *   <image_filename>: the path to the image we wish to process
 */
int main(int argc, char** argv){

  //////////// PART 1: PROCESS ARGUMENTS /////////////////////////////
  if (!valid_args(argc, EXPECTED_ARG_NUM, ARG_FORMAT))
    return -1;

  const string image_filename(argv[1]);


  //////////// PART 2: EXTRACT PIECES FROM IMAGE /////////////////////////////
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
  if (!translate_success){
    cout << "INTERNAL ERROR: SHAPE TRANSLATE FAIL" << endl;
    return -1;
  }
  print_area(pieces);
  cout << "Translation complete!" << endl;

  //////////// PART 4: WRITING PUZZLE PIECES INFO TO FILE /////////////////////////////

  // preparation of information
  // compute area of the other pieces
  std::sort(pieces.rbegin(), pieces.rend());
  uint container_height = pieces[0].getHeight();
  uint container_width = pieces[0].getWidth();
  uint container_area = pieces[0].getShapeArea();
  uint pieces_area = 0;
  for (uint i = 1; i < pieces.size(); i++) {
    pieces_area += pieces[i].getShapeArea();
  }
  bool areas_match = pieces_area == container_area? true: false;
  shape_matrix_write("./pieces", pieces, areas_match);
  const string solver_program = "bin/sp";

  /////////// PART 5: FIND SOLUTION USING SOLVER PROGRAM BINARY ///////////////////
  if (areas_match){
    // quote sign will be used a lot

    string quote("\"");

    // compute command
    string command = solver_program + " . ";
    command += quote + to_string(container_width) + ' ' + to_string(container_height);
    for (uint i = 0; i < pieces[0].getMatrixArea(); i++) {
      command += pieces[0].get(i)? " 0": " -9" ;
    }
    command += quote + ' ' + to_string(0);

    // print command to std output stream
    cout << command << endl;

    // make the call to sp module, and send the output to std output stream
    string output = execute_command(command.c_str());
    cout << output << endl;
    // create a stringstream using the output
    stringstream ansstream(output);
    int first_integer;

    // Check if there is a solution
    ansstream >> first_integer;

    // if first integer is not positive, there is no solution
    if (first_integer <= 0){
      cout << "Error code" << first_integer;
      cout << "There is no solution." << endl;

    } else {
        // copy and print solution
      ansstream >> first_integer; //remove second integer.
      int** solution = new int*[container_height];
      for (uint i = 0; i < container_height; i ++) {
        solution[i] = new int [container_height];
        for (uint j = 0; j < container_width; j ++) {
          ansstream >> solution[i][j];
        }
      }
      print_solution_board(solution, container_height, container_width);


      for (uint i = 0; i < container_height; i ++) {
        delete solution[i];
      }
      delete [] solution;
    }

    // if first integer is not positive, there is no solution
    // if (first_integer <= 0){
    //   ofstream output_stream(pieces_filename.c_str());
    //   output_stream << -3 << endl;
    //   output_stream.close();
    // }
  }

  return delete_temp_file("./pieces");
}
