#include "debugger.h"

#include <iostream>

#include "types.h"

using std::cout;
using std::endl;

void print_list_of_shapes(const ListOfShapes* const list) {
 for (uint i = 0; i < list->size(); i++){
   cout << "Shape " << i << endl;
   print_list_of_points((*list)[i]);
 }

 cout << endl;
}

void print_list_of_points(const ListOfPoints* const shape_points) {
 for (uint i = 0; i < shape_points->size(); i++){
   cout << (*shape_points)[i];

   if (i < shape_points->size() - 1) {
     // add comma after each (x, y) unless it is the last
     cout << ", ";
   }
 }

 cout << endl;
}

void print_area(const ListOfShapeMatrices* const matrices) {
  int matricesSize = (int)matrices->size();
  int total = 0;
  cout << "---- Area ----" << endl;
  for (int i = 0; i < matricesSize; i++) {
    int tempArea = (*matrices)[i]->getShapeArea();
    total += tempArea;
    cout << "Shape: " << i+1 <<", Area: "<< tempArea << endl;
  }
  cout << "Total Area: " << total << endl;
  cout << "--------------" << endl;
}

void print_solution_board(int** const board, uint height, uint width) {
  cout << "-- [ rows: " << height << ", cols: " << width << " ] --" << endl;
  cout << "┌";
  for (uint c = 0; c < width; ++c) {
    if (c < width - 1
        && board[0][c] == board[0][c + 1]) {
      cout <<  "─────";
    } else if (c == width - 1) {
      cout <<  "────┐";
    } else {
      cout <<  "────┬";
    }
  }
  cout << endl;
  for (uint r = 0; r < height; ++r) {
    cout << "│ ";
    for (uint c = 0; c < width; ++c) {
      int current_num = board[r][c];
      if (current_num<10) {
        cout<<" ";
      }
      cout << current_num << " ";
      if (c < width - 1
          && board[r][c + 1] == current_num) {
        cout << "  ";
      } else {
        cout << "│ ";
      }
    }
    cout << endl;
    if (r < height - 1) {
      if (board[r][0] == board[r + 1][0]) {
        cout << "│";
      } else {
        cout << "├";
      }
    } else {
      cout << "└";
    }
    for (uint c = 0; c < width; ++c) {
      int current_num = board[r][c];
      if (r < height - 1 && c < width - 1) {
        if (board[r + 1][c + 1] == current_num
            && board[r + 1][c] == current_num
            && board[r][c + 1] == current_num) {
          // all four quadrants same
          cout << "     ";
        } else if (board[r + 1][c + 1] == current_num
            && board[r + 1][c] == current_num
            && board[r][c + 1] != current_num) {
          // only top-right corner is different
          cout << "    └";
        } else if (board[r + 1][c + 1] != current_num
            && board[r + 1][c] == current_num
            && board[r][c + 1] == current_num) {
          // only bottom-right corner is different
          cout << "    ┌";
        } else if (board[r + 1][c + 1] == current_num
            && board[r + 1][c] != current_num
            && board[r][c + 1] == current_num) {
          // only bottom-left corner is different
          cout << "────┐";
        } else if (board[r + 1][c] == current_num
            && board[r + 1][c + 1] == board[r][c + 1]) {
          // both column-wise is the same
          cout << "    │";
        } else if (board[r][c + 1] == board[r + 1][c] &&
            board[r + 1][c] == board[r + 1][c + 1] &&
            board[r][c + 1] != current_num) {
          // only current one different
          cout << "────┘";
        } else if (board[r + 1][c] == current_num
            && board[r + 1][c + 1] != board[r][c + 1]) {
          // only left column same
          cout << "    ├";
        } else if (board[r + 1][c] != current_num
            && board[r + 1][c + 1] == board[r][c + 1]) {
          // only left column same
          cout << "────┤";
        } else if (board[r][c + 1] == current_num &&
            board[r + 1][c] == board[r + 1][c + 1]) {
          // both row-wise is the same
          cout << "─────";
        } else if (board[r][c + 1] == current_num &&
            board[r + 1][c] != board[r + 1][c + 1]) {
          // only top row-wise is the same
          cout << "────┬";
        } else if (board[r + 1][c] == board[r + 1][c + 1]) {
          // only bottom row-wise is the same
          cout << "────┴";
        } else {
          cout << "────┼";
        }

      // last column
      } else if (r < height - 1 && c == width - 1) {
        if (board[r + 1][c] == current_num) {
          cout << "    │";
        } else {
          cout << "────┤";
        }

      // last row
      } else if(r == height - 1 && c < width - 1) {
        if (board[r][c + 1] == current_num) {
          cout << "─────";
        } else {
          cout << "────┴";
        }
      } else {
        cout << "────┘";
      }
    }
    cout << endl;
  }
}
