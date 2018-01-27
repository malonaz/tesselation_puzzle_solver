
#include "puzzle_board.h"

#include <cstddef>
#include <iostream>

#include "shape_matrix.h"
#include "types.h"

using namespace std;

PuzzleBoard::PuzzleBoard(ShapeMatrix* shape):
    container(shape),
    current_board(NULL),
    remainingArea(shape->getShapeArea()) {
  int width = shape->getWidth();
  int height = shape->getHeight();
  this->current_board = new int*[height];
  for (int i = 0; i < height; i++) {
    current_board[i] = new int[width];
    for (int j = 0; j < width; j++) {
      current_board[i][j] = 0;
    }
    this->bitset[i] = 0;
  }
}

PuzzleBoard::PuzzleBoard(const PuzzleBoard &copy):
    container(new ShapeMatrix(*copy.container)),
    current_board(NULL),
    remainingArea(copy.remainingArea) {
  int width = copy.container->getWidth();
  int height = copy.container->getHeight();
  this->current_board = new int*[height];
  for (int i = 0; i < height; i++) {
    current_board[i] = new int[width];
    for (int j = 0; j < width; j++) {
      current_board[i][j] = copy.current_board[i][j];
    }
    this->bitset[i] = copy.bitset[i];
  }
}

uint PuzzleBoard::getHeight() const {
  return this->container->getHeight();
}

uint PuzzleBoard::getWidth() const {
  return this->container->getWidth();
}

bool PuzzleBoard::placePiece(int x, int y, uint idx, uint64 bitmask[64], ShapeMatrix* piece) {
  for (uint i = y; i < this->container->getHeight() && i < piece->getHeight() + y; ++i) {
    if ((this->bitset[i] & (bitmask[i - y] << x)) != 0) {
      return false;
    }
  }
  for (uint i = y; i < this->container->getHeight() && i < piece->getHeight() + y; ++i) {
    //this->remainingArea -= __builtin_popcount(bitmask[i - y] << x);
    this->bitset[i] |= (bitmask[i - y] << x);
  }
  this->remainingArea -= piece->getShapeArea();
  return true;
}

bool PuzzleBoard::removePiece(int x, int y, uint64 bitmask[64], ShapeMatrix* piece) {
  for (uint i = y; i < this->container->getHeight() && i < piece->getHeight() + y; ++i) {
    //this->remainingArea += __builtin_popcount(bitmask[i - y] << x);
    this->bitset[i] ^= (bitmask[i - y] << x);
  }
  this->remainingArea += piece->getShapeArea();
  return true;
}

int** PuzzleBoard::getCurrentBoard() const {
  return this->current_board;
}

uint PuzzleBoard::getRemainingArea() const {
  return this->remainingArea;
}

void PuzzleBoard::printBoard() const {
  int width = this->container->getWidth();
  int height = this->container->getHeight();
  cout << "-- [ rows: " << height << ", cols: " << width << " ] --" << endl;
  cout << "+";
  for (int c = 0; c < width; ++c) {
    if (c < width - 1
        && this->current_board[0][c] == this->current_board[0][c + 1]) {
      cout <<  "----";
    } else {
      cout <<  "---+";
    }
  }
  cout << endl;
  for (int r = 0; r < height; ++r) {
    cout << "| ";
    for (int c = 0; c < width; ++c) {
      int current_num = this->current_board[r][c];
      cout << current_num << " ";
      if (c < width - 1
          && this->current_board[r][c + 1] == current_num) {
        cout << "  ";
      } else {
        cout << "| ";
      }
    }
    cout << endl;
    cout << "+";
    for (int c = 0; c < width; ++c) {
      int current_num = this->current_board[r][c];
      if (r < height - 1 && c < width - 1) {
        if (this->current_board[r + 1][c + 1] == current_num
            && this->current_board[r + 1][c] == current_num
            && this->current_board[r][c + 1] == current_num) {
          cout << "    ";
        } else if (this->current_board[r + 1][c] == current_num
            && this->current_board[r][c + 1] == current_num) {
              cout << "   +";
        } else if (this->current_board[r + 1][c] == current_num) {
          cout << "   |";
        } else if (this->current_board[r][c + 1] == current_num) {
          cout << "----";
        } else {
          cout << "---+";
        }
      } else if (r < height - 1 && c == width - 1) {
        if (this->current_board[r + 1][c] == current_num) {
          cout << "   |";
        } else {
          cout << "---+";
        }
      } else if(r == height - 1 && c < width - 1) {
        if (this->current_board[r][c + 1] == current_num) {
          cout << "----";
        } else {
          cout << "---+";
        }
      } else {
        cout << "---+";
      }
    }
    cout << endl;
  }
}

PuzzleBoard& PuzzleBoard::operator=(const PuzzleBoard& rhs) {
  if (this == &rhs) {
    return *this;
  }

  int container_height = this->container->getHeight();
  delete container;
  for (int i = 0; i < container_height; i++) {
    delete[] current_board[i];
  }
  delete[] current_board;

  this->container = new ShapeMatrix(*rhs.container);

  int width = rhs.container->getWidth();
  int height = rhs.container->getHeight();
  this->current_board = new int*[height];
  for (int i = 0; i < height; i++) {
    current_board[i] = new int[width];
    for (int j = 0; j < width; j++) {
      current_board[i][j] = rhs.current_board[i][j];
    }
    this->bitset[i] = rhs.bitset[i];
  }


  return *this;
}

PuzzleBoard::~PuzzleBoard() {
  int container_height = this->container->getHeight();
  delete container;
  for (int i = 0; i < container_height; i++) {
    delete[] current_board[i];
  }
  delete[] current_board;
}
