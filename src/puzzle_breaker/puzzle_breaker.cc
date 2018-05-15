#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector

#include "common/shape_matrix_io.h"
using namespace std;

/**
  * Helper function to deallocate a 2d array
  *
*/
void delete_2d_array(int** array, uint height) {
  // free each row
  for (uint i = 0; i < height; i++) {
    delete[] array[i];
  }

  // free array
  delete[] array;
}

/**
 * Helper function which rotates a 2D-int array 90 degrees clockwise
 * this is used to rotate a found solution (such that 4 solutions will be available with a single solution)
 *  @params:
 *   board: 2D-int array
 *   height: the height of the board_solution param
 *   width: the width of the board_solution param
 */
void rotate_board(int** &board, int& height, int& width) {

  // compute the height and width of the rotated board
  int rotated_board_height = width;
  int rotated_board_width = height;

  // create a temporary 2D int array on the stack, containing the rotated values
  int rotated_board[rotated_board_width][rotated_board_height];

  for (int i = 0;  i < height; i++)
    for (int j = 0; j < width; j++)
      rotated_board[j][height - i - 1] = board[i][j];

  // free board
  delete_2d_array(board, height);

  // reassign board to a newly-created 2D array on the heap
  board = new int*[rotated_board_height];

  for (int i = 0; i < rotated_board_height; i++) {
    // assign a row on the heap
    board[i] = new int[rotated_board_width];

    // copy rotated board's value into board solution
    for (int j = 0; j < rotated_board_width; j++)
      board[i][j] = rotated_board[i][j];
  }

  // update the width and height
  width = rotated_board_width;
  height = rotated_board_height;
}

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


int get_adjacent_empty_area(int row, int col, int** board, int height, int width, int currentIndex, double* bias) {
  // return 0 if board[row][col] is  (i) a NOT valid square within boundaries of the board AND (ii) is NOT empty
  if (row >= height || col >= width || row < 0 || col < 0 || board[row][col] != 0)
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


int get_adjacent_value (int row, int col, int** board, int height, int width) {
  //logical check for out of bounds
  if (row >= height || col >= width || row < 0 || col < 0) {
    cout << row << ", " << col << ": out of bounds!" << endl;
    return -1;
  }
  return board[row][col];
}




//Idea of this function is that if we have a single unit sized empty cell block, assign it one of the neighbouring numbers
bool assign_adjacent_value (int row, int col, int** board, int height, int width) {
  cout << " inside assign value function" << endl;
  int temp[4];
  temp[0] = get_adjacent_value(row, col + 1, board, height, width);
  cout << "right side value is " << temp [0] << endl;
  temp[1] = get_adjacent_value(row, col - 1, board, height, width);
  cout << "left side value is " << temp [1] << endl;
  temp[2] = get_adjacent_value(row + 1, col, board, height, width);
  cout << "top side value is " << temp [2] << endl;
  temp[3] = get_adjacent_value(row - 1, col, board, height, width);
  cout << "bottom side value is " << temp [3] << endl;

  int counter = 0;
  while (counter < 4) {
      if (temp[counter] > 0) {
        board[row][col] = temp[counter];
        cout << "assigned the number " << counter << "th value: "<< board[row][col] << endl;
        return true;
      }
      counter ++;
  }
  return false;
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

vector<int> randomize_vector(int min_index, int max_index){
  vector<int> temp;

  for (int i = min_index; i <= max_index; i ++) {
    temp.push_back(i);
  }

  // for (uint j = 0; j < temp.size(); j ++) {
  //   cout << temp[j] << endl;
  // }

  random_shuffle ( temp.begin(), temp.end() );
  //cout << "randomized" << endl;
  // for (uint i = 0; i < temp.size(); i ++) {
  //
  //   cout << temp[i] << endl;
  // }
  return temp;
}

void write_shapes_to_file(int**& intArray, int& height, int& width, const string filename, int maxIndex){
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

   vector<int> randVec = randomize_vector(2, maxIndex - 1);

  for (uint i = 0 ; i < randVec.size(); i++) {
    int index = randVec[i];
    while(r2() > 0.5) {
      rotate_board(intArray, height, width);
    }

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

void write_solution_to_file(int**& board_solution, string filename, uint height, uint  width){
  // open stream
  ofstream out(filename.c_str());
  // send board data to stream
  for (uint row = 0; row < height; row++) {
    for (uint col = 0; col < width; col++) {
      out << board_solution[row][col] << " ";
    }
  }

  // add line break and close stream
  out << endl;
  out.close();
}

bool assign_random_numbers(int** intArray, int& height, int& width) {
  int begArea = height*width;
  int currIndex = 2;

  while(begArea > 0) {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        int temp = get_adjacent_empty_area (i, j, intArray, height, width, currIndex, new double(1.0));
        if (temp) {
          if (temp == 1) {
            cout << " need to assign value at row" << i << ", col" << j << endl;

            if (!assign_adjacent_value (i, j, intArray, height, width)) {
              return false;
            }
          }
          else {
            currIndex++;
          }
        }
        begArea -= temp;
        cout << "Area is now " << begArea << endl;
      }
    }
  }
  print(intArray,height,width);
  write_shapes_to_file(intArray, height, width, "pieces", currIndex); //"<current_directory>/pieces"

  write_solution_to_file(intArray, "first", height, width); //<current_directory>/solutions/first

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
  cout<<h <<"and "<< w<<endl;

  assign_random_numbers(board, h, w);
  cout << r2() << endl;
  cout << r2() << endl;

  delete[]board;
  return 0;
}
