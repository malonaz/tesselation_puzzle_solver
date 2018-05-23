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
#include "common/types.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


#include <vector>
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

using std::vector;
using std::cout;
using std::endl;

using cv::resize;
using cv::cvtColor;
using cv::findContours;
using cv::approxPolyDP;
using cv::imread;
using cv::Mat;
using cv::adaptiveThreshold;
using cv::COLOR_BGR2GRAY;
using cv::RETR_EXTERNAL;
using cv::CHAIN_APPROX_SIMPLE;
using cv::ADAPTIVE_THRESH_GAUSSIAN_C;
using cv::THRESH_BINARY_INV;
using cv::arcLength;
using cv::contourArea;
using cv::WINDOW_AUTOSIZE;
using cv::waitKey;
using cv::Scalar;
using cv::circle;
using cv::Size;


#define MAX_VALUE 128
#define SCALE_DOWN_IMAGE_HEIGHT 800
#define SCALE_DOWN_IMAGE_WIDTH 1280
#define MIN_CONTOUR_AREA 1200
#define PIXEL_EPSILON 15


#define EPSILON_2D_IMAGE 0.05
#define EPSILON_2D_PICTURE 0.10
#define EPSILON_3D_PICTURE 0.15


#define NO_ERROR 0
#define ERROR_DELETING_TEMP_FILE -1

using namespace std;

void find_coordinates_3D2(Mat &image_matrix, vector<ListOfPoints> &polygons_corners){

  // print message
  cout << "Identified picture of 3D shapes" << endl;
  
  // adaptive threshold
  adaptiveThreshold(image_matrix, image_matrix, MAX_VALUE,
		    ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 27, 15);
}


void find_coordinates_2D2(Mat &image_matrix, vector<ListOfPoints> &polygons_corners){

  // print message
  cout << "Identified image of 2D shapes" << endl;
  
  // adaptive threshold
  adaptiveThreshold(image_matrix, image_matrix, MAX_VALUE,
  		    ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 31, 2);

}


void find_coordinates_2D_picture2(Mat &image_matrix){

  // print message
  cout << "Identified picture of 2D shapes" << endl;
  
  /// Reduce noise with a kernel 1 x 1
  cv::blur(image_matrix, image_matrix, Size(1, 1));
  debug_image(image_matrix);
  
  /// canny edge detector
  int lowThreshold = 20, kernel_size = 3, maxThreshold = 255;
  cv::Canny(image_matrix, image_matrix, lowThreshold, maxThreshold, kernel_size, true);
  debug_image(image_matrix);
  
  // floodfill
  cv::floodFill(image_matrix, cv::Point(0, 0), Scalar(255, 255, 255));
  debug_image(image_matrix);
  
  // invert because identify_polygons will need shapes to be filled in white
  cv::bitwise_not(image_matrix, image_matrix);
  debug_image(image_matrix);
  
  /// Reduce noise with a kernel 5 x 5 kernel
  cv::blur(image_matrix, image_matrix, Size(5, 5));
  debug_image(image_matrix);
}


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

  //////////// PART 1: 2D IMAGE FILTER /////////////////////////////

  string image_filename = "demo_malon_data/1.jpeg";
  Mat image_matrix = imread(image_filename);
  scale_down_image(image_matrix, image_matrix);
  debug_image(image_matrix);
  
  standardize_image(image_matrix);
  
  debug_image(image_matrix);

  vector<ListOfPoints> puzzle_pieces;
  find_coordinates_2D_picture2(image_matrix);
  find_coordinates(image_filename.c_str(), puzzle_pieces);
  debug_coordinates(image_filename.c_str(), puzzle_pieces);
  

  //////////// PART 2: 2D IMAGE /////////////////////////////
  image_filename = "puzzle2.jpg";
  image_matrix = imread(image_filename);
  scale_down_image(image_matrix, image_matrix);
  debug_image(image_matrix);
  
  standardize_image(image_matrix);
  debug_image(image_matrix);

  find_coordinates_2D2(image_matrix, puzzle_pieces);
  puzzle_pieces.clear();
  debug_image(image_matrix);
  find_coordinates(image_filename.c_str(), puzzle_pieces);
  debug_coordinates(image_filename.c_str(), puzzle_pieces);




  //////////// PART 3: 3D image /////////////////////////////
  image_filename = "demo_malon_data/4.JPG";
  image_matrix = imread(image_filename);
  scale_down_image(image_matrix, image_matrix);
  debug_image(image_matrix);
  
  standardize_image(image_matrix);
  debug_image(image_matrix);

  find_coordinates_3D2(image_matrix, puzzle_pieces);
  puzzle_pieces.clear();
  debug_image(image_matrix);
  find_coordinates(image_filename.c_str(), puzzle_pieces);
  debug_coordinates(image_filename.c_str(), puzzle_pieces);

  return -1;

  
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
