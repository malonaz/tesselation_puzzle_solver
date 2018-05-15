#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cstdio>

#include "common/coordinates_io.h"
#include "common/debugger.h"
#include "common/memory.h"
#include "common/point.h"
#include "common/puzzle_board.h"
#include "common/shape_matrix_io.h"
#include "common/shape_matrix.h"
#include "common/types.h"
#include "common/utils.h"
#include "solver/solver.h"

using namespace std;

void print(int** intarray, int rows, int cols){
  cout<<endl;
  for (int i = 0; i <rows; i++) {
    for (int j = 0; j <cols; j++) {
      cout << *(*(intarray+i)+j) << " ";
      if (*(*(intarray+i)+j)<10) {
        cout<<" ";
      }
    }
    cout << endl;
  }
}

int** makeIntArray(int rows, int cols){
  int** board = new int*[rows];
  for (int i = 0; i <rows; i++) {
    board[i] = new int[cols];
    for (int j = 0; j <cols; j++) {
      board[i][j]=0;
    }
  }
    return board;
}

double r2() {
    return (double)rand() / (double)RAND_MAX ;
}


int get_adjacent_empty_area(uint row, uint col, int** board, uint height, uint width, int currentIndex, double* bias) {
  // return 0 if board[row][col] is  (i) a NOT valid square within boundaries of the board AND (ii) is NOT empty
  if (row >= height || col >= width || board[row][col] != 0)
    return 0;

  if (r2()+*bias < 0.5) return 0;

  // mark this empty square equal to -1
  board[row][col] = currentIndex;
  *bias -= 0.35;

  // make recursive call to check the remaining adjacent area
  return 1 + get_adjacent_empty_area(row, col + 1, board, height, width, currentIndex, bias)
    + get_adjacent_empty_area(row, col - 1, board, height, width, currentIndex, bias)
    + get_adjacent_empty_area(row + 1, col, board, height, width, currentIndex, bias)
    + get_adjacent_empty_area(row - 1, col, board, height, width, currentIndex, bias);
}


void test_dimensions(int** intArray, int index, int height, int width,int& shapeH, int& shapeW, int& begR, int& begC){

  shapeH = 0;
  shapeW = 0;
  begR = -1;
  begC = -1;


  for (int i =0; i < height; i++){
    bool rowHasIndex = false;
    for (int j =0; j < width; j++){
      if (intArray[i][j]==index) {
        if (!rowHasIndex) {shapeH++; rowHasIndex = true;}
        if (begR < 0 ) {begR = i;}
      }
    }
  }

  for (int i =0; i < width; i++){
    bool colHasIndex = false;
    for (int j =0; j < height; j++){
      if (intArray[j][i]==index) {
        if (!colHasIndex) {shapeW++; colHasIndex = true;}
        if (begC < 0 ) {begC = i;}
      }
    }
  }
}

void write_shapes_to_file(int** intArray, int& height, int& width, const string filename, int maxIndex){
 ofstream output_file;
 output_file.open(filename);
 if(output_file.fail()){
   cout<<"failed!"<<endl;
   return;
 }

   int containerArea  = height * width;
   output_file << 1 << ' ';
   output_file << width << ' ';
   output_file << height << ' ';
   for (int i = 0; i < containerArea; i++){
     output_file << 1 << ' ';
   }
   output_file<<endl;

  for (int index = 2; index< maxIndex; index++){
    // while(r2() > 0.5) {
    //   rotate_board_solution(intArray, height, width, 1);
    // }
    int shapeH, shapeW, begR, begC;
    test_dimensions(intArray, index, height, width, shapeH, shapeW, begR, begC);

    output_file << index << ' ';
    output_file << shapeW << ' ';
    output_file << shapeH << ' ';

    for (int i = 0; i < shapeH; i++) {
      for (int j = 0; j< shapeW; j++){
        if (intArray[i+begR][j+begC] == index){
          output_file<< 1<<' ';
        } else {
          output_file<< 0<<' ';
        }
      }
    }
    output_file<< endl;
  }



  output_file.close();

}

void write_solution_to_file(int** board_solution, string filename, uint height, uint   width){

  // open stream
  ofstream out(filename.c_str());

  // send board data to stream
  for (uint row = 0; row < height; row++)
    for (uint col = 0; col < width; col++)
      out << board_solution[row][col] << " ";

  // add line break and close stream
  out << endl;
  out.close();
}

bool assign_random_numbers(int** intArray, int& height, int& width) {
  int begArea = height*width;
  int currIndex = 2;

  while(begArea>0){
    for (int i = 0; i < height; i++){
      for (int j = 0; j < width; j++){
        int temp = get_adjacent_empty_area (i, j, intArray, height, width, currIndex, new double(1.0));
        if (temp) currIndex++;
        begArea -= temp;
      }
    }
  }
  print(intArray,height,width);
  write_shapes_to_file(intArray, height, width, "pieces", currIndex); //"src/puzzle_breaker/pieces"
  write_solution_to_file(intArray, "first", height, width); //src/puzzle_breaker/solutions/first
  return true;
}

int main(int argc, char** argv){
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts); //nanosecond precision needed
  srand((time_t)ts.tv_nsec);
  //srand( time( NULL ) );
  stringstream h1(argv[1]);
  //stringstream w2(argv[2]);

  int h,w;
  h1 >> h;
  w = h;

  cout<<h <<"and "<< w<<endl;
  int** board = makeIntArray(h,w);

  print(board,h,w);
  assign_random_numbers(board, h, w);
  cout << r2() << endl;
  cout << r2() << endl;

  delete[]board;
  return 0;
}
