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

void print_board(const int** board, uint height, uint width) {
  cout << "-- [ rows: " << height << ", cols: " << width << " ] --" << endl;
  cout << "+";
  for (uint c = 0; c < width; ++c) {
    if (c < width - 1
        && board[0][c] == board[0][c + 1]) {
      cout <<  "-----";
    } else {
      cout <<  "----+";
    }
  }
  cout << endl;
  for (uint r = 0; r < height; ++r) {
    cout << "| ";
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
        cout << "| ";
      }
    }
    cout << endl;
    cout << "+";
    for (uint c = 0; c < width; ++c) {
      int current_num = board[r][c];
      if (r < height - 1 && c < width - 1) {
        if (board[r + 1][c + 1] == current_num
            && board[r + 1][c] == current_num
            && board[r][c + 1] == current_num) {
          cout << "     ";
        } else if (board[r + 1][c] == current_num
            && board[r][c + 1] == current_num) {
              cout << "    +";
        } else if (board[r + 1][c] == current_num) {
          cout << "    |";
        } else if (board[r][c + 1] == current_num) {
          cout << "-----";
        } else {
          cout << "----+";
        }
      } else if (r < height - 1 && c == width - 1) {
        if (board[r + 1][c] == current_num) {
          cout << "    |";
        } else {
          cout << "----+";
        }
      } else if(r == height - 1 && c < width - 1) {
        if (board[r][c + 1] == current_num) {
          cout << "-----";
        } else {
          cout << "----+";
        }
      } else {
        cout << "----+";
      }
    }
    cout << endl;
  }
}


void print_solution_board(int** const board, uint height, uint width) {
  cout << "-- [ rows: " << height << ", cols: " << width << " ] --" << endl;
  cout << "+";
  for (uint c = 0; c < width; ++c) {
    if (c < width - 1
        && board[0][c] == board[0][c + 1]) {
      cout <<  "-----";
    } else {
      cout <<  "----+";
    }
  }
  cout << endl;
  for (uint r = 0; r < height; ++r) {
    cout << "| ";
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
        cout << "| ";
      }
    }
    cout << endl;
    cout << "+";
    for (uint c = 0; c < width; ++c) {
      int current_num = board[r][c];
      if (r < height - 1 && c < width - 1) {
        if (board[r + 1][c + 1] == current_num
            && board[r + 1][c] == current_num
            && board[r][c + 1] == current_num) {
          cout << "     ";
        } else if (board[r + 1][c] == current_num
            && board[r][c + 1] == current_num) {
              cout << "    +";
        } else if (board[r + 1][c] == current_num) {
          cout << "    |";
        } else if (board[r][c + 1] == current_num) {
          cout << "-----";
        } else {
          cout << "----+";
        }
      } else if (r < height - 1 && c == width - 1) {
        if (board[r + 1][c] == current_num) {
          cout << "    |";
        } else {
          cout << "----+";
        }
      } else if(r == height - 1 && c < width - 1) {
        if (board[r][c + 1] == current_num) {
          cout << "-----";
        } else {
          cout << "----+";
        }
      } else {
        cout << "----+";
      }
    }
    cout << endl;
  }
}
